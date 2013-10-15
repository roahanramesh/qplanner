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
#include "resource.h"
#include "plan.h"
#include "calendar.h"

/*************************************************************************************************/
/************************ Contains scheduled resource employment on tasks ************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Employment::Employment()
{
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
  QMutableHashIterator<Resource*, TaskEmployment> i( m_employment );
  while( i.hasNext() )
  {
    i.next();
    if ( task == i.value().task ) i.remove();
  }
}

/********************************************* work **********************************************/

TimeSpan Employment::work( const Task* task )
{

  // loop through all employment records to sum work done on specified task
  float work = 0.0;
  QHashIterator<Resource*, TaskEmployment> i( m_employment );
  while( i.hasNext() )
  {
    i.next();
    if ( task != i.value().task ) continue;

    foreach( Employ use, i.value().list )
    {
      Calendar*  cal = i.key()->calendar();
      work += cal->workBetween( use.start, use.end ).number() * use.num;
    }
  }

  return TimeSpan( work, TimeSpan::UNIT_DAYS );
}

/******************************************** insert *********************************************/

void Employment::employ( Resource* res, Task* task, float quantity, QDateTime start, QDateTime end )
{
  // create employment record
  Employ usage;
  usage.start  = start;
  usage.end    = end;
  usage.num    = quantity;

  // loop through resource records to see if resource already employed on specified task
  bool  notFound = true;
  QMultiHash<Resource*, TaskEmployment>::iterator i = m_employment.find( res );
  while( i != m_employment.end() && i.key() == res )
  {
    // if employment record is for specified task, add usage
    if ( i.value().task == task )
    {
      notFound = false;
      i.value().list.append( usage );
      break;
    }

    ++i;
  }

  // if no employment record found for specified task, insert new TaskEmployment
  if ( notFound )
  {
    TaskEmployment emp;
    emp.task = task;
    emp.list.append( usage );

    m_employment.insert( res, emp );
  }
}

/********************************************* free **********************************************/

float Employment::free( Resource* res, QDateTime dt, QDateTime& change )
{
  // return resource quantity free to be allocated and when this quantity changes
  float avail = available( res, dt, change );

  // free is difference between available and already allocated
  QMultiHash<Resource*, TaskEmployment>::iterator i = m_employment.find( res );
  while( i != m_employment.end() && i.key() == res )
  {
    QList<Employ>  use = i.value().list;
    qDebug("***************************** Employment::free  use list size=%i",use.size());
    // TODO !!!!  <THIS METHOD IS NOT FINISHED>

    ++i;
  }

  return avail;
}

/******************************************* available *******************************************/

float Employment::available( Resource* res, QDateTime dt, QDateTime& change )
{
  // return resource quantity available to be allocated and when this quantity changes
  QDate  start = res->m_start;
  if ( start.isValid() && dt.date() < start )  // is dt before resource availability start?
  {
    change = QDateTime( start );
    return 0.0;
  }

  change = QDateTime( res->m_end ).addDays(1);
  if ( !change.isValid() ) change = plan->MAX_DATETIME;
  if ( dt >= change )                          // is dt after resource availability end?
  {
    change = plan->MAX_DATETIME;
    return 0.0;
  }

  return res->m_availability;
}
