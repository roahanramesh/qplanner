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

#ifndef EMPLOYMENT_H
#define EMPLOYMENT_H

#include <QHash>
#include <QDateTime>

class Task;
class Resource;

/*************************************************************************************************/
/*********************** Contains scheduled task employment for a resource ***********************/
/*************************************************************************************************/

class Employment
{
public:
  Employment();                                             // constructor

  void       setResource( Resource* r ) { m_res = r; }      // set resource internal variable
  void       clear();                                       // clears all contents
  void       clear( const Task* );                          // clears contents related to specified task
  float      work( const Task* );                           // return work done on specified task in days
  void       employ( Task*, float, QDateTime, QDateTime );  // register resource employment
  float      assignable( QDateTime, QDateTime&, int );      // return resource free to be employed
  float      available( QDateTime, QDateTime& );            // return resource available to be employed

  struct Employ
  {
    QDateTime     start;          // null not allowed, and must be less than end
    QDateTime     end;            // null not allowed, and must be greater than start
    float         num;            // must be greater than zero
  };

private:
  QHash<Task*,QList<Employ> >  m_employment;                // container of all employment records
  Resource*                    m_res;                       // pointer to resource
};

#endif // EMPLOYMENT_H
