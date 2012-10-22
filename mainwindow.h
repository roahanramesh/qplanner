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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QUndoView;

/*************************************************************************************************/
/********************* Main application window showing tabbed main screens ***********************/
/*************************************************************************************************/

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow( QWidget *parent = 0 );               // constructor
  
public slots:
  void slotUndoStackView( bool );              // slot for actionUndoStackView triggered signal
  void slotUndoStackViewDestroyed();           // slot for undo stack view destroyed signal
  void slotTabChange( int );                   // slot for mainTabWidget current changed signal

private:
  Ui::MainWindow*  ui;                         // user interface created using qt designer
  QUndoView*       m_undoview;                 // window to display contents of undostack
};

#endif // MAINWINDOW_H
