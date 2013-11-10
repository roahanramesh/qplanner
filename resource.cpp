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

#include "plan.h"
#include "resource.h"
#include "calendar.h"
#include "employment.h"
#include "commandresourcesetdata.h"

#include <QXmlStreamWriter>

/*************************************************************************************************/
/************************************* Single plan resource **************************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Resource::Resource()
{
  // set resource variables to default/null values
  m_availability = 1.0;
  m_cost         = 0.0;
  m_calendar     = nullptr;
  m_ability      = 1.0;
  m_employment.setResource( this );
}

/****************************************** constructor ******************************************/

Resource::Resource( bool unassigned )
{
  Q_UNUSED( unassigned )

  // set resource variables for unassigned resource
  m_availability = 1e10;
  m_cost         = 0.0;
  m_calendar     = nullptr;
  m_ability      = 1.0;
  m_comment      = "Unassigned";
  m_employment.setResource( this );
}

/****************************************** constructor ******************************************/

Resource::Resource( QXmlStreamReader* stream ) : Resource()
{
  // create resource from stream
  foreach( QXmlStreamAttribute attribute, stream->attributes() )
  {
    if ( attribute.name() == "initials" )
      m_initials = attribute.value().toString();

    if ( attribute.name() == "name" )
      m_name = attribute.value().toString();

    if ( attribute.name() == "org" )
      m_org = attribute.value().toString();

    if ( attribute.name() == "group" )
      m_group = attribute.value().toString();

    if ( attribute.name() == "role" )
      m_role = attribute.value().toString();

    if ( attribute.name() == "alias" )
      m_alias = attribute.value().toString();

    if ( attribute.name() == "start" )
      m_start = QDate::fromString( attribute.value().toString(), "yyyy-MM-dd" );

    if ( attribute.name() == "end" )
      m_end = QDate::fromString( attribute.value().toString(), "yyyy-MM-dd" );

    if ( attribute.name() == "availability" )
      m_availability = attribute.value().toString().toFloat();

    if ( attribute.name() == "ability" )
      m_ability = attribute.value().toString().toFloat();

    if ( attribute.name() == "cost" )
      m_cost = attribute.value().toString().toFloat();

    if ( attribute.name() == "calendar" )
      m_calendar = plan->calendar( attribute.value().toString().toInt() );

    if ( attribute.name() == "comment" )
      m_comment = attribute.value().toString();
  }
}

/***************************************** saveToStream ******************************************/

void  Resource::saveToStream( QXmlStreamWriter* stream )
{
  // write resource data to xml stream
  stream->writeAttribute( "initials", m_initials );
  stream->writeAttribute( "name", m_name );
  stream->writeAttribute( "org", m_org );
  stream->writeAttribute( "group", m_group );
  stream->writeAttribute( "role", m_role );
  stream->writeAttribute( "alias", m_alias );
  stream->writeAttribute( "start", m_start.toString(Qt::ISODate) );
  stream->writeAttribute( "end", m_end.toString(Qt::ISODate) );
  stream->writeAttribute( "availability", QString("%1").arg(m_availability) );
  stream->writeAttribute( "ability", QString("%1").arg(m_ability) );
  stream->writeAttribute( "cost", "TODO" );
  stream->writeAttribute( "calendar", QString("%1").arg(plan->index(m_calendar)) );
  stream->writeAttribute( "comment", m_comment );
}

/****************************************** headerData *******************************************/

QVariant  Resource::headerData( int column )
{
  // return section horizontal header title text
  if ( column == SECTION_INITIALS )   return "Initials";
  if ( column == SECTION_NAME )       return "Name";
  if ( column == SECTION_ORG )        return "Organisation";
  if ( column == SECTION_GROUP )      return "Group";
  if ( column == SECTION_ROLE )       return "Role";
  if ( column == SECTION_ALIAS )      return "Alias";
  if ( column == SECTION_START )      return "Start";
  if ( column == SECTION_END )        return "End";
  if ( column == SECTION_AVAIL )      return "Available";
  if ( column == SECTION_ABILITY )    return "Ability";
  if ( column == SECTION_COST )       return "Cost";
  if ( column == SECTION_CALENDAR )   return "Calendar";
  if ( column == SECTION_COMMENT )    return "Comment";
  return QVariant();
}

/********************************************* data **********************************************/

QVariant  Resource::data( int column, int role )
{
  // if initials are blank, other sections are grayed (as not editable)
  if ( role == Qt::BackgroundRole )
  {
    if ( column != SECTION_INITIALS && isNull() )
      return plan->nullCellColour();
  }

  // if initials are blank don't show anything
  if ( isNull() ) return QVariant();

  // if role is EditRole, return appropriate edit value
  if ( role == Qt::EditRole )
  {
    if ( column == SECTION_START )    return m_start;
    if ( column == SECTION_END )      return m_end;
    if ( column == SECTION_AVAIL )    return m_availability;
    if ( column == SECTION_ABILITY )  return m_ability;
    if ( column == SECTION_COST )     return m_cost;
    if ( column == SECTION_CALENDAR ) return plan->index( m_calendar );

    // for all other columns return the DisplayRole for EditRole
    role = Qt::DisplayRole;
  }

  // if role is DisplayRole, return appropriate display value
  if ( role == Qt::DisplayRole )
  {
    if ( column == SECTION_INITIALS ) return m_initials;
    if ( column == SECTION_NAME )     return m_name;
    if ( column == SECTION_ORG )      return m_org;
    if ( column == SECTION_GROUP )    return m_group;
    if ( column == SECTION_ROLE )     return m_role;
    if ( column == SECTION_ALIAS )    return m_alias;
    if ( column == SECTION_START )    return m_start.toString( "dd/MM/yyyy" );
    if ( column == SECTION_END )      return m_end.toString( "dd/MM/yyyy" );
    if ( column == SECTION_AVAIL )    return QString("%1").arg( m_availability, 0, 'f', 1 );
    if ( column == SECTION_ABILITY )  return QString("%1").arg( m_ability, 0, 'f', 1 );
    if ( column == SECTION_COST )     return QString("%1").arg( m_cost, 0, 'f', 1 );;
    if ( column == SECTION_CALENDAR ) return m_calendar->name();
    if ( column == SECTION_COMMENT )  return m_comment;
  }

  // if role is TextAlignmentRole, return appropriate display alignment
  if ( role == Qt::TextAlignmentRole )
  {
    if ( column == SECTION_NAME ||
         column == SECTION_ORG  ||
         column == SECTION_GROUP ||
         column == SECTION_CALENDAR ||
         column == SECTION_COMMENT )   return int( Qt::AlignVCenter | Qt::AlignLeft );
    return Qt::AlignCenter;
  }

  // otherwise return null QVariant
  return QVariant();
}

/******************************************** setData ********************************************/

bool  Resource::setData( int row, int col, const QVariant& new_value )
{
  // TODO some checks that set data will be allowed, return false if not allowed

  // if value hasn't changed, don't proceed
  QVariant old_value = data( col, Qt::EditRole );
  if ( new_value == old_value ) return false;

  // set data via undo/redo command
  plan->undostack()->push( new CommandResourceSetData( row, col, new_value, old_value ) );
  return true;
}

/***************************************** setDataDirect *****************************************/

void  Resource::setDataDirect( int col, const QVariant& value )
{
  // update resource (should only be called by undostack)
  if ( col == SECTION_INITIALS ) m_initials     = value.toString().simplified();
  if ( col == SECTION_NAME )     m_name         = value.toString().simplified();
  if ( col == SECTION_ORG )      m_org          = value.toString().simplified();
  if ( col == SECTION_GROUP )    m_group        = value.toString().simplified();
  if ( col == SECTION_ROLE )     m_role         = value.toString().simplified();
  if ( col == SECTION_ALIAS )    m_alias        = value.toString().simplified();
  if ( col == SECTION_AVAIL )    m_availability = value.toFloat();
  if ( col == SECTION_ABILITY )  m_ability      = value.toFloat();
  if ( col == SECTION_CALENDAR ) m_calendar     = plan->calendar( value.toInt() );
  if ( col == SECTION_START )    m_start        = value.toDate();
  if ( col == SECTION_END )      m_end          = value.toDate();
  if ( col == SECTION_COST )     m_cost         = value.toFloat();
  if ( col == SECTION_COMMENT )  m_comment      = value.toString();

  if ( m_calendar == nullptr ) m_calendar = plan->calendar();
  if ( isNull() ) m_calendar = nullptr;
  plan->resources()->updateAssignable();
}

/******************************************** setData ********************************************/

QList<QString>  Resource::assignable() const
{
  // return assignable names
  QList<QString>  list;
  QString         temp;

  temp = m_initials;
  if ( !temp.isEmpty() ) list << temp;
  temp = m_name;
  if ( !temp.isEmpty() ) list << temp;
  temp = m_org;
  if ( !temp.isEmpty() ) list << temp;
  temp = m_group;
  if ( !temp.isEmpty() ) list << temp;
  temp = m_role;
  if ( !temp.isEmpty() ) list << temp;
  temp = m_alias;
  if ( !temp.isEmpty() ) list << temp;

  return list;
}

/******************************************** hasTag *********************************************/

bool Resource::hasTag( QString tag ) const
{
  // return true if tag matches on of the free text fields
  if ( tag == m_initials ) return true;
  if ( tag == m_name )     return true;
  if ( tag == m_org )      return true;
  if ( tag == m_group )    return true;
  if ( tag == m_role )     return true;
  if ( tag == m_alias )    return true;

  return false;
}

/**************************************** clearEmployment ****************************************/

void Resource::clearEmployment( Task* task )
{
  // remove employment on given task
  m_employment.clear( task );
}

/******************************************** hasTag *********************************************/

float Resource::assignable( QDateTime dt, QDateTime& change )
{
  // return quantity of resource that can be assigned at given date-time and this changes
  return m_employment.assignable( dt, change );
}

/******************************************** employ *********************************************/

void Resource::employ( Task* task, float num, QDateTime start, QDateTime end )
{
  // register employment on given task
  m_employment.employ( task, num, start, end );
}

/********************************************* work **********************************************/

float Resource::work( const Task* task )
{
  // return work done on task in days
  return m_employment.work( task );
}
