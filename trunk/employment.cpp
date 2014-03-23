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

#include "employment.h"
#include "plan.h"
#include "task.h"
#include "resource.h"
#include "calendar.h"

/*************************************************************************************************/
/*********************** Contains scheduled task employment for a resource ***********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Employment::Employment()
{
  // initialise private variables
  m_res = nullptr;
}

/********************************************* clear *********************************************/

void Employment::clear()
{
  // clears all contents
  m_employment.clear();
}

/********************************************* clear *********************************************/

void Employment::clear( const Task* task )
{
  // remove employment records related to specified task
  m_employment.remove( (Task*)task );
}

/********************************************* work **********************************************/

float Employment::work( const Task* task )
{

  // loop through all employment records on specified task and sum work done
  float      work = 0.0;
  Calendar*  cal  = m_res->m_calendar;
  foreach( Employ emp, m_employment.value( (Task*)task ) )
    work += cal->workBetween( emp.start, emp.end ).number() * emp.num;

  return work;
}

/******************************************** insert *********************************************/

void Employment::employ( Task* task, float quantity, QDateTime start, QDateTime end )
{
  // create employment record
  Q_ASSERT( !start.isNull() );
  Q_ASSERT( !end.isNull() );
  Q_ASSERT( start < end );
  Q_ASSERT( quantity > 0.0 );
  Employ emp;
  emp.start  = start;
  emp.end    = end;
  emp.num    = quantity;

  m_employment[ task ].append( emp );

  // TODO check if this cases over-employment, and if it does reduce other employment(s)!
}

/****************************************** assignable *******************************************/

float Employment::assignable( QDateTime now, QDateTime& change, int priority )
{
  // return resource quantity available to be allocated and when this quantity changes
  float avail = available( now, change );

  QHash<Task*,QList<Employ> >::const_iterator i = m_employment.constBegin();
  while( i != m_employment.constEnd() )
  {
    qDebug("Employment::assignable()  Task*=%p  list size=%i", i.key(), i.value().size() );

    Task* task = i.key();
    // if other task has lower priority then its employment assigments are ignored
    if ( task->priority() >= priority )
      foreach( Employ emp, i.value() )
      {
        qDebug(" --- s(%s)  e(%s)  %f", qPrintable(emp.start.toString()), qPrintable(emp.end.toString()), emp.num );

        // if employment ends before now, move on as not of interest
        if ( emp.end <= now ) continue;

        // if employement start after now but before change, make this new change and move on
        if ( emp.start > now )
        {
          if ( emp.start < change ) change = emp.start;
          continue;
        }

        // employment overlaps with now, if end before change, make this new change
        if ( emp.end < change ) change = emp.end;

        if ( task->priority() == priority )
        {
          // other task has equal priroty so equal access to resources
          qDebug(" --- equal priority %i",priority );



        }
        else
        {
          // other task has higher priority so can't touch it's resourcing
          avail -= emp.num;
        }
      }

    ++i;
  }



/*
  // free is difference between available and already allocated
  QMultiHash<Resource*, TaskEmployment>::iterator i = m_employment.find( res );
  while( i != m_employment.end() && i.key() == res )
  {
    QList<Employ>  use = i.value().list;
    qDebug("***************************** Employment::free  use list size=%i",use.size());
    // TODO !!!!  <THIS METHOD IS NOT FINISHED>

    ++i;
  }
*/

  return avail;
}

/******************************************* available *******************************************/

float Employment::available( QDateTime dt, QDateTime& change )
{
  // return resource quantity available to be allocated and when this quantity changes
  QDate  start = m_res->m_start;
  if ( start.isValid() && dt.date() < start )  // is dt before resource availability start?
  {
    change = QDateTime( start );
    return 0.0;
  }

  QDate  end = m_res->m_end;
  if ( end.isValid() ) change = QDateTime( end.addDays(1) );
  else                 change = plan->MAX_DATETIME;

  if ( dt >= change )                          // is dt after resource availability end?
  {
    change = plan->MAX_DATETIME;
    return 0.0;
  }

  return m_res->m_availability;
}
