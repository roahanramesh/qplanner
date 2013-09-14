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

#include <QVariant>
#include <QFont>

#include "plan.h"
#include "task.h"
#include "calendar.h"
#include "commandtasksetdata.h"
#include "task_schedule.h"
#include "xdatetime.h"

#include <QXmlStreamWriter>

/*************************************************************************************************/
/*************************************** Single plan task ****************************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Task::Task()
{
  // set task variables to default/null values
  m_indent   = 0;
  m_summary  = false;
  m_expanded = true;
  m_type     = TYPE_DEFAULT;
  m_cost     = 0.0;
  m_priority = 100 * 1000000;
  m_duration = TimeSpan("1d");
  m_work     = TimeSpan("1d");
}

/****************************************** constructor ******************************************/

Task::Task( bool planSummary )
{
  Q_UNUSED( planSummary )

  // set task variables for plan summary task 0 (hidden by default)
  m_indent   = -1;
  m_summary  = true;
  m_expanded = true;
  m_type     = TYPE_DEFAULT;
  m_cost     = 0.0;
  m_priority = 100 * 1000000;
  m_comment  = "Plan summary";
}

/****************************************** constructor ******************************************/

Task::Task( QXmlStreamReader* stream ) : Task()
{
  // create task from stream
  foreach( QXmlStreamAttribute attribute, stream->attributes() )
  {
    if ( attribute.name() == "indent" )
      m_indent = attribute.value().toString().toShort();

    if ( attribute.name() == "summary" )
      m_summary = ( attribute.value() == "1" );

    if ( attribute.name() == "expanded" )
      m_expanded = ( attribute.value() == "1" );

    if ( attribute.name() == "title" )
      m_title = attribute.value().toString();

    if ( attribute.name() == "duration" )
      m_duration = attribute.value().toString();

    if ( attribute.name() == "start" )
      m_start = XDateTime::fromText( attribute.value().toString() );

    if ( attribute.name() == "end" )
      m_end = XDateTime::fromText( attribute.value().toString() );

    if ( attribute.name() == "work" )
      m_work = attribute.value().toString();

    if ( attribute.name() == "resources" )
      m_resources = attribute.value().toString();

    if ( attribute.name() == "type" )
      m_type = attribute.value().toString().toInt();

    if ( attribute.name() == "priority" )
      m_priority = attribute.value().toString().toInt() * 1e6;

    if ( attribute.name() == "cost" )
      m_cost = attribute.value().toString().toFloat();

    if ( attribute.name() == "comment" )
      m_comment = attribute.value().toString();
  }
}
/***************************************** saveToStream ******************************************/

void  Task::saveToStream( QXmlStreamWriter* stream )
{
  // write task data to xml stream
  stream->writeAttribute( "indent", QString("%1").arg(m_indent) );
  stream->writeAttribute( "summary", QString("%1").arg(m_summary) );
  stream->writeAttribute( "expanded", QString("%1").arg(m_expanded) );
  stream->writeAttribute( "title", m_title );
  stream->writeAttribute( "duration", m_duration.toString() );
  stream->writeAttribute( "start", XDateTime( m_start ).toText() );
  stream->writeAttribute( "end", XDateTime( m_end ).toText() );
  stream->writeAttribute( "work", m_work.toString() );
  stream->writeAttribute( "resources", m_resources.toString() );
  stream->writeAttribute( "type", QString("%1").arg(int(m_type)) );
  stream->writeAttribute( "priority", QString("%1").arg(m_priority/1e6) );
  stream->writeAttribute( "cost", "TODO" );
  stream->writeAttribute( "comment", m_comment );
}

/****************************************** headerData *******************************************/

QVariant  Task::headerData( int column )
{
  // return section horizontal header title text
  if ( column == SECTION_TITLE )    return "Title";
  if ( column == SECTION_DURATION ) return "Duration";
  if ( column == SECTION_WORK )     return "Work";
  if ( column == SECTION_TYPE )     return "Type";
  if ( column == SECTION_START )    return "Start";
  if ( column == SECTION_END )      return "End";
  if ( column == SECTION_PREDS )    return "Predecessors";
  if ( column == SECTION_DEADLINE ) return "Deadline";
  if ( column == SECTION_RES )      return "Resources";
  if ( column == SECTION_COST )     return "Cost";
  if ( column == SECTION_PRIORITY ) return "Priority";
  if ( column == SECTION_COMMENT )  return "Comment";
  return QVariant();
}

/**************************************** hasPredecessor *****************************************/

bool  Task::hasPredecessor( Task* task ) const
{
  // return true if task is predecessor
  return m_predecessors.hasPredecessor( task );
}

/**************************************** scheduleOrder ******************************************/

bool  Task::scheduleOrder( Task* t1, Task* t2 )
{
  // less than function for qSort - firstly if predecessor
  bool pred12 = t1->hasPredecessor( t2 );
  bool pred21 = t2->hasPredecessor( t1 );
  if ( pred12 || pred21 ) return pred21;

  // then if summary
  bool sum1 = t1->isSummary();
  bool sum2 = t2->isSummary();
  if ( sum1 && sum2 ) return plan->index(t1) > plan->index(t2);
  if ( sum1 || sum2 ) return sum2;

  // otherwise, by priority and index
  return ( t1->m_priority - plan->index(t1) ) > ( t2->m_priority - plan->index(t2) );
}

/************************************* dataBackgroundColorRole ***********************************/

QVariant  Task::dataBackgroundColorRole( int col ) const
{
  // if task is blank grey out all but title
  if ( isNull() && col != SECTION_TITLE ) return plan->nullCellColour();

  // return appropriate background colour for summary calculated cells
  if ( isSummary() &&
       col != SECTION_TITLE &&
       col != SECTION_COMMENT ) return plan->nullCellColour();

  // return appropriate background colour from plan cell
  if ( col    == SECTION_WORK &&
       m_type != TYPE_ASAP_FWORK &&
       m_type != TYPE_SON_FWORK ) return plan->nullCellColour();

  if ( col    == SECTION_DURATION &&
       m_type != TYPE_ASAP_FDUR &&
       m_type != TYPE_SON_FDUR ) return plan->nullCellColour();

  if ( col    == SECTION_START &&
       m_type != TYPE_SON_FWORK &&
       m_type != TYPE_SON_FDUR ) return plan->nullCellColour();

  if ( col    == SECTION_END &&
       m_type != TYPE_FIXED_PERIOD ) return plan->nullCellColour();

  if ( col    == SECTION_COST ) return plan->nullCellColour();

  return QVariant();
}

/***************************************** dataEditRole ******************************************/

QVariant  Task::dataEditRole( int col ) const
{
  // return data in a form suitable for editing in an editor
  if ( col == SECTION_TITLE ) return m_title; // return title without indentation

  if ( col == SECTION_TYPE  ) return m_type;  // return m_types as int for QComboBox index

  if ( col == SECTION_START )
  {
    if ( m_start.isValid() ) return m_start;
    return plan->start();
  }

  if ( col == SECTION_END )
  {
    if ( m_end.isValid() ) return m_end;
    return plan->start();
  }

  if ( col == SECTION_DEADLINE )
  {
    if ( m_deadline.isValid() ) return m_deadline;
    return plan->start();
  }

  if ( col == SECTION_WORK )
  {
    // return m_work as float for QDoubleSpinBox value
    //if ( m_work < 0 ) return 1;
    return m_work.toString();
  }

  if ( col == SECTION_DURATION )
  {
    // return m_duration as float for QDoubleSpinBox value
    //if ( m_duration < 0 ) return 1;
    return m_duration.toString();
  }

  // if not any of above return display text
  return dataDisplayRole( col );
}

/************************************* dataTextAlignmentRole *************************************/

QVariant  Task::dataTextAlignmentRole( int col ) const
{
  // return right aligned if numeric field
  if ( col == SECTION_DURATION ||
       col == SECTION_WORK ||
       col == SECTION_COST ||
       col == SECTION_PRIORITY ) return Qt::AlignRight;

  // return centre aligned if date or m_type field
  if ( col == SECTION_START ||
       col == SECTION_END ||
       col == SECTION_DEADLINE ||
       col == SECTION_TYPE ) return Qt::AlignHCenter;

  return QVariant();
}

/**************************************** dataToolTipRole ****************************************/

QVariant  Task::dataToolTipRole( int col ) const
{
  // return tool tip text for cell
  if ( col == SECTION_START && m_start.isValid() )
    return start().toString( "ddd dd MMM yyyy hh:mm" );

  if ( col == SECTION_END && m_end.isValid() )
    return end().toString( "ddd dd MMM yyyy hh:mm" );

  if ( col == SECTION_DEADLINE && m_deadline.isValid() )
    return m_deadline.toString( "ddd dd MMM yyyy hh:mm" );

  return QVariant();
}

/***************************************** dataFontRole ******************************************/

QVariant  Task::dataFontRole( int col ) const
{
  Q_UNUSED(col);

  // return bold font for cell if task is a summary
  if ( isSummary() )
  {
    QFont font;
    font.setBold( true );
    return font;
  }

  return QVariant();
}

/***************************************** dataDisplayRole ***************************************/

QVariant  Task::dataDisplayRole( int col ) const
{
  // if task is null don't display anything
  if ( isNull() ) return QVariant();

  // if summary return appropriate display text for summary calculated cells
  if ( isSummary() )
  {
    // TODO return QVariant();
  }

  // return appropriate display text from plan data
  if ( col == SECTION_TITLE ) return m_title;

  if ( col == SECTION_DURATION ) return duration().toString();

  if ( col == SECTION_WORK ) return m_work.toString();

  if ( col == SECTION_TYPE ) return typeToString( m_type );

  if ( col == SECTION_START ) return start().toString( plan->datetimeFormat() );

  if ( col == SECTION_END ) return end().toString( plan->datetimeFormat() );

  if ( col == SECTION_PREDS ) return m_predecessors.toString();

  if ( col == SECTION_DEADLINE ) return m_deadline.toString( plan->datetimeFormat() );

  if ( col == SECTION_RES ) return m_resources.toString();

  if ( col == SECTION_COST ) return QString("$ %1").arg( m_cost );

  if ( col == SECTION_PRIORITY ) return m_priority / 1000000;

  if ( col == SECTION_COMMENT ) return m_comment;

  return QVariant();
}

/***************************************** typetoString ******************************************/

QString  Task::typeToString( int type )
{
  // return type string description equivalent
  if ( type == TYPE_ASAP_FWORK )   return "ASAP - work fixed";
  if ( type == TYPE_ASAP_FDUR )    return "ASAP - duration fixed";
  if ( type == TYPE_SON_FWORK )    return "Start on - work fixed";
  if ( type == TYPE_SON_FDUR )     return "Start on - duration fixed";
  if ( type == TYPE_FIXED_PERIOD ) return "Fixed period";
  return "";
}

/******************************************** setData ********************************************/

bool  Task::setData( int row, int col, const QVariant& value )
{
  // TODO some checks that set data will be allowed, return false if not allowed

  // if value hasn't changed, don't proceed
  if ( value == dataEditRole(col) ) return false;

  // set data via undo/redo command
  plan->undostack()->push( new CommandTaskSetData( this, row, col, value ) );
  return true;
}

/***************************************** setDataDirect *****************************************/

void  Task::setDataDirect( int col, const QVariant& value )
{
  // if the task was null determine a suitable default indent
  if ( isNull() )
  {
    Task* above = plan->tasks()->nonNullTaskAbove( this );
    if ( above && above->isSummary() ) m_indent = above->indent() + 1;
    if ( above && !above->isSummary() ) m_indent = above->indent();
  }

  // update task (should only be called by undostack)
  if ( col == SECTION_TITLE )    m_title        = value.toString();
  if ( col == SECTION_DURATION ) m_duration     = value.toString();
  if ( col == SECTION_WORK )     m_work         = value.toString();
  if ( col == SECTION_TYPE )     m_type         = value.toInt();
  if ( col == SECTION_START )    m_start        = value.toDateTime();
  if ( col == SECTION_END )      m_end          = value.toDateTime();
  if ( col == SECTION_PREDS )    m_predecessors = value.toString();
  if ( col == SECTION_DEADLINE ) m_deadline     = value.toDateTime();
  if ( col == SECTION_RES )      m_resources    = value.toString();
  if ( col == SECTION_COST )     m_cost         = value.toReal();
  if ( col == SECTION_PRIORITY ) m_priority     = value.toInt() * 1000000;
  if ( col == SECTION_COMMENT )  m_comment      = value.toString();
}

/******************************************* duration ********************************************/

TimeSpan Task::duration() const
{
  // return task or summary duration
  if ( m_summary ) return plan->calendar()->workBetween( m_gantt.start(), m_gantt.end() );

  return m_duration;
}

/********************************************* start *********************************************/

QDateTime Task::start() const
{
  // return task or summary start date-time
  if ( m_summary )
  {
    int        here = plan->index( (Task*)this );
    int        last = plan->tasks()->rowCount() - 1;
    QDateTime  s    = plan->MAX_DATETIME;

    // loop through each subtask
    for( int t = here+1 ; t <= last ; t++ )
    {
      // skip null tasks
      Task*  task = plan->task( t );
      if ( task->isNull() ) continue;

      // if indent is same or lower, we have got to last subtask
      if ( task->indent() <= m_indent ) return s;

      // if task isn't summary check if its start is before current earliest
      if ( !task->isSummary() && task->start() < s ) s = task->start();
    }

    return s;
  }

  return m_start;
}

/********************************************** end **********************************************/

QDateTime Task::end() const
{
  // return task or summary end date-time
  if ( m_summary )
  {
    int        here = plan->index( (Task*)this );
    int        last = plan->tasks()->rowCount() - 1;
    QDateTime  e    = plan->MIN_DATETIME;

    // loop through each subtask
    for( int t = here+1 ; t <= last ; t++ )
    {
      // skip null tasks
      Task*  task = plan->task( t );
      if ( task->isNull() ) continue;

      // if indent is same or lower, we have got to last subtask
      if ( task->indent() <= m_indent ) return e;

      // if task isn't summary check if its end is after current latest
      if ( !task->isSummary() && task->end() > e ) e = task->end();
    }

    return e;
  }

  return m_end;
}
