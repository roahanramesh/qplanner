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

#ifndef PLAN_H
#define PLAN_H

#include <QUndoStack>
#include <QString>
#include <QDateTime>

class TasksModel;
class ResourcesModel;
class CalendarsModel;
class DaysModel;

/*************************************************************************************************/
/************************** Holds the complete data model for the plan ***************************/
/*************************************************************************************************/

class Plan
{
public:
  Plan();                       // constructor
  ~Plan();                      // destructor

  TasksModel*      tasks() { return m_tasks; }                      // return tasks model pointer
  ResourcesModel*  resources() { return m_resources; }              // return resources model pointer
  CalendarsModel*  calendars() { return m_calendars; }              // return calendars model pointer
  DaysModel*       days() { return m_days; }                        // return days model pointer
  QUndoStack*      undostack() { return m_undostack; }              // return undo stack pointer
  QString          title() { return m_title; }                      // return title
  QDateTime        start() { return m_start; }                      // return start
  QString          datetime_format() { return m_datetime_format; }  // return datetime format
  QString          filename() {return m_filename; }                 // return filename
  QString          file_location() { return m_file_location; }      // return file location
  QString          saved_by() { return m_saved_by; }                // return saved by username
  QDateTime        saved_when() { return m_saved_when; }            // return saved datetime

  void             setTitle( QString t ) { m_title = t; }           // set title
  void             setStart( QDateTime dt ) { m_start = dt; }       // set start
  void             setDatetime_format( QString f )
                                        { m_datetime_format = f; }  // set datetime format

private:
  TasksModel*      m_tasks;             // model of plan tasks
  ResourcesModel*  m_resources;         // model of plan resources
  CalendarsModel*  m_calendars;         // model of plan calendars
  DaysModel*       m_days;              // model of plan day types
  QUndoStack*      m_undostack;         // undo stack of plan editing
  QString          m_title;             // plan title as set in properties
  QDateTime        m_start;             // plan start as set in properties
  QString          m_datetime_format;   // plan datetime format as set in properties
  QString          m_filename;          // filename when last opened/saved
  QString          m_file_location;     // file location when last opened/saved
  QString          m_saved_by;          // username of who last saved
  QDateTime        m_saved_when;        // datetime when last saved
};

#endif // PLAN_H
