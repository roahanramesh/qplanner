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

#ifndef TASKSDELEGATE_H
#define TASKSDELEGATE_H

#include <QStyledItemDelegate>

/*************************************************************************************************/
/*********************** Delegate for displaying & editing task data items ***********************/
/*************************************************************************************************/

class TasksDelegate : public QStyledItemDelegate
{
public:
  TasksDelegate();                                          // constructor

  QWidget* createEditor( QWidget*,
                         const QStyleOptionViewItem&,
                         const QModelIndex& ) const;        // create the editor
  void     setEditorData( QWidget*,
                          const QModelIndex& ) const;       // set editor data from the model
  void     setModelData( QWidget*,
                         QAbstractItemModel*,
                         const QModelIndex& ) const;        // update the model from editor data
  void     paint( QPainter*,
                  const QStyleOptionViewItem&,
                  const QModelIndex& ) const;               // paint the show/hide subtasks symbol

protected:
  void     initStyleOption( QStyleOptionViewItem*,
                            const QModelIndex& ) const;     // indent the title
};

#endif // TASKSDELEGATE_H
