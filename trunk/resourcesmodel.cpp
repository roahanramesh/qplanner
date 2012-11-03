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

#include "resourcesmodel.h"
#include "resource.h"

#include <QTableView>

/*************************************************************************************************/
/**************************** Table model containing all resources *******************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ResourcesModel::ResourcesModel() : QAbstractTableModel()
{
}

/****************************************** initialise ******************************************/

void ResourcesModel::initialise()
{
  // create initial blank resource
  m_resources.append( new Resource() );
}

/**************************************** setColumnWidths ****************************************/

void ResourcesModel::setColumnWidths( QTableView* table )
{
  // set initial column widths
  table->setColumnWidth( Resource::SECTION_INITIALS,  50 );
  table->setColumnWidth( Resource::SECTION_NAME,     150 );
  table->setColumnWidth( Resource::SECTION_ORG,      150 );
  table->setColumnWidth( Resource::SECTION_GROUP,    150 );
  table->setColumnWidth( Resource::SECTION_AVAIL,     65 );
  table->setColumnWidth( Resource::SECTION_ABILITY,   65 );
  table->setColumnWidth( Resource::SECTION_COST,      65 );
  table->setColumnWidth( Resource::SECTION_COMMENT,  250 );
}

/******************************************** rowCount *******************************************/

int ResourcesModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return m_resources.size();
}

/****************************************** columnCount ******************************************/

int ResourcesModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return Resource::SECTION_MAXIMUM + 1;
}

/********************************************** data *********************************************/

QVariant ResourcesModel::data( const QModelIndex& index, int role  = Qt::DisplayRole ) const
{
  // if index is not valid, return an invalid QVariant
  if ( !index.isValid() ) return QVariant();

  // if index row is out of bounds, return an invalid QVariant
  int row = index.row();
  if ( row<0 || row>=m_resources.size() ) return QVariant();

  return m_resources.at(row)->data( index.column(), role );
}

/******************************************** setData ********************************************/

bool ResourcesModel::setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole )
{
  // if index is not valid, return false - can't set data
  if ( !index.isValid() ) return false;

  // if role is not Qt::EditRole, return false - can't set data
  if ( role != Qt::EditRole ) return false;


  // TODO
  return false;
}

/****************************************** headerData *******************************************/

QVariant ResourcesModel::headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  // if horizontal header, return resource header, otherwise row section number
  if ( orientation == Qt::Horizontal ) return Resource::headerData( section );
  return QString("%1").arg( section+1 );
}

/********************************************* flags *********************************************/

Qt::ItemFlags ResourcesModel::flags( const QModelIndex& index ) const
{
  // if initials are blank, other sections are not editable
  int row = index.row();
  int col = index.column();
  if ( col != Resource::SECTION_INITIALS  &&  m_resources.at(row)->initials().isEmpty() )
    return 0;

  // otherwise item is selectable/enabled/editable
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
