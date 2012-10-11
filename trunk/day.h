/***************************************************************************
 *   Copyright (C) 2012 by Richard Crook                                   *
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

#ifndef DAY_H
#define DAY_H

#include <QVariant>
#include <QVector>
#include <QTime>

/*************************************************************************************************/
/**************************** Single day type used in plan calendars *****************************/
/*************************************************************************************************/

class Day
{
public:
  Day();                                        // constructor
  Day( int );                                   // constructor

  static QVariant   headerData( int );          // TODO

  QString    name() { return m_name; }                 // return day name
  qreal      work() { return m_work; }                 // return work days equivalent
  quint8     periods() { return m_periods; }           // return number of work periods
  QTime      start( int n ) { return m_start.at(n); }  // return work period start
  QTime      end( int n ) { return m_end.at(n); }      // return work period end

  enum DefaultDayTypes
  {
    DEFAULT_NONWORK         = 0,
    DEFAULT_STANDARDWORK    = 1,
    DEFAULT_SHORT           = 2,
    DEFAULT_EVENING         = 3,
    DEFAULT_TWENTYFOURHOURS = 4,
    DEFAULT_MAX             = 4
  };

  enum Sections
  {
    SECTION_NAME  = 0,
    SECTION_WORK  = 1,
    SECTION_PARTS = 2,
    SECTION_START = 3,
    SECTION_END   = 4
  };

private:
  QString          m_name;              // name of day type
  qreal            m_work;              // equivalent days worked (typically 1.0 or 0.0)
  quint8           m_periods;           // number of work periods within the day
  QVector<QTime>   m_start;             // vector of work period start times
  QVector<QTime>   m_end;               // vector of work period end times
};

#endif // DAY_H
