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

#include "tasksmodel.h"
#include "task.h"

#include <QTableView>

/*************************************************************************************************/
/**************************** Table model containing all plan tasks ******************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

TasksModel::TasksModel() : QAbstractTableModel()
{
}

/****************************************** initialise *******************************************/

void TasksModel::initialise()
{
  // create initial plan blank task
  m_tasks.append( new Task() );
}

/**************************************** setColumnWidths ****************************************/

void TasksModel::setColumnWidths( QTableView* table )
{
  // set initial column widths
  table->setColumnWidth( Task::SECTION_TITLE,    200 );
  table->setColumnWidth( Task::SECTION_DURATION,  60 );
  table->setColumnWidth( Task::SECTION_WORK,      60 );
  table->setColumnWidth( Task::SECTION_PRIORITY,  50 );
  table->setColumnWidth( Task::SECTION_COMMENT,  200 );
}

/******************************************** rowCount *******************************************/

int TasksModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return m_tasks.size();
}

/****************************************** columnCount ******************************************/

int TasksModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return Task::SECTION_MAXIMUM + 1;
}

/********************************************** data *********************************************/

QVariant TasksModel::data( const QModelIndex& index, int role  = Qt::DisplayRole ) const
{
  // if index is not valid, return an invalid QVariant
  if ( !index.isValid() ) return QVariant();

  // if index row is out of bounds, return an invalid QVariant
  int row = index.row();
  if ( row<0 || row>=m_tasks.size() ) return QVariant();

  int col = index.column();
  if ( role == Qt::DisplayRole )         return m_tasks.at(row)->dataDisplayRole( col );
  if ( role == Qt::ToolTipRole )         return m_tasks.at(row)->dataToolTipRole( col );
  if ( role == Qt::EditRole )            return m_tasks.at(row)->dataEditRole( col );
  if ( role == Qt::TextAlignmentRole )   return m_tasks.at(row)->dataTextAlignmentRole( col );
  if ( role == Qt::BackgroundColorRole ) return m_tasks.at(row)->dataBackgroundColorRole( col );
  if ( role == Qt::FontRole )            return m_tasks.at(row)->dataFontRole( col );

  // otherwise return an invalid QVariant
  return QVariant();
}

/******************************************** setData ********************************************/

bool TasksModel::setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole )
{
  // if ind is not valid, return FALSE - can't set data
  if ( !index.isValid() ) return FALSE;

  // if role is not Qt::EditRole, return FALSE - can't set data
  if ( role != Qt::EditRole ) return FALSE;


  // TODO
  return FALSE;
}

/****************************************** headerData *******************************************/

QVariant TasksModel::headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  // if horizontal header, return task header, otherwise row section number
  if ( orientation == Qt::Horizontal ) return Task::headerData( section );
  return QString("%1").arg( section+1 );
}

/********************************************* flags *********************************************/

Qt::ItemFlags TasksModel::flags( const QModelIndex& index ) const
{
  // if task is summary, then some cells not editable
  if ( m_tasks.at(index.row())->isSummary() &&
       index.column() != Task::SECTION_TITLE &&
       index.column() != Task::SECTION_COMMENT )
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // otherwise cell is enabled, selectable, editable
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
