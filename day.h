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

  enum DefaultDayTypes
  {
    NONWORK         = 0,
    STANDARDWORK    = 1,
    SHORT           = 2,
    EVENING         = 3,
    TWENTYFOURHOURS = 4
  };

  static QVariant   headerData( int );          // TODO

private:
  QString          m_name;              // name of day type
  double           m_work;              // equivalent days worked (typically 1.0 or 0.0)
  int              m_periods;           // number of work periods within the day
  QVector<QTime>   m_start;             // vector of work period start times
  QVector<QTime>   m_end;               // vector of work period end times
};

#endif // DAY_H
