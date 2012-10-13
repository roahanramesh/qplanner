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

#include "task.h"

#include <QVariant>

/*************************************************************************************************/
/*************************************** Single plan task ****************************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Task::Task()
{
  // set task variables to default/null values
  m_indent   = 0;
  m_summary  = FALSE;
  m_expanded = TRUE;
  m_duration = -1.0;
  m_work     = -1.0;
  m_type     = TYPE_DEFAULT;
  m_cost     = -1.0;
  m_priority = 100;
}

/****************************************** headerData *******************************************/

QVariant  Task::headerData( int section )
{
  // return section horizontal header title text
  if ( section == SECTION_TITLE )         return "Title";
  else if ( section == SECTION_DURATION ) return "Duration";
  else if ( section == SECTION_WORK )     return "Work";
  else if ( section == SECTION_TYPE )     return "Type";
  else if ( section == SECTION_START )    return "Start";
  else if ( section == SECTION_END )      return "End";
  else if ( section == SECTION_RES )      return "Resources";
  else if ( section == SECTION_COST )     return "Cost";
  else if ( section == SECTION_PRIORITY ) return "Priority";
  else if ( section == SECTION_COMMENT )  return "Comment";
  else return QVariant();
}
