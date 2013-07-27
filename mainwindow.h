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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QUndoView;
class QItemSelection;
class QModelIndex;

/*************************************************************************************************/
/********************* Main application window showing tabbed main screens ***********************/
/*************************************************************************************************/

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow( QWidget *parent = nullptr );         // constructor

public slots:
  void slotUndoStackView( bool );              // slot for actionUndoStackView triggered signal
  void slotUndoStackViewDestroyed();           // slot for undo stack view destroyed signal
  void slotTabChange( int );                   // slot for mainTabWidget current changed signal
  void slotUpdatePropertiesWidgets();          // slot for ensuring 'Properties' tab widgets are up-to-date
  void slotSchedulePlan();                     // slot for schedule plan action
  void slotIndent();                           // slot for indent task(s) action
  void slotOutdent();                          // slot for outdent task(s) action
  void slotStretchTasks( bool );               // slot for stretch tasks action

  void slotFileNew();                          // slot for file new plan action
  void slotFileOpen();                         // slot for file open plan action
  void slotFileSave();                         // slot for file save plan action
  bool slotFileSaveAs();                       // slot for file saveAs plan action
  void slotFilePrint();                        // slot for file print action
  void slotFilePrintPreview();                 // slot for file print preview action
  void slotFileExit();                         // slot for file exit application action
  void slotAboutQPlanner();                    // slot for about qplanner action

  void slotTaskSelectionChanged( const QItemSelection&,
                                 const QItemSelection& );   // slot for task selection change
  void slotTaskDataChanged( const QModelIndex&,
                            const QModelIndex& );           // slot for task data change
  void slotEditTaskCell( const QModelIndex&,
                         const QString& );                  // slot for editing task cell

private:
  Ui::MainWindow*  ui;                         // user interface created using qt designer
  QUndoView*       m_undoview;                 // window to display contents of undostack
};

#endif // MAINWINDOW_H
