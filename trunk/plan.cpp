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

#include "plan.h"
#include "tasksmodel.h"
#include "resourcesmodel.h"
#include "calendarsmodel.h"
#include "daysmodel.h"

/*************************************************************************************************/
/************************** Holds the complete data model for the plan ***************************/
/*************************************************************************************************/

Task*      Plan::task( int n ) { return m_tasks->task(n); }               // return the n'th task
Resource*  Plan::resource( int n ) { return m_resources->resource(n); }   // return the n'th resource
Calendar*  Plan::calendar( int n ) { return m_calendars->calendar(n); }   // return the n'th calendar
Day*       Plan::day( int n ) { return m_days->day(n); }                  // return the n'th day type

/****************************************** constructor ******************************************/

Plan::Plan()
{
  // initialise private variables
  m_days      = new DaysModel();
  m_calendars = new CalendarsModel( m_days );  // need to pass m_days as plan pointer isn't ready to use yet
  m_resources = new ResourcesModel();
  m_tasks     = new TasksModel();
  m_undostack = new QUndoStack();
  m_start     = QDateTime::currentDateTime();
  m_datetime_format = "ddd dd/MM/yy";
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
