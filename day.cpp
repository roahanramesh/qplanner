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

#include "day.h"

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

Day::Day( int type )
{
  // create template day
  if ( type == STANDARDWORK )
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
  else if ( type == TWENTYFOURHOURS )
  {
    // default 24H day
    m_name     = "24H day";
    m_work     = 1.5;
    m_periods  = 1;
    m_start.resize(1);
    m_start[0] = QTime( 0,0);
    m_end.resize(1);
    m_end[0]   = QTime(23,59,59,999);
  }
  else if ( type == SHORT )
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
  else if ( type == EVENING )
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
  else  // NONWORK
  {
    // default non-working day
    m_name    = "Non working";
    m_work    = 0.0;
    m_periods = 0;
  }
}

/****************************************** headerData *******************************************/

QVariant  Day::headerData( int section )
{
  // TODO
  return QVariant();
}
