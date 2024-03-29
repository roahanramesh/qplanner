/***************************************************************************
 *   Copyright (C) 2014 by Richard Crook                                   *
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
#include "tasksmodel.h"
#include "task.h"
#include "commandtaskindent.h"
#include "commandtaskoutdent.h"

#include <QTableView>
#include <QHeaderView>
#include <QXmlStreamWriter>
#include <QMessageBox>

/*************************************************************************************************/
/**************************** Table model containing all plan tasks ******************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

TasksModel::TasksModel() : QAbstractTableModel()
{
  // create plan summary task, also known as task zero, usually hidden
  m_tasks.append( new Task(true) );
}

/****************************************** destructor *******************************************/

TasksModel::~TasksModel()
{
  // delete all tasks in model
  foreach( Task* t, m_tasks ) delete t;
}

/****************************************** initialise *******************************************/

void TasksModel::initialise()
{
  // create initial plan blank tasks
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
}

/********************************************* task **********************************************/

Task* TasksModel::task( int n )
{
  // return pointer to n'th task, checking n is in range first
  if ( n >=0 && n < m_tasks.size() ) return m_tasks.at(n);

  qWarning("TasksModel::task - out of range '%i'",n);
  return nullptr;
}

/***************************************** saveToStream ******************************************/

void  TasksModel::saveToStream( QXmlStreamWriter* stream )
{
  // write tasks data to xml stream
  stream->writeStartElement( "tasks-data" );

  foreach( Task* t, m_tasks )
  {
    // don't write 'plan summary' task 0
    if ( plan->index(t) == 0 ) continue;

    stream->writeStartElement( "task" );
    stream->writeAttribute( "id", QString("%1").arg(plan->index(t)) );
    if ( !t->isNull() ) t->saveToStream( stream );
    stream->writeEndElement();
  }

  foreach( Task* t, m_tasks )
  {
    if ( !t->predecessorsString().isEmpty() )
    {
      stream->writeStartElement( "predecessors" );
      stream->writeAttribute( "task", QString("%1").arg(plan->index(t)) );
      stream->writeAttribute( "preds", t->predecessorsString() );
      stream->writeEndElement();
    }
  }

  // close tasks-data element
  stream->writeEndElement();
}

/**************************************** loadFromStream *****************************************/

void  TasksModel::loadFromStream( QXmlStreamReader* stream )
{
  // load tasks data from xml stream
  while ( !stream->atEnd() )
  {
    stream->readNext();

    // if task element create new task
    if ( stream->isStartElement() && stream->name() == "task" )
      m_tasks.append( new Task(stream) );

    // if predecessors element update task
    if ( stream->isStartElement() && stream->name() == "predecessors" )
    {
      int      task = -1;
      QString  preds;
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
        if ( attribute.name() == "task" ) task  = attribute.value().toString().toInt();
        if ( attribute.name() == "preds") preds = attribute.value().toString();
      }
      plan->task( task )->setPredecessors( preds );
    }

    // when reached end of tasks data break out of loop
    if ( stream->isEndElement() && stream->name() == "tasks-data" ) break;
  }

  // ensure summaries are set correctly
  setSummaries();
}

/*************************************** nonNullTaskAbove ****************************************/

Task*  TasksModel::nonNullTaskAbove( Task* t )
{
  // returns pointer to first non-null task above, or nullptr if none
  if ( index(t) <= 0 ) return nullptr;

  int r = index(t) - 1;
  while ( r > 0 && task(r)->isNull() ) r--;
  if ( task(r)->isNull() ) return nullptr;
  return task(r);
}

/****************************************** canIndent ********************************************/

bool  TasksModel::canIndent( int row )
{
  // return true if task can be indented
  if ( row == 0 ) return false;
  if ( task(row)->isNull() ) return false;

  // non-null above is same or higher indent
  int r = row - 1;
  while ( r > 0 && task(r)->isNull() ) r--;
  if ( task(r)->isNull() ) return false;
  if ( task(r)->indent() < task(row)->indent() ) return false;
  return true;
}

/****************************************** canOutdent *******************************************/

bool  TasksModel::canOutdent( int row )
{
  // return true if task can be outdented
  if ( task(row)->isNull() ) return false;
  if ( task(row)->indent() == 0 ) return false;
  return true;
}

/***************************************** setSummaries ******************************************/

void  TasksModel::setSummaries()
{
  // set summaries for all tasks, start by assembling list of non-null tasks
  QList<Task*>  nonNull;
  for( int t = 0 ; t < m_tasks.size() ; t++ )
  {
    Task*  task = m_tasks.at( t );
    if ( !task->isNull() ) nonNull.append( task );
  }

  // last non-null task cannot be summary
  if ( nonNull.isEmpty() ) return;
  nonNull.last()->setNotSummary();

  // set summaries for all other non-null tasks
  for( int t = 0 ; t < nonNull.size()-1 ; t++ )
  {
    // assume not summary until discovered otherwise
    nonNull.at(t)->setNotSummary();

    // check if summary
    int  indent = nonNull.at(t)->indent();
    if ( indent < nonNull.at(t+1)->indent() )
    {
      int last = t + 1;
      while ( last+1 < nonNull.size() && indent < nonNull.at(last+1)->indent() ) last++;
      nonNull.at(t)->setSummaryEnd( plan->index( nonNull.at(last) ) );
    }
  }
}

/************************************** predecessorsIndentOk *************************************/

bool  TasksModel::predecessorsIndentOk( QSet<int> rows )
{
  // indent tasks, to enable checking if any forbidden predecessors created
  foreach( int row, rows )
    m_tasks.at(row)->setIndent( m_tasks.at(row)->indent() + 1 );
  setSummaries();

  // make list of tasks with bad predecessors
  QString  list;
  for( int t = 1 ; t < m_tasks.size() ; t++ )
    if ( !m_tasks.at(t)->predecessorsOK() ) list += QString("%1, ").arg(t);
  list.chop(2);

  // revert by outdenting tasks
  foreach( int row, rows )
    m_tasks.at(row)->setIndent( m_tasks.at(row)->indent() - 1 );
  setSummaries();

  // if list is not empty, ask if ok to remove forbidden predecessors automatically
  if ( !list.isEmpty() )
  {
    QString  msg = "Predecessors for tasks %2\nwill be modified to remove forbidden dependencies.";
    int ret = QMessageBox::warning( 0, "QPlanner - Indent", msg.arg(list),
                                    QMessageBox::Ok | QMessageBox::Cancel );
    if ( ret == QMessageBox::Cancel ) return false;
  }

  return true;
}

/************************************* predecessorsOutdentOk *************************************/

bool  TasksModel::predecessorsOutdentOk( QSet<int> rows )
{
  // outdent tasks, to enable checking if any forbidden predecessors created
  foreach( int row, rows )
    m_tasks.at(row)->setIndent( m_tasks.at(row)->indent() - 1 );
  setSummaries();

  // make list of tasks with bad predecessors
  QString  list;
  for( int t = 1 ; t < m_tasks.size() ; t++ )
    if ( !m_tasks.at(t)->predecessorsOK() ) list += QString("%1, ").arg(t);
  list.chop(2);

  // revert by indenting tasks
  foreach( int row, rows )
    m_tasks.at(row)->setIndent( m_tasks.at(row)->indent() + 1 );
  setSummaries();

  // if list is not empty, ask if ok to remove forbidden predecessors automatically
  if ( !list.isEmpty() )
  {
    QString  msg = "Predecessors for tasks %2\nwill be modified to remove forbidden dependencies.";
    int ret = QMessageBox::warning( 0, "QPlanner - Outdent", msg.arg(list),
                                    QMessageBox::Ok | QMessageBox::Cancel );
    if ( ret == QMessageBox::Cancel ) return false;
  }

  return true;
}

/******************************************* indentRows ******************************************/

bool  TasksModel::indentRows( QSet<int> rows )
{
  // only allow rows that can be intended
  foreach( int row, rows )
    if ( !canIndent(row) ) rows.remove(row);

  // if summary being indented, ensure all subtasks also included
  foreach( int row, rows )
    if ( task(row)->isSummary() )
    {
      for( int r=row+1 ; r <= task(row)->summaryEnd() ; r++ )
        if ( !task(r)->isNull() ) rows.insert(r);
    }

  // check for predecessors that would become forbidden
  if ( predecessorsIndentOk( rows ) == false ) return false;

  // do indenting via undo/redo command
  plan->undostack()->push( new CommandTaskIndent( rows ) );

  // return true to say successful indenting
  return true;
}

/******************************************* outdentRows *****************************************/

bool  TasksModel::outdentRows( QSet<int> rows )
{
  // only allow rows that can be intended
  foreach( int row, rows )
    if ( !canOutdent(row) ) rows.remove(row);

  // if summary being outdented, ensure all subtasks also included
  foreach( int row, rows )
    if ( task(row)->isSummary() )
    {
      for( int r=row+1 ; r <= task(row)->summaryEnd() ; r++ )
        if ( !task(r)->isNull() ) rows.insert(r);
    }

  // check for predecessors that would become forbidden
  if ( predecessorsOutdentOk( rows ) == false ) return false;

  // do outdenting via undo/redo command
  plan->undostack()->push( new CommandTaskOutdent( rows ) );

  // return true to say successful outdenting
  return true;
}

/***************************************** planBeginning *****************************************/

QDateTime TasksModel::planBeginning()
{
  // return start of earliest starting task
  QDateTime  first;
  foreach( Task* t, m_tasks )
  {
    if ( !t->isNull() && !first.isValid()   ) first = t->start();
    if ( !t->isNull() && t->start() < first ) first = t->start();
  }

  return first;
}

/******************************************** planEnd ********************************************/

QDateTime TasksModel::planEnd()
{
  // return finish of latest finishing task
  QDateTime  end;
  foreach( Task* t, m_tasks )
  {
    if ( !t->isNull() && !end.isValid() ) end = t->end();
    if ( !t->isNull() && t->end() > end ) end = t->end();
  }

  return end;
}

/******************************************** number *********************************************/

int TasksModel::number()
{
  // return number of tasks in plan excluding null tasks
  int  count = 0;
  foreach( Task* t, m_tasks )
    if ( !t->isNull() ) count++;

  return count;
}

/******************************************* schedule ********************************************/

void TasksModel::schedule()
{
  // re-schedule tasks - first construct list of tasks in correct order
  qDebug("TasksModel::schedule() -------------------- cycle started ----------------------");
  QList<Task*>   scheduleList;
  scheduleList.reserve( m_tasks.size() );

  foreach( Task* t, m_tasks )
    if ( !t->isNull() ) scheduleList.append( t );
  //---------qDebug("Tasks to schedule = %i",scheduleList.size());
  qSort( scheduleList.begin(), scheduleList.end(), Task::scheduleOrder );

  // ensure task resourcing quick access container is up-to-date
  foreach( Task* t, scheduleList )
    t->resourceProcess();

  // re-schedule each task
  foreach( Task* t, scheduleList )
  {
    //---------qDebug("Post sort %i %s",plan->index(t),qPrintable(t->name()));
    t->schedule();
  }

  // now scheduling has completed update both tasks table view and gantt view
  emit dataChanged( QAbstractTableModel::index( 0, 0 ), QAbstractTableModel::index( rowCount(), columnCount() ) );
  emit ganttChanged();
}

/**************************************** setColumnWidths ****************************************/

void TasksModel::setColumnWidths( QTableView* table )
{
  // set initial column widths
  table->horizontalHeader()->setDefaultSectionSize( 140 );
  table->setColumnWidth( Task::SECTION_TITLE,    150 );
  table->setColumnWidth( Task::SECTION_DURATION,  60 );
  table->setColumnWidth( Task::SECTION_WORK,      50 );
  table->setColumnWidth( Task::SECTION_PREDS,     80 );
  table->setColumnWidth( Task::SECTION_RES,       80 );
  table->setColumnWidth( Task::SECTION_TYPE,     150 );
  table->setColumnWidth( Task::SECTION_PRIORITY,  50 );
  table->setColumnWidth( Task::SECTION_COST,      50 );
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
  // if index matches override index, return override value
  if ( index == m_overrideIndex )
    if ( role == Qt::DisplayRole || role == Qt::EditRole )
      return m_overrideValue;

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
  // if index is not valid, return false - can't set data
  if ( !index.isValid() ) return false;

  // if role is not Qt::EditRole, return false - can't set data
  if ( role != Qt::EditRole ) return false;

  // try to set data
  int row = index.row();
  int col = index.column();
  return m_tasks.at( row )->setData( col, value );
}

/****************************************** headerData *******************************************/

QVariant TasksModel::headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  // if horizontal header, return task header, otherwise row section number
  if ( orientation == Qt::Horizontal ) return Task::headerData( section );
  return QString("%1").arg( section );
}

/********************************************* flags *********************************************/

Qt::ItemFlags TasksModel::flags( const QModelIndex& index ) const
{
  // if task is null (blank), then only title is editable, others are not
  int row = index.row();
  int col = index.column();
  if ( m_tasks.at(row)->isNull() &&  col != Task::SECTION_TITLE )
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // if task is summary, then some cells not editable
  if ( m_tasks.at(row)->isSummary() &&
       col != Task::SECTION_TITLE &&
       col != Task::SECTION_PREDS &&
       col != Task::SECTION_DEADLINE &&
       col != Task::SECTION_RES &&
       col != Task::SECTION_PRIORITY &&
       col != Task::SECTION_COMMENT )
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // otherwise cell is enabled, selectable, editable
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
