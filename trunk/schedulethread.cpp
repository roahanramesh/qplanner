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

#include "schedulethread.h"

/*************************************************************************************************/
/*************************** Thread that creates updated plan schedule ***************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ScheduleThread::ScheduleThread( QObject* parent) : QThread( parent )
{
  // initialise variables that control flow of run()
  m_restart = false;
  m_abort   = false;
}

/****************************************** destructor *******************************************/

ScheduleThread::~ScheduleThread()
{
  // destructor can be called at any point, set abort to true to tell run() to stop
  m_mutex.lock();
  m_abort = true;
  m_condition.wakeOne();
  m_mutex.unlock();

  // wait until associated thread has finished executing (i.e. run() returns)
  wait();
}

/******************************************* schedule ********************************************/

void  ScheduleThread::schedule( ScheduleData data )
{
  // called whenever schedule needs to be updated
  QMutexLocker  locker( &m_mutex );
  m_data_in_waiting = data;

  // if thread not running, start it, otherwise restart and wake up
  if ( !isRunning() )
    start( LowPriority );
  else
  {
    m_restart = true;
    m_condition.wakeOne();
  }
}

/********************************************** run **********************************************/

void  ScheduleThread::run()
{
  forever
  {
    // process input to generate schedule result output
    m_mutex.lock();
    m_data = m_data_in_waiting;
    m_mutex.unlock();
    process();

    // if abort exit run() immediately to finish thread
    if ( m_abort ) return;

    // if not restart wait until another thread signals
    m_mutex.lock();
    if ( !m_restart ) m_condition.wait( &m_mutex );
    m_restart = false;
    m_mutex.unlock();
  }
}

/******************************************** process ********************************************/

void  ScheduleThread::process()
{
  qDebug("ScheduleThread::process()");
  // process schedule input data to create schedule output result
  ScheduleResult  result;



  // if not restart and not abort, emit schedule result
  if ( !m_restart && !m_abort ) emit scheduleUpdate( result );

  return;
}
