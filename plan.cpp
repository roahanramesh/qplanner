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

#include "plan.h"
#include "tasksmodel.h"
#include "resourcesmodel.h"
#include "calendarsmodel.h"
#include "daysmodel.h"
#include "calendar.h"
#include "task.h"

#include <QUndoStack>
#include <QXmlStreamWriter>
#include <QFileInfo>

/*************************************************************************************************/
/************************** Holds the complete data model for the plan ***************************/
/*************************************************************************************************/

Task*      Plan::task( int n ) { return m_tasks->task(n); }               // return the n'th task pointer
Resource*  Plan::resource( int n ) { return m_resources->resource(n); }   // return the n'th resource pointer
Calendar*  Plan::calendar( int n ) { return m_calendars->calendar(n); }   // return the n'th calendar pointer
Day*       Plan::day( int n ) { return m_days->day(n); }                  // return the n'th day-type pointer

int        Plan::index( Task* t ) { return m_tasks->index(t); }           // return internal index of task
int        Plan::index( Resource* r ) { return m_resources->index(r); }   // return internal index of resource
int        Plan::index( Calendar* c ) { return m_calendars->index(c); }   // return internal index of calendar
int        Plan::index( Day* d ) { return m_days->index(d); }             // return internal index of day

int        Plan::numTasks() { return m_tasks->number(); }                 // return number of tasks in plan
int        Plan::numResources() { return m_resources->number(); }         // return number of resources in plan
int        Plan::numCalendars() { return m_calendars->number(); }         // return number of calendars in plan
int        Plan::numDays() { return m_days->number(); }                   // return number of day types in plan

QDateTime  Plan::beginning() { return m_tasks->planBeginning(); }         // return start of earliest starting task
QDateTime  Plan::end() { return m_tasks->planEnd(); }                     // return finish of latest finishing task

void       Plan::schedule() { m_tasks->schedule(); }                      // schedule the plan tasks

const QDate      Plan::MIN_DATE = QDate( -99999999, 1, 1 );               // const min date allowed in plan
const QDate      Plan::MAX_DATE = QDate(  99999999, 1, 1 );               // const max date allowed in plan
const QDateTime  Plan::MIN_DATETIME = QDateTime( Plan::MIN_DATE );              // const min date-time allowed in plan
const QDateTime  Plan::MAX_DATETIME = QDateTime( Plan::MAX_DATE );              // const max date-time allowed in plan

/****************************************** constructor ******************************************/

Plan::Plan()
{
  // create blank models and set private variables
  m_days       = new DaysModel();
  m_calendars  = new CalendarsModel();
  m_resources  = new ResourcesModel();
  m_tasks      = new TasksModel();
  m_undostack  = new QUndoStack();

  m_datetime_format = "ddd dd/MM/yyyy hh:mm:ss";
  m_calendar        = nullptr;
  stretchTasks      = true;
}

/****************************************** destructor *******************************************/

Plan::~Plan()
{
  // delete models and undostack
  delete m_tasks;
  delete m_resources;
  delete m_calendars;
  delete m_days;
  delete m_undostack;
}

/****************************************** initialise *******************************************/

void  Plan::initialise()
{
  // initialise the models
  m_days->initialise();
  m_calendars->initialise();

  m_calendar = calendar( Calendar::DEFAULT_CALENDAR );
  m_start    = m_calendar->workUp( QDateTime( QDate::currentDate(), QTime(0,0,0) ) );

  m_resources->initialise();
  m_tasks->initialise();
}

/********************************************* isOK **********************************************/

bool  Plan::isOK()
{
  // return if plan appears ok
  if ( numDays()          > 0  &&
       numCalendars()     > 0  &&
       index( calendar()) >= 0 )
    return true;

  return false;
}

/***************************************** setFileInfo *******************************************/

void  Plan::setFileInfo( QString filename, QDateTime when, QString who )
{
  // set plan file, when, who properties
  QFileInfo  file = filename;
  m_filename      = file.fileName();
  m_file_location = file.path();
  m_saved_by      = who;
  m_saved_when    = when;
}

/***************************************** saveToStream ******************************************/

void  Plan::saveToStream( QXmlStreamWriter* stream )
{
  // write plan data to xml stream
  m_days->saveToStream( stream );
  m_calendars->saveToStream( stream );
  m_resources->saveToStream( stream );
  m_tasks->saveToStream( stream );

  stream->writeStartElement( "plan-data" );
  stream->writeAttribute( "title", m_title );
  stream->writeAttribute( "start", m_start.toString(Qt::ISODate) );
  stream->writeAttribute( "calendar", QString("%1").arg(plan->index(m_calendar)) );
  stream->writeAttribute( "datetime-format", m_datetime_format );
  stream->writeAttribute( "notes", m_notes );
  stream->writeEndElement();
}

/**************************************** loadFromStream *****************************************/

void  Plan::loadFromStream( QXmlStreamReader* stream, QString file )
{
  // read qplanner file attributes
  foreach( QXmlStreamAttribute attribute, stream->attributes() )
  {
    if ( attribute.name() == "user" )
      m_saved_by = attribute.value().toString();

    if ( attribute.name() == "when" )
      m_saved_when = QDateTime::fromString( attribute.value().toString(), "yyyy-MM-ddTHH:mm:ss" );
  }
  setFileInfo( file, m_saved_when, m_saved_by );

  // load plan data from xml stream
  while ( !stream->atEnd() )
  {
    stream->readNext();
    if ( stream->isStartElement() )
    {
      if ( stream->name() == "days-data"      ) m_days->loadFromStream( stream );
      if ( stream->name() == "calendars-data" ) m_calendars->loadFromStream( stream );
      if ( stream->name() == "resources-data" ) m_resources->loadFromStream( stream );
      if ( stream->name() == "tasks-data"     ) m_tasks->loadFromStream( stream );

      if ( stream->name() == "plan-data" )
        foreach( QXmlStreamAttribute attribute, stream->attributes() )
        {
          if ( attribute.name() == "title" )
            m_title = attribute.value().toString();

          if ( attribute.name() == "start" )
            m_start = QDateTime::fromString( attribute.value().toString(), "yyyy-MM-ddTHH:mm:ss" );

          if ( attribute.name() == "calendar" )
            m_calendar = calendar( attribute.value().toString().toInt() );

          if ( attribute.name() == "datetime-format" )
            m_datetime_format = attribute.value().toString();

          if ( attribute.name() == "notes" )
            m_notes = attribute.value().toString();
        }

      // if stream name is display-data then have finished loading plan
      if ( stream->name() == "display-data" ) return;
    }
  }
}

/********************************************* stretch *******************************************/

QDateTime  Plan::stretch( QDateTime dt )
{
  // return date-time stretched across full 24 hrs if plan stretchTasks flag is true
  if ( stretchTasks )
  {
    QTime time = m_calendar->getDay( dt.date() )->stretch( dt.time() );
    return QDateTime( dt.date(), time );
  }

  // plan stretchTasks flag not true, so return original date-time
  return dt;
}
