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
class ResourcesUsage;

class Task;
class Resource;
class Calendar;
class Day;

class QXmlStreamWriter;
class QXmlStreamReader;

/*************************************************************************************************/
/************************** Holds the complete data model for the plan ***************************/
/*************************************************************************************************/

class Plan : public QObject
{
  Q_OBJECT
public:
  Plan();                       // constructor
  ~Plan();                      // destructor

  void             initialise();                                    // create initial plan default contents
  bool             isOK();                                          // return if plan appears valid
  void             setFileInfo( QString, QDateTime, QString );      // set plan file, when, who properties
  void             saveToStream( QXmlStreamWriter* );               // write plan data to xml stream
  void             loadFromStream( QXmlStreamReader*, QString );    // load plan data from xml stream
  void             schedule();                                      // schedule the plan tasks

  TasksModel*      tasks() { return m_tasks; }                      // return tasks model pointer
  ResourcesModel*  resources() { return m_resources; }              // return resources model pointer
  CalendarsModel*  calendars() { return m_calendars; }              // return calendars model pointer
  DaysModel*       days() { return m_days; }                        // return days model pointer
  QUndoStack*      undostack() { return m_undostack; }              // return undo stack pointer

  Task*            task( int );                                     // return the n'th task pointer
  Resource*        resource( int );                                 // return the n'th resource pointer
  Calendar*        calendar( int );                                 // return the n'th calendar pointer
  Day*             day( int );                                      // return the n'th day type pointer

  int              index( Task* );                                  // return internal index of task
  int              index( Resource* );                              // return internal index of resource
  int              index( Calendar* );                              // return internal index of calendar
  int              index( Day* );                                   // return internal index of day

  int              numTasks();                                      // return number of tasks in plan
  int              numResources();                                  // return number of resources in plan
  int              numCalendars();                                  // return number of calendars in plan
  int              numDays();                                       // return number of day types in plan

  QString          title() { return m_title; }                      // return title of plan
  QDateTime        start() { return m_start; }                      // return nominal start of plan (T0)
  QDateTime        beginning();                                     // return start of earliest starting task
  QDateTime        end();                                           // return finish of latest finishing task
  Calendar*        calendar() { return m_calendar; }                // return default calendar pointer
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

  bool             stretchTasks;                                    // flag if gantt task bars stretched to use full 24h day
  QDateTime        stretch( QDateTime dt );                         // return date-time stretched if necessary

  QColor           nullCellColour() { return QColor( "#F0F0F0" ); } // return colour for null table cell

  static const QDate        MIN_DATE;                               // const min date allowed in plan
  static const QDate        MAX_DATE;                               // const max date allowed in plan
  static const QDateTime    MIN_DATETIME;                           // const min date-time allowed in plan
  static const QDateTime    MAX_DATETIME;                           // const max date-time allowed in plan

signals:
  void  signalPropertiesUpdated();      // signal to say plan properties updated

private:
  TasksModel*      m_tasks;             // model of plan tasks
  ResourcesModel*  m_resources;         // model of plan resources
  CalendarsModel*  m_calendars;         // model of plan calendars
  DaysModel*       m_days;              // model of plan day types

  QUndoStack*      m_undostack;         // undo stack of plan editing
  ResourcesUsage*  m_resUsage;          // contains resource usage from scheduling

  QString          m_title;             // plan title as set in properties
  QDateTime        m_start;             // plan start as set in properties
  Calendar*        m_calendar;          // plan default calendar pointer
  QString          m_datetime_format;   // plan datetime format as set in properties
  QString          m_filename;          // filename when last opened/saved
  QString          m_file_location;     // file location when last opened/saved
  QString          m_saved_by;          // username of who last saved
  QDateTime        m_saved_when;        // datetime when last saved
  QString          m_notes;             // plan notes as in properties
};

extern Plan*        plan;

#endif // PLAN_H
