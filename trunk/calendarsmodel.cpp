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

#include "calendarsmodel.h"
#include "calendar.h"

/*************************************************************************************************/
/************************** Table model containing all base calendars ****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

CalendarsModel::CalendarsModel( DaysModel* days ) : QAbstractTableModel()
{
  // create initial default calendars
  for ( int cal=0 ; cal<=Calendar::DEFAULT_MAX ; cal++ )
    m_calendars.append( new Calendar(days,cal) );
}

/******************************************** rowCount *******************************************/

int CalendarsModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);

  // TODO
  return 3;
}

/****************************************** columnCount ******************************************/

int CalendarsModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);

  // TODO
  return 3;
}

/********************************************** data *********************************************/

QVariant CalendarsModel::data( const QModelIndex& ind, int role  = Qt::DisplayRole ) const
{

  // TODO
  return QVariant();
}

/******************************************** setData ********************************************/

bool CalendarsModel::setData( const QModelIndex& ind, const QVariant& value, int role = Qt::EditRole )
{
  // if ind is not valid, return FALSE - can't set data
  if ( !ind.isValid() ) return FALSE;

  // if role is not Qt::EditRole, return FALSE - can't set data
  if ( role != Qt::EditRole ) return FALSE;


  // TODO
  return FALSE;
}

/****************************************** headerData *******************************************/

QVariant CalendarsModel::headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  if ( orientation == Qt::Vertical )
  {
      // TODO
      return "Vertical";
  }

  // TODO
  return "Horizontal";
}

/********************************************* flags *********************************************/

Qt::ItemFlags CalendarsModel::flags( const QModelIndex& ind ) const
{
  // TODO
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
