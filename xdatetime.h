/***************************************************************************
 *   Copyright (C) 2014 by Richard Crook                                   *
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

#ifndef XDATETIME_H
#define XDATETIME_H

#include <QDateTime>
#include <QString>

typedef  quint32 DateTime;   // simple date-time between 1AD and ~8000AD with one minute resolution

/*************************************************************************************************/
/**************************** XDateTime provides an enhanced QDateTime ***************************/
/*************************************************************************************************/

class XDateTime : public QDateTime
{
public:
  XDateTime();                         // constructor
  XDateTime( DateTime );               // constructor
  XDateTime( QDateTime );              // constructor

  enum Interval
  {
    INTERVAL_YEAR,                     // Interval is one year
    INTERVAL_HALFYEAR,                 // Interval is half a year
    INTERVAL_QUARTERYEAR,              // Interval is quarter year
    INTERVAL_MONTH,                    // Interval is one month
    INTERVAL_WEEK,                     // Interval is one week
    INTERVAL_DAY                       // Interval is one day
  };

  QDateTime         truncInterval( int );         // return truncated date-time for interval
  QDateTime         nextInterval( int );          // return next date-time for interval
  QString           toLabel( QString );           // return label based on supplied format
  QString           toText();                     // return date-time as text
  static QDateTime  fromText( QString );          // return text as date-time

  static const QDateTime  DT_ANCHOR;              // 1-Jan-0001 AD anchor for typedef DataTime
  static const DateTime   NULL_DATETIME;          // null / invalid DateTime (-1 or 0xFFFFFFFF)
};

#endif // XDATETIME_H
