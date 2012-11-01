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

#ifndef PREDECESSORS_H
#define PREDECESSORS_H

#include <QString>
#include <QList>

#include "timespan.h"

class Task;

/*************************************************************************************************/
/********************** Task predecessors shows dependencies on other tasks **********************/
/*************************************************************************************************/

class Predecessors
{
public:
  Predecessors();

  QString     toString() const;                      // return string for display in tasks view

  enum pred_type
  {
    TYPE_FINISH_START  = 0,     // Finish (other task) to start (this start)
    TYPE_START_START   = 1,     // Start to start
    TYPE_START_FINISH  = 2,     // Start to finish
    TYPE_FINISH_FINSIH = 3,     // Finish to finish
    TYPE_DEFAULT       = 0
  };

  typedef struct
  {
    Task*     task;
    uchar     type;
    TimeSpan  lag;
  } Predecessor;

private:
  QList<Predecessor>    m_preds;      // list of task predecessors

};

#endif // PREDECESSORS_H
