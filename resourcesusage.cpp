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

#include "resourcesusage.h"

/*************************************************************************************************/
/******************************* Contains resource usage on Tasks ********************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ResourcesUsage::ResourcesUsage()
{
  qDebug("ResourcesUsage::ResourcesUsage() - constructor");
}

/********************************************* clear *********************************************/

void ResourcesUsage::clear()
{
  // clears all contents
  m_usage.clear();
}

/********************************************* clear *********************************************/

void ResourcesUsage::clear( Task* task )
{
  // clears contents related to specified task
  Q_UNUSED( task )
  qDebug("ResourcesUsage::clear(Task %p)",task);
}

/******************************************** insert *********************************************/

void ResourcesUsage::insert( Resource* res, Task* task, QDateTime start, QDateTime end, float quantity )
{
  // insert usage record
  Q_UNUSED( res )
  Q_UNUSED( task )
  Q_UNUSED( start )
  Q_UNUSED( end )
  Q_UNUSED( quantity )
  qDebug("ResourcesUsage::insert()");
}

/******************************************* available *******************************************/

float ResourcesUsage::available( Resource* res, QDateTime dt, QDateTime& change )
{
  // returns quantity and date-time quantity changes
  Q_UNUSED( res )
  Q_UNUSED( dt )
  Q_UNUSED( change )
  qDebug("ResourcesUsage::insert()");

  return 0.0;
}
