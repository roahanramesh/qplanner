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

#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDateTime>

/*************************************************************************************************/
/*************************************** Single plan task ****************************************/
/*************************************************************************************************/

class Task
{
public:
  Task();                                                         // constructor

  static QVariant   headerData( int );                            // return column header data
  QVariant          dataDisplayRole( int ) const;                 // return display text for cell
  QVariant          dataEditRole( int ) const;                    // return cell data ready for editor
  QVariant          dataBackgroundColorRole( int ) const;         // return background colour for cell
  QVariant          dataTextAlignmentRole( int ) const;           // return text alignment for cell
  QVariant          dataToolTipRole( int ) const;                 // return tool tip text for cell
  QVariant          dataFontRole( int ) const;                    // return font for cell

  bool              isSummary() const { return m_summary; }       // is the task a summary

  enum sections                 // sections to be displayed by view
  {
    SECTION_MINIMUM  = 0,
    SECTION_TITLE    = 0,
    SECTION_DURATION = 1,
    SECTION_START    = 2,
    SECTION_END      = 3,
    SECTION_WORK     = 4,
    SECTION_RES      = 5,
    SECTION_PRIORITY = 6,
    SECTION_TYPE     = 7,
    SECTION_COST     = 8,
    SECTION_COMMENT  = 9,
    SECTION_MAXIMUM  = 9
  };

  enum task_type
  {
    TYPE_ASAP_FWORK    = 0,     // Early as possible - fixed work
    TYPE_ASAP_FDUR     = 1,     // Early as possible - fixed duration
    TYPE_SON_FWORK     = 2,     // Start on - fixed work
    TYPE_SON_FDUR      = 3,     // Start on - fixed duration
    TYPE_FIXED_PERIOD  = 4,     // Fixed period
    TYPE_DEFAULT       = 1
  };

private:
  short       m_indent;          // task indent level, zero for no indent
  bool        m_summary;         // is a summary task
  bool        m_expanded;        // is summary task expanded
  QString     m_title;           // free text title
  float       m_duration;        //
  float       m_work;            //
  char        m_type;            // see enumerator task_type
  QDateTime   m_start;
  QDateTime   m_end;
  QString     m_resources;
  float       m_cost;
  short       m_priority;
  QString     m_comment;
};

#endif // TASK_H
