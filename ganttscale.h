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

#ifndef GANTTSCALE_H
#define GANTTSCALE_H

class QPaintEvent;
class QMenu;

#include <QWidget>
#include <QString>
#include <QDateTime>

/*************************************************************************************************/
/************************* GanttScale provides a scale for the gantt chart ***********************/
/*************************************************************************************************/

class GanttScale : public QWidget
{
public:
  GanttScale( QWidget* );                          // constructor
  void     setStart( QDateTime );                  // set start date-time for GanttScale
  void     setSecsPerPixel( double );              // set seconds per pixel scale
  void     setInterval( int );                     // set interval (XDateTime::INTERVAL_xx)
  void     setLabelFormat( QString );              // set label text format string
  QMenu*   menu();                                 // return pointer to context menu

protected:
  void     paintEvent( QPaintEvent* );             // draw GanttScale contents

private:
  QDateTime    m_start;             // start date-time for GanttScale
  double       m_secsPP;            // seconds per pixel
  int          m_interval;          // interval (XDateTime::INTERVAL_xx)
  QString      m_format;            // label text format
  int          m_stretch;           // label text stretch factor

  QMenu*       m_menu;              // menu for popup context menu
};

#endif // GANTTSCALE_H
