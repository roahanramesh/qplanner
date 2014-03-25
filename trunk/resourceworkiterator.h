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

#ifndef RESOURCEWORKITERATOR_H
#define RESOURCEWORKITERATOR_H

#include "resource.h"
#include "xdatetime.h"

/*************************************************************************************************/
/****************************** Iterator for resource work periods *******************************/
/*************************************************************************************************/

class ResourceWorkIterator
{
public:
  ResourceWorkIterator();                                              // constructor

  DateTime      start() { return m_start; }
  DateTime      end() { return m_end; }
  float         efficacy() { return m_efficacy; }
  float         available() { return m_available; }

  Resource*     resource() { return m_res;}
  void          nextWorkPeriod();
  void          previousWorkPeriod();
  void          allocate();

private:
  DateTime      m_start;            // work period start
  DateTime      m_end;              // work period end
  float         m_efficacy;         // units of work per minute
  float         m_available;        // maximum availability of resource

  Resource*     m_res;              // pointer to resource
};

#endif // RESOURCEWORKITERATOR_H
