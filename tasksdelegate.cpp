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

#include "plan.h"
#include "tasksdelegate.h"
#include "task.h"
#include "timespanspinbox.h"

#include <QDateTimeEdit>
#include <QComboBox>
#include <QPainter>

/*************************************************************************************************/
/*********************** Delegate for displaying & editing task data items ***********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

TasksDelegate::TasksDelegate() : QStyledItemDelegate()
{
}

/********************************************* paint *********************************************/

void  TasksDelegate::paint( QPainter* painter,
                            const QStyleOptionViewItem& option,
                            const QModelIndex& index ) const
{
  // get Qt to paint the table cell as normal
  QStyledItemDelegate::paint( painter, option, index );

  // check if we need to add a summary task show/hide subtasks symbol
  if ( index.column() == Task::SECTION_TITLE &&
       plan->task( index.row() )->isSummary() )
  {
    QRect cell   = option.rect;
    int   indent = 10 * plan->task( index.row() )->indent() + 5;
    QRect sym    = QRect( cell.x() + indent, cell.center().y() - 4, 9, 9 );

    painter->setPen( Qt::black );
    painter->drawLine( sym.x()+2, sym.center().y(), sym.right()-2, sym.center().y() );

    if ( !plan->task( index.row() )->isExpanded() )
      painter->drawLine( sym.center().x(), sym.y()+2, sym.center().x(), sym.bottom()-2 );

    painter->setPen( Qt::gray );
    painter->drawRect( sym.adjusted(0,0,-1,-1) );
  }
}

/**************************************** initStyleOption ****************************************/

void  TasksDelegate::initStyleOption( QStyleOptionViewItem* option,
                                      const QModelIndex& index ) const
{
  QStyledItemDelegate::initStyleOption( option, index );

  // if column is title section, indent depending on indent level
  if ( index.column() == Task::SECTION_TITLE )
  {
    int indent = 10 * ( plan->task( index.row() )->indent() + 1.6 );
    option->rect.setX( option->rect.x() + indent );
  }
}

/***************************************** createEditor ******************************************/

QWidget*  TasksDelegate::createEditor( QWidget *parent,
                                      const QStyleOptionViewItem& option,
                                      const QModelIndex& index ) const
{
  // create different editors based on the column
  switch ( index.column() )
  {
    case Task::SECTION_DURATION:
    case Task::SECTION_WORK:
      return new TimeSpanSpinBox( parent );

    case Task::SECTION_START:
    case Task::SECTION_END:
    case Task::SECTION_DEADLINE:
    {
      QDateTimeEdit* editor = new QDateTimeEdit( parent );
      editor->setCalendarPopup( TRUE );
      return editor;
    }

    case Task::SECTION_TYPE:
    {
      QComboBox* editor = new QComboBox( parent );
      editor->addItem( Task::typeToString( Task::TYPE_ASAP_FDUR ) );
      editor->addItem( Task::typeToString( Task::TYPE_ASAP_FWORK ) );
      editor->addItem( Task::typeToString( Task::TYPE_SON_FDUR ) );
      editor->addItem( Task::typeToString( Task::TYPE_SON_FWORK ) );
      editor->addItem( Task::typeToString( Task::TYPE_FIXED_PERIOD ) );
      return editor;
    }

    default:
      return QStyledItemDelegate::createEditor( parent, option, index );
  }
}

/***************************************** setEditorData *****************************************/

void  TasksDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
{
  // set the editor initial value, method depends on editor which depends on column
  switch ( index.column() )
  {
    case Task::SECTION_DURATION:
    case Task::SECTION_WORK:
    {
      TimeSpanSpinBox*  spin  = dynamic_cast<TimeSpanSpinBox*>( editor );
      QString           value = index.model()->data( index, Qt::EditRole ).toString();
      TimeSpan          ts    = TimeSpan( value );
      spin->setValue( ts.number() );
      spin->slotSetUnits( ts.units() );
      return;
    }

    default:
      QStyledItemDelegate::setEditorData( editor, index );
      return;
  }
}

/***************************************** setModelData ******************************************/

void  TasksDelegate::setModelData( QWidget* editor,
                                  QAbstractItemModel* model,
                                  const QModelIndex& index ) const
{
  // update the model value, method depends on editor which depends on column
  switch ( index.column() )
  {
    case Task::SECTION_DURATION:
    case Task::SECTION_WORK:
    {
      TimeSpanSpinBox*  spin  = dynamic_cast<TimeSpanSpinBox*>( editor );
      model->setData( index, spin->text() );
      return;
    }

    default:
      QStyledItemDelegate::setModelData( editor, model, index );
      return;
  }
}
