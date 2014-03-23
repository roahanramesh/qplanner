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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QString>
#include <QDate>

#include "employment.h"

class Calendar;
class QXmlStreamWriter;
class QXmlStreamReader;

/*************************************************************************************************/
/************************************* Single plan resource **************************************/
/*************************************************************************************************/

class Resource
{
public:
  Resource();                                                        // constructor (normal)
  Resource( bool );                                                  // constructor (unassigned)
  Resource( QXmlStreamReader* );                                     // constructor

  void              saveToStream( QXmlStreamWriter* );               // write resource data to xml stream
  static QVariant   headerData( int );                               // return column header data
  QVariant          data( int, int );                                // return data for column & role
  bool              setData( int, int, const QVariant& );            // attempt to set value via undostack
  void              setDataDirect( int, const QVariant& );           // set value directly
  bool              isNull() const { return m_initials.isNull(); }   // is the task null (blank)
  QString           initials() const { return m_initials; }          // return initials
  Calendar*         calendar() const { return m_calendar; }          // return resource calendar
  QList<QString>    assignable() const;                              // return assignable names
  bool              hasTag( QString ) const;                         // return true if tag matches
  void              clearEmployment( Task* );                        // remove employment on given task
  float             assignable( QDateTime, QDateTime&, int );        // return quantity and change date-time
  void              employ( Task*, float, QDateTime, QDateTime );    // register employment on given task
  float             work( const Task* );                             // return work done on task in days

  enum sections                            // sections to be displayed by view
  {
    SECTION_MINIMUM  = 0,
    SECTION_INITIALS = 0,
    SECTION_NAME     = 1,
    SECTION_ORG      = 2,
    SECTION_GROUP    = 3,
    SECTION_ROLE     = 4,
    SECTION_ALIAS    = 5,
    SECTION_START    = 6,
    SECTION_END      = 7,
    SECTION_AVAIL    = 8,
    SECTION_ABILITY  = 9,
    SECTION_COST     = 10,
    SECTION_CALENDAR = 11,
    SECTION_COMMENT  = 12,
    SECTION_MAXIMUM  = 12
  };

  friend class Employment;

private:
  QString            m_initials;           // should be unique across all resources in model
  QString            m_name;               // free text
  QString            m_org;                // free text
  QString            m_group;              // free text
  QString            m_role;               // free text
  QString            m_alias;              // free text
  QDate              m_start;              // date availability starts (at 00:00:00)
  QDate              m_end;                // date availability end (at 23:59:59)
  float              m_availability;       // number available
  float              m_ability;            // ability factor - typical 1.0
  float              m_cost;               // cost TODO
  Calendar*          m_calendar;           // calendar for resource
  QString            m_comment;            // free text
  Employment         m_employment;         // contains resource employment from scheduling
};

#endif // RESOURCE_H
