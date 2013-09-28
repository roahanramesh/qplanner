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

#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDateTime>

#include "predecessors.h"
#include "TaskResources.h"
#include "ganttdata.h"

class QXmlStreamWriter;
class QXmlStreamReader;

/*************************************************************************************************/
/*************************************** Single plan task ****************************************/
/*************************************************************************************************/

class Task
{
public:
  Task();                                                         // constructor (normal)
  Task( bool );                                                   // constructor (plan summary)
  Task( QXmlStreamReader* );                                      // constructor

  void              saveToStream( QXmlStreamWriter* );            // write task data to xml stream
  static QVariant   headerData( int );                            // return column header data
  QVariant          dataDisplayRole( int ) const;                 // return display text for cell
  QVariant          dataEditRole( int ) const;                    // return cell data ready for editor
  QVariant          dataBackgroundColorRole( int ) const;         // return background colour for cell
  QVariant          dataTextAlignmentRole( int ) const;           // return text alignment for cell
  QVariant          dataToolTipRole( int ) const;                 // return tool tip text for cell
  QVariant          dataFontRole( int ) const;                    // return font for cell
  bool              setData( int, const QVariant& );              // attempt to set value via undostack
  void              setDataDirect( int, const QVariant& );        // set value directly

  bool              isNull() const { return m_title.isNull(); }   // is this task null (blank)
  QString           name() const { return m_title; }              // return name of task (i.e. title)
  QDateTime         start() const;                                // return task (or summary) start date-time
  QDateTime         end() const;                                  // return task (or summary) end date-time
  TimeSpan          duration() const;                             // return task (or summary) duration
  bool              isExpanded() const { return m_expanded; }     // if summary is it expanded to show subtasks
  bool              isSummary() const { return m_summaryEnd >= 0; }    // is this task a summary
  int               summaryEnd() const { return m_summaryEnd; }   // return summary last sub-task id, or -1 if not summary
  void              setNotSummary() { m_summaryEnd = -1; }        // set task to non-summary
  void              setSummaryEnd( int s ) { m_summaryEnd = s; }  // set summary last sub-task id
  int               indent() const { return m_indent; }           // return task (or summary) indent level
  void              setIndent( short i ) { m_indent = i; }        // set task indent level
  bool              hasPredecessor( Task* ) const;                // return true if other task is predecessor of this task
  QString           predecessorsClean();                          // clean & return task predecessors
  QString           predecessorsString() const;                   // return task predecessors as string
  Predecessors&     predecessors() { return m_predecessors; }     // return task predecessors by reference
  void              setPredecessors( QString p ) { m_predecessors = p; }    // set task predecessors
  bool              predecessorsOK() const;                       // return true if no forbidden predecessors

  GanttData*        ganttData() { return &m_gantt; }              // return pointer to gantt data
  static QString    typeToString( int );                          // return type string equivalent

  static bool       scheduleOrder( Task*, Task* );                // less than function for qSort
  void              schedule();                                   // schedule task
  void              schedule_ASAP_FDUR();                         // schedule ASAP fixed duration
  QDateTime         scheduleStart() const;                        // determine start based on predecessors

  enum sections                 // sections to be displayed by view
  {
    SECTION_MINIMUM  = 0,
    SECTION_TITLE    = 0,
    SECTION_DURATION = 1,
    SECTION_START    = 2,
    SECTION_END      = 3,
    SECTION_WORK     = 4,
    SECTION_PREDS    = 5,
    SECTION_RES      = 6,
    SECTION_TYPE     = 7,
    SECTION_PRIORITY = 8,
    SECTION_DEADLINE = 9,
    SECTION_COST     = 10,
    SECTION_COMMENT  = 11,
    SECTION_MAXIMUM  = 11
  };

  enum task_type
  {
    TYPE_ASAP_FDUR     = 0,     // Early as possible - fixed duration
    TYPE_ASAP_FWORK    = 1,     // Early as possible - fixed work
    TYPE_SON_FDUR      = 2,     // Start on - fixed duration
    TYPE_SON_FWORK     = 3,     // Start on - fixed work
    TYPE_FIXED_PERIOD  = 4,     // Fixed period
    TYPE_DEFAULT       = 0
  };

private:
  short           m_indent;          // task indent level, zero for no indent
  int             m_summaryEnd;      // last sub-task id, or -1 for non-summaries
  bool            m_expanded;        // if summary, is task expanded
  GanttData       m_gantt;           // data for drawing task gantt

  QString         m_title;           // free text title
  TimeSpan        m_duration;        // duration of task
  QDateTime       m_start;           // start date-time of task
  QDateTime       m_end;             // end date-time of task
  TimeSpan        m_work;            // work effort for task
  Predecessors    m_predecessors;    // task predecessors
  TaskResources   m_resources;       // resources allocated to task
  char            m_type;            // see enumerator task_type
  int             m_priority;        // overall task priority (0 to 999 times one million)
  QDateTime       m_deadline;        // task warning deadline
  float           m_cost;            // calculated cost based on resource use
  QString         m_comment;         // free text comment
};

#endif // TASK_H
