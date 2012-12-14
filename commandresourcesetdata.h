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

#ifndef COMMANDRESOURCESETDATA_H
#define COMMANDRESOURCESETDATA_H

#include <QUndoCommand>

#include "plan.h"
#include "resource.h"
#include "resourcesmodel.h"

/*************************************************************************************************/
/*********************** Command for setting Resource data via QUndoStack ************************/
/*************************************************************************************************/

class CommandResourceSetData : public QUndoCommand
{
public:
  CommandResourceSetData( int row, int col, const QVariant& new_value, const QVariant& old_value )
  {
    // set private variables for new and old values
    m_row       = row;
    m_column    = col;
    m_new_value = new_value;
    m_old_value = old_value;

    // construct command description
    setText( QString("Resource %1 %2 = %3")
             .arg( row + 1 )
             .arg( Resource::headerData( col ).toString() )
             .arg( new_value.toString() ) );
  }

  void  redo()
  {
    // update resource with new value
    plan->resource( m_row )->setDataDirect( m_column, m_new_value );
    plan->resources()->emitDataChangedRow( m_row );
  }

  void  undo()
  {
    // revert resource back to old value
    plan->resource( m_row )->setDataDirect( m_column, m_old_value );
    plan->resources()->emitDataChangedRow( m_row );
  }

private:
  int       m_row;
  int       m_column;
  QVariant  m_new_value;
  QVariant  m_old_value;
};

#endif // COMMANDRESOURCESETDATA_H
