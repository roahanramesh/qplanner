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

#ifndef TASK_SCHEDULE_H
#define TASK_SCHEDULE_H

#include "task.h"
#include "plan.h"
#include "calendar.h"

/*************************************************************************************************/
/**************************** Scheduling methods for single plan task ****************************/
/*************************************************************************************************/

/******************************************* schedule ********************************************/

void  Task::schedule()
{
  // schedule individual task

  switch ( m_type )
  {
    case TYPE_ASAP_FDUR:
      schedule_ASAP_FDUR();
      return;
    case TYPE_ASAP_FWORK:
      qDebug("Task::schedule() - don't know how to handle TYPE_ASAP_FWORK %i %p",plan->index(this),this);
      return;
    case TYPE_FIXED_PERIOD:
      qDebug("Task::schedule() - don't know how to handle TYPE_FIXED_PERIOD %i %p",plan->index(this),this);
      return;
    case TYPE_SON_FDUR:
      qDebug("Task::schedule() - don't know how to handle TYPE_SON_FDUR %i %p",plan->index(this),this);
      return;
    case TYPE_SON_FWORK:
      qDebug("Task::schedule() - don't know how to handle TYPE_SON_FWORK %i %p",plan->index(this),this);
      return;
    default:
      qDebug("Task::schedule() - unknown type(%i) %i %p",m_type,plan->index(this),this);
  }

  qDebug("Task::schedule() - UNHANDLED !!! %i %p",plan->index(this),this);
}

/************************************** schedule_ASAP_FDUR ***************************************/

void  Task::schedule_ASAP_FDUR()
{
  // schedule ASAP fixed duration

  // get start from predecessors
  m_start = plan->calendar()->workUp( m_predecessors.start() );

  // TRY TO DETERMINE END ???
  QDateTime end = plan->calendar()->addTimeSpan( m_start, m_duration );
  end = plan->calendar()->workDown( end );
  if ( end < m_start ) m_end = m_start;
  else                 m_end = end;

  if ( m_summary ) m_gantt.setSummary( m_start, m_end );
  else             m_gantt.setTask( m_start, m_end );


  qDebug("Task::schedule_ASAP_FDUR() UNFINISHED !!! %i %p",plan->index(this),this);
}

#endif // TASK_SCHEDULE_H
