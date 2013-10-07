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

#ifndef RESOURCESUSAGE_H
#define RESOURCESUSAGE_H

#include <QMultiHash>
#include <QDateTime>

class Task;
class Resource;

/*************************************************************************************************/
/******************************* Contains resource usage on Tasks ********************************/
/*************************************************************************************************/

class ResourcesUsage
{
public:
  ResourcesUsage();                                       // constructor

  void       clear();                                     // clears all contents
  void       clear( Task* );                              // clears contents related to specified task
  void       insert( Resource*, Task*, QDateTime,
                     QDateTime, float );                  // insert usage record
  float      available( Resource*, QDateTime,
                        QDateTime& );                     // returns quantity and date-time quantity changes

  struct Usage
  {
    QDateTime     start;          // null not allowed
    QDateTime     end;            // null not allowed
    float         quantity;       // must be greater than zero
  };

  struct TaskUsage
  {
    Task*         task;           // nullptr not allowed
    QList<Usage>  list;
  };

private:
  QMultiHash<Resource*,TaskUsage>  m_usage;      // container of all usage records
};

#endif // RESOURCESUSAGE_H
