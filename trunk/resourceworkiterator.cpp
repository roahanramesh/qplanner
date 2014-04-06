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

#include "resourceworkiterator.h"
#include "resource.h"
#include "calendar.h"

/*************************************************************************************************/
/****************************** Iterator for resource work periods *******************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ResourceWorkIterator::ResourceWorkIterator()
{
  // construct null ResourceWorkIterator
  m_res       = nullptr;
  m_start     = XDateTime::NULL_DATETIME;
  m_end       = XDateTime::NULL_DATETIME;
  m_efficacy  = 0.0f;
  m_available = 0.0f;
}

/****************************************** constructor ******************************************/

ResourceWorkIterator::ResourceWorkIterator( Resource* res, DateTime now, bool forward )
{
  qDebug("ResourceWorkIterator::ResourceWorkIterator() - NOT YET IMPLEMENTED");

  // construct initial resource work iterator
  m_res = res;

  if ( res->isInWorkPeriod( now ) )  // in work period
  {
    m_start = res->previousWorkPeriodStart( now );
    m_end   = res->nextWorkPeriodEnd( now );
  }
  else  // not in work period
  {
    if ( forward ) // forward
    {
      m_start = res->nextWorkPeriodStart( now );
      m_end   = res->nextWorkPeriodEnd( m_start );
    }
    else  // backwards
    {
      m_start = res->previousWorkPeriodStart( now );
      m_end   = res->nextWorkPeriodEnd( m_start );
    }
  }

  m_efficacy  = 0.0f;  // TODO
  m_available = 0.0f;  // TODO
}

/**************************************** nextWorkPeriod *****************************************/

void ResourceWorkIterator::nextWorkPeriod()
{
  qDebug("ResourceWorkIterator::nextWorkPeriod() - NOT YET IMPLEMENTED");
}

/************************************** previousWorkPeriod ***************************************/

void ResourceWorkIterator::previousWorkPeriod()
{
  qDebug("ResourceWorkIterator::previousWorkPeriod() - NOT YET IMPLEMENTED");
}

/******************************************* allocate ********************************************/

void ResourceWorkIterator::allocate()
{
  qDebug("ResourceWorkIterator::allocate() - NOT YET IMPLEMENTED");
}
