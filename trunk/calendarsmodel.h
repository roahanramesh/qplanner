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

#ifndef CALENDARSMODEL_H
#define CALENDARSMODEL_H

#include <QAbstractTableModel>

class Calendar;
class DaysModel;
class QXmlStreamWriter;
class QXmlStreamReader;

/*************************************************************************************************/
/************************** Table model containing all base calendars ****************************/
/*************************************************************************************************/

class CalendarsModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  CalendarsModel();                                                       // constructor
  ~CalendarsModel();                                                      // destructor

  void           initialise();                                            // create initial default contents
  void           saveToStream( QXmlStreamWriter* );                       // write calendars data to xml stream
  void           loadFromStream( QXmlStreamReader* );                     // load calendars data from xml stream

  Calendar*      calendar( int n );                                       // return pointer to n'th calendar
  int            index( Calendar* c ) { return m_calendars.indexOf(c); }  // return index of calendar, or -1
  int            number() { return m_calendars.size(); }                  // return number of calendars in plan
  QStringList    namesList() const;                                       // return list of calendar names

  /********************* methods to support QAbstractTableModel ************************/

  int            rowCount( const QModelIndex& parent = QModelIndex() ) const;     // implement virtual row count
  int            columnCount( const QModelIndex& parent = QModelIndex() ) const;  // implement virtual column count
  QVariant       data( const QModelIndex&, int ) const;                           // implement virtual return data
  bool           setData( const QModelIndex&, const QVariant&, int );             // implement virtual set data
  QVariant       headerData( int, Qt::Orientation, int ) const;                   // implement virtual header data
  Qt::ItemFlags  flags( const QModelIndex& ) const;                               // implement virtual return flags

private:
  QList<Calendar*>   m_calendars;     // list of calendars available to plan

};

#endif // CALENDARSMODEL_H
