/***************************************************************************
 *   Copyright (C) 2013 by Richard Crook                                   *
 *   http://code.google.com/p/qplanner                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "plan.h"
#include "daysmodel.h"
#include "calendar.h"
#include "day.h"

#include <QXmlStreamWriter>

/*************************************************************************************************/
/********************************* Single calendar for planning **********************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Calendar::Calendar()
{
  // create null calendar
  m_name        = "Null";
  m_cycleAnchor = QDate(2000,1,1);   // Saturday 1st Jan 2000
  m_cycleLength = 0;
  m_normal.resize( m_cycleLength );
}

/****************************************** constructor ******************************************/

Calendar::Calendar( int type )
{
  Day* working    = plan->day( Day::DEFAULT_STANDARDWORK );
  Day* nonWorking = plan->day( Day::DEFAULT_NONWORK );

  // create calendars
  if ( type == DEFAULT_CALENDAR )
  {
    // create default base calendar
    m_name        = "Standard";
    m_cycleAnchor = QDate(2000,1,1);   // Saturday 1st Jan 2000
    m_cycleLength = 7;                 // 7 day week
    m_normal.resize( m_cycleLength );

    for( int n=0 ; n<7 ; n++ )
    {
      if ( n < 2 )    // Sat + Sun
        m_normal[n] = nonWorking;
      else            // Mon to Fri
        m_normal[n] = working;
    }

    // add easter and some bank holidays
    m_exceptions[ QDate(2012,12,25) ] = nonWorking;
    m_exceptions[ QDate(2012,12,26) ] = nonWorking;

    m_exceptions[ QDate(2013, 1, 1) ] = nonWorking;
    m_exceptions[ QDate(2013, 3,29) ] = nonWorking;
    m_exceptions[ QDate(2013, 4, 1) ] = nonWorking;
    m_exceptions[ QDate(2013, 5, 6) ] = nonWorking;
    m_exceptions[ QDate(2013, 5,27) ] = nonWorking;
    m_exceptions[ QDate(2013, 8,26) ] = nonWorking;
    m_exceptions[ QDate(2013,12,25) ] = nonWorking;
    m_exceptions[ QDate(2013,12,26) ] = nonWorking;
  }
  else if ( type == DEFAULT_FANCY )
  {
    // create fancy calendar
    m_name        = "Fancy";
    m_cycleAnchor = QDate(2012,1,1);
    m_cycleLength = 10;                 // 10 day cycle
    m_normal.resize( m_cycleLength );

    m_normal[0] = nonWorking;
    m_normal[1] = nonWorking;
    m_normal[2] = nonWorking;
    m_normal[3] = plan->day( Day::DEFAULT_SHORT );
    m_normal[4] = plan->day( Day::DEFAULT_SHORT );
    m_normal[5] = plan->day( Day::DEFAULT_EVENING );
    m_normal[6] = plan->day( Day::DEFAULT_EVENING );
    m_normal[7] = plan->day( Day::DEFAULT_TWENTYFOURHOURS );
    m_normal[8] = nonWorking;
    m_normal[9] = plan->day( Day::DEFAULT_TWENTYFOURHOURS );

    m_exceptions[ QDate(2012,12,25) ] = nonWorking;
    m_exceptions[ QDate(2012,12,26) ] = nonWorking;
  }
  else  // DEFAULT_FULLTIME
  {
    // create fulltime calendar
    m_name        = "Full Time";
    m_cycleAnchor = QDate(2012,1,1);
    m_cycleLength = 1;                 // same day every day
    m_normal.resize( m_cycleLength );

    m_normal[0] = plan->day( Day::DEFAULT_TWENTYFOURHOURS );
  }
}

/****************************************** constructor ******************************************/

Calendar::Calendar( QXmlStreamReader* stream ) : Calendar()
{
  // create calendar from stream
  foreach( QXmlStreamAttribute attribute, stream->attributes() )
  {
    if ( attribute.name() == "name" )
      m_name = attribute.value().toString();

    if ( attribute.name() == "anchor" )
      m_cycleAnchor = QDate::fromString( attribute.value().toString(), "yyyy-MM-dd" );
  }

  while ( !stream->atEnd() )
  {
    stream->readNext();

    // if normal element append new day
    if ( stream->isStartElement() && stream->name() == "normal" )
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
        if ( attribute.name() == "day" )
        {
          m_normal.append( plan->day( attribute.value().toString().toInt() ) );
          m_cycleLength = m_normal.size();
        }
      }

    // if exception element append new exception
    if ( stream->isStartElement() && stream->name() == "exception" )
    {
      QDate  date;
      Day*   day = nullptr;
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
        if ( attribute.name() == "date" )
          date = QDate::fromString( attribute.value().toString(), "yyyy-MM-dd" );

        if ( attribute.name() == "day" )
          day = plan->day( attribute.value().toString().toInt() );
      }
      m_exceptions[ date ] = day;
    }

    // when reached end of calendar return
    if ( stream->isEndElement() && stream->name() == "calendar" ) return;
  }
}

/***************************************** saveToStream ******************************************/

void  Calendar::saveToStream( QXmlStreamWriter* stream )
{
  // write calendar data to xml stream
  stream->writeAttribute( "name", m_name );
  stream->writeAttribute( "anchor", m_cycleAnchor.toString(Qt::ISODate) );

  for( int n=0 ; n<m_cycleLength ; n++ )
  {
    stream->writeEmptyElement( "normal" );
    stream->writeAttribute( "id", QString("%1").arg(n) );
    stream->writeAttribute( "day", QString("%1").arg(plan->index(m_normal[n])) );
  }

  QHashIterator<QDate, Day*> e( m_exceptions );
  while ( e.hasNext() )
  {
      e.next();
      stream->writeEmptyElement( "exception" );
      stream->writeAttribute( "date", e.key().toString(Qt::ISODate) );
      stream->writeAttribute( "day", QString("%1").arg(plan->index(e.value())) );
  }
}

/********************************************* getDay ********************************************/

Day*  Calendar::getDay( QDate date ) const
{
  // if exception exists return it, otherwise return normal cycle day
  if ( m_exceptions.contains( date ) ) return m_exceptions.value( date );

  int normal = m_cycleAnchor.daysTo( date ) % m_cycleLength;
  if ( normal < 0 ) normal += m_cycleLength;

  return m_normal.at( normal );
}

/********************************************* addXXX ********************************************/

QDateTime  Calendar::addTimeSpan( QDateTime start, TimeSpan ts )
{
  // check if start is valid date-time
  if ( !start.isValid() )
  {
    qWarning("Calendar::addTimeSpan - invalid start");
    return QDateTime();
  }

  // if timespan is zero length return original start
  if ( ts.number() == 0.0 ) return start;

  // return date-time moved by TimeSpan
  if ( ts.units() == TimeSpan::UNIT_SECONDS ) return addSeconds( start, ts.number() );
  if ( ts.units() == TimeSpan::UNIT_MINUTES ) return addSeconds( start, ts.number()*60.0 );
  if ( ts.units() == TimeSpan::UNIT_HOURS )   return addSeconds( start, ts.number()*3600.0 );
  if ( ts.units() == TimeSpan::UNIT_DAYS )    return addDays(    start, ts.number() );
  if ( ts.units() == TimeSpan::UNIT_WEEKS )   return addWeeks(   start, ts.number() );
  if ( ts.units() == TimeSpan::UNIT_MONTHS )  return addMonths(  start, ts.number() );
  if ( ts.units() == TimeSpan::UNIT_YEARS )   return addYears(   start, ts.number() );

  // unknown timespace units - should never happen!
  qWarning("Calendar::addTimeSpan - unknown timespace units '%c'",ts.units());
  return start;
}

QDateTime  Calendar::addSeconds( QDateTime start, int secs )
{
  // check that secs is not zero
  if ( secs == 0 ) return start;         // nothing to add

  // if secs is positive go forward in time, else go backwards
  if ( secs > 0 )
  {
    // use up any remaining working seconds on start date
    QDate date  = start.date();
    Day*  today = getDay( date );
    int toGo  = today->secsToGo( start.time() );
    if ( toGo == secs ) return QDateTime( date, today->end() );
    if ( toGo >  secs ) return QDateTime( date, today->doSecs( start.time(), secs ) );

    // to go was insufficient so move to next day
    date  = date.addDays(1);
    secs -= toGo;

    // repeat forever until no need to move to next day
    while ( true )
    {
      // check if found finish date
      today = getDay( date );
      if ( today->seconds() == secs ) return QDateTime( date, today->end() );
      if ( today->seconds() >  secs ) return QDateTime( date, today->doSecs( QTime(), secs ) );

      // not finished so move to next day
      date  = date.addDays(1);
      secs -= today->seconds();
    }
  }
  else
  {
    // work backwards on any seconds done on start date
    secs = -secs;
    QDate date  = start.date();
    Day*  today = getDay( date );
    int done  = today->secsDone( start.time() );
    if ( done == secs ) return QDateTime( date, today->start() );
    if ( done >  secs ) return QDateTime( date, today->doSecs( today->start(), done - secs ) );

    // done was insufficient so move to previous day
    date  = date.addDays(1);
    secs -= done;

    // repeat forever until no need to move to previous day
    while ( true )
    {
      // check if found finish date
      today = getDay( date );
      if ( today->seconds() == secs ) return QDateTime( date, today->start() );
      if ( today->seconds() >  secs ) return QDateTime( date, today->doSecs( today->start(), today->seconds() - secs ) );

      // not finished so move to previous day
      date  = date.addDays(-1);
      secs -= today->seconds();
    }
  }
}

QDateTime  Calendar::addDays( QDateTime start, float days )
{
  // check that days is not zero
  if ( days == 0.0 ) return start;         // nothing to add

  // if days is positive go forward in time, else go backwards
  if ( days > 0.0 )
  {
    // use up any remaining working time on start date
    QDate date  = start.date();
    Day*  today = getDay( date );
    float toGo  = today->workToGo( start.time() );
    if ( toGo == days ) return QDateTime( date, today->end() );
    if ( toGo >  days ) return QDateTime( date, today->doWork( start.time(), days ) );

    // to go was insufficient so move to next day
    date  = date.addDays(1);
    days -= toGo;

    // repeat forever until no need to move to next day
    while ( true )
    {
      // check if found finish date
      today = getDay( date );
      if ( today->work() == days ) return QDateTime( date, today->end() );
      if ( today->work() >  days ) return QDateTime( date, today->doWork( QTime(), days ) );

      // not finished so move to next day
      date  = date.addDays(1);
      days -= today->work();
    }
  }
  else
  {
    // work backwards on any work done on start date
    days = -days;
    QDate date  = start.date();
    Day*  today = getDay( date );
    float done  = today->workDone( start.time() );
    if ( done == days ) return QDateTime( date, today->start() );
    if ( done >  days ) return QDateTime( date, today->doWork( today->start(), done - days ) );

    // done was insufficient so move to previous day
    date  = date.addDays(-1);
    days -= done;

    // repeat forever until no need to move to previous day
    while ( true )
    {
      // check if found finish date
      today = getDay( date );
      if ( today->work() == days ) return QDateTime( date, today->start() );
      if ( today->work() >  days ) return QDateTime( date, today->doWork( today->start(), today->work() - days ) );

      // not finished so move to previous day
      date  = date.addDays(-1);
      days -= today->work();
    }
  }
}

QDateTime  Calendar::addWeeks( QDateTime start, float weeks )
{
  // return date-time moved by weeks (7 day week ignores non-working days)
  return start.addSecs( weeks*7*24*3600 );
}

QDateTime  Calendar::addMonths( QDateTime start, float months )
{
  // return date-time moved by months
  int   whole    = floor(months);
  float fraction = months - whole;
  start = start.addMonths( whole );

  if ( fraction == 0.0 ) return start;
  QDate date1 = start.date();
  QDate date2 = date1.addMonths(1);
  int   diff  = date2.toJulianDay() - date1.toJulianDay();
  return start.addDays( diff * fraction );
}

QDateTime  Calendar::addYears( QDateTime start, float years )
{
  // return date-time moved by years
  int   whole    = floor(years);
  float fraction = years - whole;

  if ( fraction == 0.0 ) return start.addYears( whole );
  return addMonths( start.addYears( whole ), fraction*12.0 );
}

/****************************************** workBetween ******************************************/

TimeSpan Calendar::workBetween( QDateTime start, QDateTime end ) const
{
  // return work between two dates
  if ( !start.isValid() || !end.isValid() ) return TimeSpan( 0, TimeSpan::UNIT_DAYS );
  if ( start == end ) return TimeSpan( 0, TimeSpan::UNIT_DAYS );
  if ( start >  end ) qSwap( start, end );
  QDate sd   = start.date();
  QDate ed   = end.date();
  Day*  day  = getDay( sd );
  float work = day->workToGo( start.time() );

  if ( sd == ed ) return TimeSpan( work - day->workToGo( end.time() ), TimeSpan::UNIT_DAYS );

  for( QDate date = sd.addDays(1) ; date < ed ; date = date.addDays(1) )
    work += getDay( date )->work();

  work += getDay( ed )->workDone( end.time() );

  return TimeSpan( work, TimeSpan::UNIT_DAYS );
}

/********************************************* workUp ********************************************/

QDateTime  Calendar::workUp( QDateTime dt ) const
{
  // if input date-time is not valid, return null date-time
  if ( !dt.isValid() ) return QDateTime();

  QDate date = dt.date();
  QTime time = dt.time();
  Day*  day  = getDay( date );

  // if day is not working or on/after day end, move forward one day until working day is found
  while ( !day->isWorking() || time >= day->end() )
  {
    date = date.addDays(1);
    time = QTime(0,0,0);
    day  = getDay( date );
  }

  // if time is before day start, return day start
  if ( time < day->start() ) return QDateTime( date, day->start() );

  // otherwise check in working period
  for ( int n=0 ; n < day->periods()-1 ; n++ )
  {
      if ( time >= day->start(n) && time <= day->end(n) ) return QDateTime( date, time );
      if ( time >  day->end(n)   && time <= day->start(n+1) ) return QDateTime( date, day->start(n+1) );
  }

  return QDateTime( date, time );
}
/******************************************** workDown *******************************************/

QDateTime  Calendar::workDown( QDateTime dt ) const
{
  // return first valid date-time at or before
  if ( !dt.isValid() ) return QDateTime();

  QDate date = dt.date();
  QTime time = dt.time();
  Day*  day  = getDay( date );

  // if day is not working or on/before day start, move back one day until working day is found
  while ( !day->isWorking() || time <= day->start() )
  {
    date = date.addDays(-1);
    time = QTime(23,59,59);
    day  = getDay( date );
  }

  // if time is after day end, return day end
  if ( time > day->end() ) return QDateTime( date, day->end() );

  // otherwise check in working period
  for ( int n=day->periods()-1 ; n > 0 ; n-- )
  {
      if ( time >= day->start(n) && time <= day->end(n) ) return QDateTime( date, time );
      if ( time >= day->end(n-1) && time <  day->start(n) ) return QDateTime( date, day->end(n-1) );
  }

  return QDateTime( date, time );
}
/********************************************** data *********************************************/

QVariant  Calendar::data( int row, int role  = Qt::DisplayRole ) const
{
  // if role is EditRole, return appropriate edit value
  if ( role == Qt::EditRole )
  {
    if ( row >= ROW_NORMAL1 )
    {
      QString  name = m_normal.at(row - ROW_NORMAL1)->name();
      return   plan->days()->namesList().indexOf( name );
    }

    // if not handled above return DisplayRole
    role = Qt::DisplayRole;
  }

  // if role is DisplayRole, return appropriate display value
  if ( role == Qt::DisplayRole )
  {
    if ( row == ROW_NAME )        return m_name;
    if ( row == ROW_ANCHOR )      return m_cycleAnchor;
    if ( row == ROW_EXCEPTIONS )  return m_exceptions.size();
    if ( row == ROW_CYCLELENGTH ) return m_cycleLength;

    if ( row >= m_cycleLength+ROW_NORMAL1 ) return QVariant();

    return m_normal.at(row - ROW_NORMAL1)->name();
  }

  // if role is TextAlignmentRole, return appropriate display alignment
  if ( role == Qt::TextAlignmentRole )
  {
    return int( Qt::AlignVCenter | Qt::AlignLeft );
  }

  // if role is BackgroundRole, return appropriate background colour
  if ( role == Qt::BackgroundRole )
  {
    if ( row >= m_cycleLength+ROW_NORMAL1 ) return plan->nullCellColour();
    return QVariant();
  }

  // if role is ToolTipRole, return appropriate tool tip text
  if ( role == Qt::ToolTipRole )
  {
    if ( row < m_cycleLength+ROW_NORMAL1 &&
         row >= ROW_NORMAL1 )
    {
      QDate  anchor  = m_cycleAnchor;
      int    len     = m_cycleLength;
      int    normal  = anchor.daysTo( QDate::currentDate() ) % len;
      QDate  example = QDate::currentDate().addDays( row - ROW_NORMAL1 - normal );
      return example.toString( "ddd dd MMM yyyy" );
    }
    return QVariant( QVariant::String );
  }

  // otherwise return an invalid QVariant
  return QVariant();
}
