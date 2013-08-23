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

#include "day.h"
#include "plan.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

/*************************************************************************************************/
/**************************** Single day type used in plan calendars *****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Day::Day()
{
  // create null day
  m_name    = "Null";
  m_work    = 0.0;
  m_periods = 0;
}

/****************************************** constructor ******************************************/

Day::Day( int type )
{
  // create template day
  if ( type == DEFAULT_STANDARDWORK )
  {
    // default working day
    m_name     = "Standard work day";
    m_work     = 1.0;
    m_periods  = 2;
    m_start.resize(2);
    m_start[0] = QTime( 9,0);
    m_start[1] = QTime(14,0);
    m_end.resize(2);
    m_end[0]   = QTime(13,0);
    m_end[1]   = QTime(18,0);
  }
  else if ( type == DEFAULT_TWENTYFOURHOURS )
  {
    // default 24H day
    m_name     = "24H day";
    m_work     = 1.5;
    m_periods  = 1;
    m_start.resize(1);
    m_start[0] = QTime( 0,0);
    m_end.resize(1);
    m_end[0]   = QTime(23,59,59);
  }
  else if ( type == DEFAULT_SHORT )
  {
    // default short day
    m_name     = "Morning only";
    m_work     = 0.5;
    m_periods  = 1;
    m_start.resize(1);
    m_start[0] = QTime( 9,0);
    m_end.resize(1);
    m_end[0]   = QTime(13,0);
  }
  else if ( type == DEFAULT_EVENING )
  {
    // default evening shift
    m_name     = "Evening shift";
    m_work     = 0.6;
    m_periods  = 1;
    m_start.resize(1);
    m_start[0] = QTime(18,0);
    m_end.resize(1);
    m_end[0]   = QTime(22,0);
  }
  else  // DEFAULT_NONWORK
  {
    // default non-working day
    m_name    = "Non working";
    m_work    = 0.0;
    m_periods = 0;
  }
}

/****************************************** constructor ******************************************/

Day::Day( QXmlStreamReader* stream ) : Day()
{
  // create day from stream
  foreach( QXmlStreamAttribute attribute, stream->attributes() )
  {
    if ( attribute.name() == "name" )
      m_name = attribute.value().toString();

    if ( attribute.name() == "work" )
      m_work = attribute.value().toString().toFloat();
  }

  while ( !stream->atEnd() )
  {
    stream->readNext();

    // if period element append new day period
    if ( stream->isStartElement() && stream->name() == "period" )
    {
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
        if ( attribute.name() == "start" )
          m_start.append( QTime::fromString(attribute.value().toString(),"HH:mm:ss") );

        if ( attribute.name() == "end" )
          m_end.append( QTime::fromString(attribute.value().toString(),"HH:mm:ss") );
      }

      m_periods = m_start.size();
      if ( m_periods != m_end.size() )
        qWarning("Day::Day( QXmlStreamReader* ) WARNING size of period starts & ends not equal!");
    }

    // when reached end of day return
    if ( stream->isEndElement() && stream->name() == "day" ) return;
  }
}

/***************************************** saveToStream ******************************************/

void  Day::saveToStream( QXmlStreamWriter* stream )
{
  // write day data to xml stream
  stream->writeAttribute( "name", m_name );
  stream->writeAttribute( "work", QString("%1").arg(m_work) );

  for( int p=0 ; p<m_periods ; p++ )
  {
    stream->writeEmptyElement( "period" );
    stream->writeAttribute( "id", QString("%1").arg(p) );
    stream->writeAttribute( "start", m_start[p].toString(Qt::ISODate) );
    stream->writeAttribute( "end", m_end[p].toString(Qt::ISODate) );
  }
}

/****************************************** headerData *******************************************/

QVariant  Day::headerData( int section )
{
  // return section horizontal header title text
  if ( section == SECTION_NAME )  return "Name";
  if ( section == SECTION_WORK )  return "Work";
  if ( section == SECTION_PARTS ) return "Periods";

  if ( (section-SECTION_START) % 2 == 0 )
    return QString("Start %1").arg( (section-SECTION_START) / 2 + 1 );
  else
    return QString("End %1").arg( (section-SECTION_END) / 2 + 1 );
}

/********************************************* data **********************************************/

QVariant  Day::data( int column, int role )
{
  // if role is EditRole, return appropriate edit value
  if ( role == Qt::EditRole )
  {
    if ( column == SECTION_WORK )   return m_work;

    // for all other columns return the DisplayRole for EditRole
    role = Qt::DisplayRole;
  }

  // if role is DisplayRole, return appropriate display value
  if ( role == Qt::DisplayRole )
  {
    if ( column == SECTION_NAME )   return m_name;
    if ( column == SECTION_WORK )   return QString("%1").arg( m_work, 0, 'f', 2 );
    if ( column == SECTION_PARTS )  return m_periods;

    if ( column >= m_periods*2+SECTION_START ) return QVariant();

    if ( (column-SECTION_START) % 2 == 0 )
      return m_start.at(( column-SECTION_START) / 2 );
    else
      return m_end.at( (column-SECTION_END) / 2 );
  }

  // if role is TextAlignmentRole, return appropriate display alignment
  if ( role == Qt::TextAlignmentRole )
  {
    if ( column == SECTION_NAME )   return int( Qt::AlignVCenter | Qt::AlignLeft );
    return Qt::AlignCenter;
  }

  // if role is BackgroundRole, return appropriate background colour
  if ( role == Qt::BackgroundRole )
  {
    if ( column >= m_periods*2+SECTION_START ) return plan->nullCellColour();
    return QVariant();
  }

  // otherwise return an invalid QVariant
  return QVariant();
}

/******************************************** workToGo ********************************************/

float Day::workToGo( QTime time ) const
{
  // count working seconds and worked seconds
  int working = 0;
  int worked  = 0;
  for( int period=0 ; period < m_periods ; period++ )
  {
    int secs = m_start.at(period).secsTo( m_end.at(period) );

    working += secs;

    if ( time >= m_end.at(period) ) worked += secs;
    if ( time >  m_start.at(period) && time < m_end.at(period) )
      worked += m_start.at(period).secsTo( time );
  }

  if ( working == 0 ) return 0.0;
  return float( working - worked ) * m_work / working;
}

/******************************************** workDone ********************************************/

float Day::workDone( QTime time ) const
{
  // count working seconds and worked seconds
  int working = 0;
  int worked  = 0;
  for( int period=0 ; period < m_periods ; period++ )
  {
    int secs = m_start.at(period).secsTo( m_end.at(period) );

    working += secs;

    if ( time >= m_end.at(period) ) worked += secs;
    if ( time >  m_start.at(period) && time < m_end.at(period) )
      worked += m_start.at(period).secsTo( time );
  }

  if ( working == 0 ) return 0.0;
  return float( worked ) * m_work / working;
}

/******************************************** secsToGo ********************************************/

int Day::secsToGo( QTime time ) const
{
  // count remaining working seconds to end of day
  int remain = 0;
  for( int period = m_periods-1 ; period >= 0 ; period-- )
  {
    if ( time <= m_start.at(period) )
      remain += m_start.at(period).secsTo( m_end.at(period) );
    if ( time >  m_start.at(period) && time < m_end.at(period) )
      remain += time.secsTo( m_end.at(period) );
  }

  return remain;
}

/******************************************** secsDone ********************************************/

int Day::secsDone( QTime time ) const
{
  // count done working seconds from start of day
  int done = 0;
  for( int period=0 ; period < m_periods ; period++ )
  {
    if ( time >= m_end.at(period) )
      done += m_start.at(period).secsTo( m_end.at(period) );
    if ( time >  m_start.at(period) && time < m_end.at(period) )
      done += m_start.at(period).secsTo( time );
  }

  return done;
}

/******************************************** seconds ********************************************/

int  Day::seconds() const
{
  // count number of working seconds in day
  int secs = 0;
  for( int period=0 ; period < m_periods ; period++ )
    secs += m_start.at(period).secsTo( m_end.at(period) );

  return secs;
}

/******************************************** doWork *********************************************/

QTime  Day::doWork( QTime time, float work ) const
{
  // calculate number of seconds to move forward
  int secs = int( seconds() * work / m_work );

  for( int period=0 ; period < m_periods ; period++ )
  {
    // if start time is after this period, skip to next period
    if ( !time.isValid() ) time = m_start.at(period);
    else if ( time > m_end.at(period) ) continue;

    int s = time.secsTo( m_end.at(period) );

    if ( s < secs )
    {
      secs -= s;
      time  = QTime();
    }
    else
      return time.addSecs( secs );
  }

  qDebug("Day::doWork - ERROR asked to do more work than remains!!!");
  return QTime();
}

/******************************************** doSecs *********************************************/

QTime  Day::doSecs( QTime time, int secs ) const
{
  // move forward by number of working seconds
  for( int period=0 ; period < m_periods ; period++ )
  {
    // if start time is after this period, skip to next period
    if ( !time.isValid() ) time = m_start.at(period);
    else if ( time > m_end.at(period) ) continue;

    int s = time.secsTo( m_end.at(period) );

    if ( s < secs )
    {
      secs -= s;
      time  = QTime();
    }
    else
      return time.addSecs( secs );
  }

  qDebug("Day::doSecs - ERROR asked to do more work than remains!!!");
  return QTime();
}

/******************************************** stretch ********************************************/

QTime  Day::stretch( QTime now ) const
{
  // if no working times return orginal date-time
  if ( m_periods == 0 ) return now;

  // calculate in seconds now time, day start, and day end
  int nowSecs   = QTime(0,0).secsTo( now );
  int startSecs = QTime(0,0).secsTo( start() );
  int endSecs   = QTime(0,0).secsTo( end() );

  if ( nowSecs <= startSecs ) return QTime( 0, 0 );
  if ( nowSecs >= endSecs   ) return QTime( 23, 59, 59, 999 );

  // stretch time component so it uses whole 24 hours
  double scale = 86400.0 / ( endSecs - startSecs );
  return QTime(0,0).addSecs( int( scale * ( nowSecs - startSecs ) ) );
}
