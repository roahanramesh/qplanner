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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QString>
#include <QDate>

class Calendar;

/*************************************************************************************************/
/************************************* Single plan resource **************************************/
/*************************************************************************************************/

class Resource
{
public:
  Resource();                                                   // constructor

  static QVariant   headerData( int );                          // return column header data
  QVariant          data( int, int );                           // return data for column & role

  QString           initials() const { return m_initials; }     // return initials

  enum sections                            // sections to be displayed by view
  {
    SECTION_MINIMUM  = 0,
    SECTION_INITIALS = 0,
    SECTION_NAME     = 1,
    SECTION_ORG      = 2,
    SECTION_GROUP    = 3,
    SECTION_START    = 4,
    SECTION_END      = 5,
    SECTION_AVAIL    = 6,
    SECTION_ABILITY  = 7,
    SECTION_COST     = 8,
    SECTION_CALENDAR = 9,
    SECTION_COMMENT  = 10,
    SECTION_MAXIMUM  = 10
  };

private:
  QString            m_initials;           // should be unique across all resources in model
  QString            m_name;               // free text
  QString            m_org;                // free text
  QString            m_group;              // free text
  QDate              m_start;              // date availability starts
  QDate              m_end;                // date availability end
  float              m_availability;       // number available
  float              m_ability;            // ability factor - typical 1.0
  float              m_cost;               // cost TODO
  Calendar*          m_calendar;           // base calendar for resource
  QString            m_comment;            // free text
};

#endif // RESOURCE_H
