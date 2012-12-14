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
#include <QColor>

class TasksModel;
class ResourcesModel;
class CalendarsModel;
class DaysModel;

class Task;
class Resource;
class Calendar;
class Day;

/*************************************************************************************************/
/************************** Holds the complete data model for the plan ***************************/
/*************************************************************************************************/

class Plan : public QObject
{
  Q_OBJECT
public:
  Plan();                       // constructor
  ~Plan();                      // destructor

  void             initialise();                                    // create initial default contents

  TasksModel*      tasks() { return m_tasks; }                      // return tasks model pointer
  ResourcesModel*  resources() { return m_resources; }              // return resources model pointer
  CalendarsModel*  calendars() { return m_calendars; }              // return calendars model pointer
  DaysModel*       days() { return m_days; }                        // return days model pointer
  QUndoStack*      undostack() { return m_undostack; }              // return undo stack pointer

  Task*            task( int );                                     // return the n'th task
  Resource*        resource( int );                                 // return the n'th resource
  Calendar*        calendar( int );                                 // return the n'th calendar
  Day*             day( int );                                      // return the n'th day type

  int              index( Task* );                                  // return index of task
  int              index( Resource* );                              // return index of resource
  int              index( Calendar* );                              // return index of calendar
  int              index( Day* );                                   // return index of day

  int              numTasks();                                      // return number of tasks in plan
  int              numResources();                                  // return number of resources in plan
  int              numCalendars();                                  // return number of calendars in plan
  int              numDays();                                       // return number of day types in plan

  QString          title() { return m_title; }                      // return title of plan
  QDateTime        start() { return m_start; }                      // return start of plan
  QDateTime        end();                                           // return end of plan
  Calendar*        calendar() { return m_calendar; }                // return default calendar
  QString          datetimeFormat() { return m_datetime_format; }   // return datetime format
  QString          filename() {return m_filename; }                 // return filename
  QString          fileLocation() { return m_file_location; }       // return file location
  QString          savedBy() { return m_saved_by; }                 // return saved by username
  QDateTime        savedWhen() { return m_saved_when; }             // return saved datetime
  QString          notes() { return m_notes; }                      // return notes text

  void             setTitle( QString t ) { m_title = t; }           // set title
  void             setStart( QDateTime dt ) { m_start = dt; }       // set start
  void             setDatetimeFormat( QString f )
                     { m_datetime_format = f; }                     // set datetime format
  void             setCalendar( Calendar* c ) { m_calendar = c; }   // set plan default calendar
  void             setCalendar( int c )
                     { m_calendar = calendar(c); }                  // set plan default calendar
  void             setNotes( QString n ) { m_notes = n; }           // set notes text

  void             emitPropertiesUpdated()
                     { emit signalPropertiesUpdated(); }            // emit signalPropertiesUpdated

  QColor           nullCellColour() { return QColor( "#F0F0F0" ); } // colour for null table cell

signals:
  void  signalPropertiesUpdated();      // signal to say plan properties updated

private:
  TasksModel*      m_tasks;             // model of plan tasks
  ResourcesModel*  m_resources;         // model of plan resources
  CalendarsModel*  m_calendars;         // model of plan calendars
  DaysModel*       m_days;              // model of plan day types

  QUndoStack*      m_undostack;         // undo stack of plan editing

  QString          m_title;             // plan title as set in properties
  QDateTime        m_start;             // plan start as set in properties
  Calendar*        m_calendar;          // plan default calendar
  QString          m_datetime_format;   // plan datetime format as set in properties
  QString          m_filename;          // filename when last opened/saved
  QString          m_file_location;     // file location when last opened/saved
  QString          m_saved_by;          // username of who last saved
  QDateTime        m_saved_when;        // datetime when last saved
  QString          m_notes;             // plan notes as in properties
};

extern Plan*  plan;


#endif // PLAN_H
