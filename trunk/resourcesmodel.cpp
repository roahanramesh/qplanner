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

#include "resourcesmodel.h"
#include "resource.h"
#include "plan.h"

#include <QTableView>
#include <QXmlStreamWriter>

/*************************************************************************************************/
/**************************** Table model containing all resources *******************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ResourcesModel::ResourcesModel() : QAbstractTableModel()
{
  // create 'unassigned' resource, also known as resource zero, usually hidden
  m_resources.append( new Resource(true) );
}

/****************************************** initialise ******************************************/

void ResourcesModel::initialise()
{
  // create initial blank resource
  m_resources.append( new Resource() );
  m_resources.append( new Resource() );
  m_resources.append( new Resource() );
  m_resources.append( new Resource() );
  m_resources.append( new Resource() );
}

/********************************************* resource ***********************************************/

Resource* ResourcesModel::resource( int n )
{
  // return pointer to n'th resource, checking n is in range first
  if ( n >=0 && n < m_resources.size() ) return m_resources.at(n);

  qWarning("ResourcesModel::resource - out of range '%i'",n);
  return nullptr;
}

/***************************************** saveToStream ******************************************/

void  ResourcesModel::saveToStream( QXmlStreamWriter* stream )
{
  // write resources data to xml stream
  stream->writeStartElement( "resources-data" );

  foreach( Resource* r, m_resources )
  {
    // don't write 'unassigned' resource 0
    if ( plan->index(r) == 0 ) continue;

    stream->writeStartElement( "resource" );
    stream->writeAttribute( "id", QString("%1").arg(plan->index(r)) );
    if ( !r->isNull() ) r->saveToStream( stream );
    stream->writeEndElement();
  }

  // close resources-data element
  stream->writeEndElement();
}

/**************************************** loadFromStream *****************************************/

void  ResourcesModel::loadFromStream( QXmlStreamReader* stream )
{
  // load resources data from xml stream
  while ( !stream->atEnd() )
  {
    stream->readNext();

    // if resource element create new resource
    if ( stream->isStartElement() && stream->name() == "resource" )
      m_resources.append( new Resource(stream) );

    // when reached end of resources data return
    if ( stream->isEndElement() && stream->name() == "resources-data" ) return;
  }
}

/******************************************** number *********************************************/

int ResourcesModel::number()
{
  // return number of resources in plan excluding null resources
  int  count = 0;
  foreach( Resource* res, m_resources )
    if ( !res->isNull() ) count++;

  return count;
}

/**************************************** updateAssignable ***************************************/

void ResourcesModel::updateAssignable()
{
  // start with an empty set
  m_assignable.clear();

  // determine assignable list
  foreach( Resource* res, m_resources )
    foreach( QString str, res->assignable() )
      m_assignable.insert( str );
}

/**************************************** setColumnWidths ****************************************/

void ResourcesModel::setColumnWidths( QTableView* table )
{
  // set initial column widths
  table->setColumnWidth( Resource::SECTION_INITIALS,  60 );
  table->setColumnWidth( Resource::SECTION_NAME,     150 );
  table->setColumnWidth( Resource::SECTION_ORG,      150 );
  table->setColumnWidth( Resource::SECTION_GROUP,    150 );
  table->setColumnWidth( Resource::SECTION_AVAIL,     65 );
  table->setColumnWidth( Resource::SECTION_ABILITY,   65 );
  table->setColumnWidth( Resource::SECTION_COST,      65 );
  table->setColumnWidth( Resource::SECTION_COMMENT,  250 );
}

/******************************************** rowCount *******************************************/

int ResourcesModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return m_resources.size();
}

/****************************************** columnCount ******************************************/

int ResourcesModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return Resource::SECTION_MAXIMUM + 1;
}

/********************************************** data *********************************************/

QVariant ResourcesModel::data( const QModelIndex& index,
                               int role  = Qt::DisplayRole ) const
{
  // if index is not valid, return an invalid QVariant
  if ( !index.isValid() ) return QVariant();

  // if index row is out of bounds, return an invalid QVariant
  int row = index.row();
  if ( row<0 || row>=m_resources.size() ) return QVariant();

  return m_resources.at( row )->data( index.column(), role );
}

/******************************************** setData ********************************************/

bool ResourcesModel::setData( const QModelIndex& index,
                              const QVariant& value,
                              int role = Qt::EditRole )
{
  // if index is not valid, return false - can't set data
  if ( !index.isValid() ) return false;

  // if role is not Qt::EditRole, return false - can't set data
  if ( role != Qt::EditRole ) return false;

  // try to set data
  int row = index.row();
  int col = index.column();
  return m_resources.at( row )->setData( row, col, value );
}

/****************************************** headerData *******************************************/

QVariant ResourcesModel::headerData( int section,
                                     Qt::Orientation orientation,
                                     int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  // if horizontal header, return resource header, otherwise row section number
  if ( orientation == Qt::Horizontal ) return Resource::headerData( section );
  return QString("%1").arg( section );
}

/********************************************* flags *********************************************/

Qt::ItemFlags ResourcesModel::flags( const QModelIndex& index ) const
{
  // if resource is null (blank), then only initials is editable, others are not
  int row = index.row();
  int col = index.column();
  if ( m_resources.at(row)->isNull() &&  col != Resource::SECTION_INITIALS )
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // otherwise item is selectable/enabled/editable
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
