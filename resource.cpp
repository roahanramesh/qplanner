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

#include "resource.h"

#include <QVariant>

/*************************************************************************************************/
/************************************* Single plan resource **************************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Resource::Resource()
{
  // set resource variables to default/null values
  m_availability = 1.0;
  m_cost         = 0.0;
  m_calendar     = 0;
}

/****************************************** headerData *******************************************/

QVariant  Resource::headerData( int section )
{
  // return section horizontal header title text
  if (section == SECTION_INITIALS)       return "Initials";
  else if (section == SECTION_NAME)      return "Name";
  else if (section == SECTION_ORG)       return "Organisation";
  else if (section == SECTION_GROUP)     return "Group";
  else if (section == SECTION_START)     return "Start";
  else if (section == SECTION_END)       return "End";
  else if (section == SECTION_AVAIL)     return "Available";
  else if (section == SECTION_COST)      return "Cost";
  else if (section == SECTION_CALENDAR)  return "Base Calendar";
  else if (section == SECTION_COMMENT)   return "Comment";
  else return QVariant();
}
