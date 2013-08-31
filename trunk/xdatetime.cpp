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

#include "xdatetime.h"

/*************************************************************************************************/
/**************************** XDateTime provides an enhanced QDateTime ***************************/
/*************************************************************************************************/

/****************************************** constuctor *******************************************/

XDateTime::XDateTime() : QDateTime()
{
}

/****************************************** constuctor *******************************************/

XDateTime::XDateTime( QDateTime dt ) : QDateTime( dt )
{
}

/****************************************** truncInterval ****************************************/

QDateTime XDateTime::truncInterval( int interval )
{
  // calc truncated date-time for interval
  switch( interval )
  {
    case INTERVAL_YEAR:
      return QDateTime( QDate(date().year(),1,1) );
    case INTERVAL_HALFYEAR:
      if ( date().month() > 6 ) return QDateTime( QDate(date().year(),7,1) );
      return QDateTime( QDate(date().year(),1,1) );
    case INTERVAL_QUARTERYEAR:
      if ( date().month() > 9 ) return QDateTime( QDate(date().year(),10,1) );
      if ( date().month() > 6 ) return QDateTime( QDate(date().year(),7,1) );
      if ( date().month() > 3 ) return QDateTime( QDate(date().year(),4,1) );
      return QDateTime( QDate(date().year(),1,1) );
    case INTERVAL_MONTH:
      return QDateTime( QDate(date().year(), date().month(), 1) );
    case INTERVAL_WEEK:
      return QDateTime( date().addDays( 1-date().dayOfWeek() ) );
    case INTERVAL_DAY:
      return QDateTime( date() );
    default:
      qDebug("XDateTime::truncInterval - UNKNOWN interval %i", interval );
      return QDateTime();
  }
}

/****************************************** nextInterval *****************************************/

QDateTime XDateTime::nextInterval( int interval )
{
  // calc next date-time for interval
  switch( interval )
  {
    case INTERVAL_YEAR:
      return addYears(1);
    case INTERVAL_HALFYEAR:
      return addMonths(6);
    case INTERVAL_QUARTERYEAR:
      return addMonths(3);
    case INTERVAL_MONTH:
      return addMonths(1);
    case INTERVAL_WEEK:
      return addDays(7);
    case INTERVAL_DAY:
      return addDays(1);
    default:
      qDebug("XDateTime::nextInterval - UNKNOWN interval %i", interval );
      return QDateTime();
  }
}

/********************************************* toText ********************************************/

QString XDateTime::toText()
{
  // convert date-time to text but without ISO format limitation of year between 0000 and 9999
  // ISO format example is "2013-08-27T09:00:00"

  if ( isValid() )
  {
    int year, month, day;
    date().getDate( &year, &month, &day );

    return QString("%1-%2-%3T%4").arg(year).arg(month).arg(day).arg(time().toString(Qt::ISODate));
  }

  // not valid
  return "INVALID";
}

/******************************************** fromText *******************************************/

QDateTime XDateTime::fromText( QString text )
{
  // convert ISO format like text to date-time but without limitation of year between 0000 and 9999
  // ISO format example is "2013-08-27T09:00:00"

  QString date = text.left( text.indexOf('T') );
  QString time = text.mid(  text.indexOf('T')+1 );

  QString year  = date.left( date.indexOf('-', 1) );
  QString month = date.left( date.indexOf('-', year.length()+1) );
  month = month.mid( month.indexOf('-',1)+1 );
  QString day   = date.mid( year.length()+month.length()+2 );

  bool ok;
  int y = year.toInt( &ok );
  int m = month.toInt( &ok );
  int d = day.toInt( &ok );

  return QDateTime( QDate(y,m,d), QTime::fromString(time,"hh:mm:ss") );
}

/********************************************* toLabel *******************************************/

QString XDateTime::toLabel( QString format )
{
  // construct label based on format
  QString label = toString( format );

  if ( label.contains( "QN" ) )
  {
    int quarter = (date().month() - 1) / 3 + 1;
    label.replace( "QN", QString( "Q%1" ).arg( quarter ) );
  }

  if ( label.contains( "HN" ) )
  {
    int half = (date().month() - 1) / 6 + 1;
    label.replace( "HN", QString( "H%1" ).arg( half ) );
  }

  return label;
}
