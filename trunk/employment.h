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

#ifndef EMPLOYMENT_H
#define EMPLOYMENT_H

#include <QMultiHash>
#include <QDateTime>

#include "timespan.h"

class Task;
class Resource;

/*************************************************************************************************/
/************************ Contains scheduled resource employment on tasks ************************/
/*************************************************************************************************/

class Employment
{
public:
  Employment();                                           // constructor

  void       clear();                                     // clears all contents
  void       clear( const Task* );                        // clears contents related to specified task
  TimeSpan   work( const Task* );                         // return work done on specified task
  void       employ( Resource*, Task*, float,
                     QDateTime, QDateTime );              // register resource employment
  float      free( Resource*, QDateTime, QDateTime& );    // return resource free to be employed
  float      available( Resource*, QDateTime,
                        QDateTime& );                     // return resource available to be employed

  struct Employ
  {
    QDateTime     start;          // null not allowed
    QDateTime     end;            // null not allowed
    float         num;            // must be greater than zero
  };

  struct TaskEmployment
  {
    Task*          task;          // nullptr not allowed
    QList<Employ>  list;
  };

private:
  QMultiHash<Resource*,TaskEmployment>  m_employment;      // container of all employment records
};

#endif // EMPLOYMENT_H
