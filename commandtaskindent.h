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

#ifndef COMMANDTASKINDENT_H
#define COMMANDTASKINDENT_H

#include <QUndoCommand>
#include <QSet>

#include "plan.h"
#include "tasksmodel.h"
#include "task.h"

/*************************************************************************************************/
/************************** Command for indenting Tasks via QUndoStack ***************************/
/*************************************************************************************************/

class CommandTaskIndent : public QUndoCommand
{
public:
  CommandTaskIndent( QSet<int> rows )
  {
    // set private variables
    m_rows  = rows;

    // construct command description
    setText( "Indent" );
  }

  void  redo()
  {
    // indent tasks
    foreach( int row, m_rows )
      plan->task(row)->setIndent( plan->task(row)->indent() + 1 );

    plan->tasks()->setSummaries();
    plan->schedule();
  }

  void  undo()
  {
    // revert by outdenting tasks
    foreach( int row, m_rows )
      plan->task(row)->setIndent( plan->task(row)->indent() - 1 );

    plan->tasks()->setSummaries();
    plan->schedule();
  }

private:
  QSet<int>     m_rows;
};

#endif // COMMANDTASKINDENT_H
