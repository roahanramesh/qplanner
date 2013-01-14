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

#ifndef SCHEDULETHREAD_H
#define SCHEDULETHREAD_H

#include "scheduledata.h"
#include "scheduleresult.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

/*************************************************************************************************/
/*************************** Thread that creates updated plan schedule ***************************/
/*************************************************************************************************/

class ScheduleThread : public QThread
{
  Q_OBJECT
public:
  ScheduleThread( QObject *parent = 0 );        // constructor
  ~ScheduleThread();                            // destructor

  void  schedule( ScheduleData );               // triggers schedule processing with input data

signals:
  void  scheduleUpdate( ScheduleResult );       // signal with schedule processing output result

protected:
  void  run();                                  // starting point of thread after start() called

private:
  ScheduleData      m_data_in_waiting;          // copy of input data ready to be processed
  ScheduleData      m_data;                     // copy of input data currently being processed
  void              process();                  // process input data and emit result when finished

  QMutex            m_mutex;                    // the mutex protects the other data member
  QWaitCondition    m_condition;                // for synchronizing threads
  bool              m_restart;                  // if processing should restart with new input
  bool              m_abort;                    // if processing should be aborted and thread finish
};

#endif // SCHEDULETHREAD_H
