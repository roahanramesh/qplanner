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

#include "daysmodel.h"
#include "day.h"

#include <QTableView>
#include <QHeaderView>

/*************************************************************************************************/
/************************ Table model containing all calendar day types **************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

DaysModel::DaysModel() : QAbstractTableModel()
{
}

/****************************************** initialise *******************************************/

void DaysModel::initialise()
{
  // create initial default day types
  for ( int day=0 ; day<=Day::DEFAULT_MAX ; day++ )
    m_days.append( new Day(day) );
}

/**************************************** setColumnWidths ****************************************/

void DaysModel::setColumnWidths( QTableView* table )
{
  // set initial column widths
  table->horizontalHeader()->setDefaultSectionSize( 70 );
  table->setColumnWidth( Day::SECTION_NAME, 150 );
  table->setColumnWidth( Day::SECTION_WORK,  50 );
  table->setColumnWidth( Day::SECTION_PARTS, 50 );
}

/******************************************** rowCount *******************************************/

int DaysModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return m_days.size();
}

/****************************************** columnCount ******************************************/

int DaysModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);

  // table column count is max number of periods * 2 + SECTION_START
  int max = 0;
  foreach( Day* day, m_days )
    if ( max < day->periods() ) max = day->periods();

  return max*2 + Day::SECTION_START;
}

/********************************************** data *********************************************/

QVariant DaysModel::data( const QModelIndex& index, int role  = Qt::DisplayRole ) const
{
  // if index is not valid, return an invalid QVariant
  if ( !index.isValid() ) return QVariant();

  // if index row is out of bounds, return an invalid QVariant
  int row = index.row();
  if ( row<0 || row>=m_days.size() ) return QVariant();

  return m_days.at(row)->data( index.column(), role );
}

/******************************************** setData ********************************************/

bool DaysModel::setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole )
{
  // if index is not valid, return false - can't set data
  if ( !index.isValid() ) return false;

  // if role is not Qt::EditRole, return false - can't set data
  if ( role != Qt::EditRole ) return false;


  // TODO
  Q_UNUSED(value)
  return false;
}

/****************************************** headerData *******************************************/

QVariant DaysModel::headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  // if horizontal header, return resource header, otherwise row section number
  if ( orientation == Qt::Horizontal ) return Day::headerData( section );
  return QString("%1").arg( section+1 );
}

/********************************************* flags *********************************************/

Qt::ItemFlags DaysModel::flags( const QModelIndex& index ) const
{
  // if cell refers to non-existing working period, then cell is not selectable, etc
  int row = index.row();
  int col = index.column();
  if ( col >= m_days.at(row)->periods()*2+Day::SECTION_START ) return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // otherwise all cells are selectable, editable, etc
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

/******************************************* namesList *******************************************/

QStringList  DaysModel::namesList() const
{
  // return list of day names
  QStringList  list;
  foreach( Day* d, m_days )
    list << d->name();

  return list;
}
