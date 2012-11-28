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

#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>

class Task;
class QTableView;

/*************************************************************************************************/
/**************************** Table model containing all plan tasks ******************************/
/*************************************************************************************************/

class TasksModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  TasksModel();                                                   // constructor

  void           initialise();                                    // create initial default contents
  void           schedule();                                      // re-schedule tasks
  void           setColumnWidths( QTableView* );                  // set initial column widths

  Task*          task( int n ) { return m_tasks.at(n); }          // return the n'th day type
  int            index( Task* t ) { return m_tasks.indexOf(t); }  // return index of task
  QDateTime      end() { return QDateTime::currentDateTime(); }   // return plan end *** TODO ***
  int            number() { return m_tasks.size(); }              // return number of tasks in plan

  void           emitDataChangedRow( int row )                    // emit data changed signal for row
                   { emit dataChanged( QAbstractTableModel::index( row, 0 ),
                                       QAbstractTableModel::index( row, columnCount() ) ); }
  void           emitDataChangedColumn( int col )                 // emit data changed signal for column
                   { emit dataChanged( QAbstractTableModel::index( 0, col ),
                                       QAbstractTableModel::index( rowCount(), col ) ); }

  /********************* methods to support QAbstractTableModel ************************/

  int            rowCount( const QModelIndex& parent = QModelIndex() ) const;     // implement virtual row count
  int            columnCount( const QModelIndex& parent = QModelIndex() ) const;  // implement virtual column count
  QVariant       data( const QModelIndex&, int ) const;                           // implement virtual return data
  bool           setData( const QModelIndex&, const QVariant&, int );             // implement virtual set data
  QVariant       headerData( int, Qt::Orientation, int ) const;                   // implement virtual header data
  Qt::ItemFlags  flags( const QModelIndex& ) const;                               // implement virtual return flags

signals:
  void           ganttChanged( int );            // signal task changed for gantt chart

private:
  QList<Task*>    m_tasks;      // list of tasks in plan

};

#endif // TASKSMODEL_H
