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

#include "resourcesdelegate.h"
#include "resource.h"

#include <QLineEdit>
#include <QDateEdit>

/*************************************************************************************************/
/********************* Delegate for displaying & editing resource data items *********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ResourcesDelegate::ResourcesDelegate() : QStyledItemDelegate()
{
  // set up validators to later use
  QRegExp rx1( "[^ ,\\[\\]]{0,10}" );   // up to 10 characters except space, comma, open & close sq brackets
  m_validator_initials = new QRegExpValidator( rx1, this );

  QRegExp rx2( "[^,\\[\\]]{0,50}" );    // up to 50 characters except comma, open & close sq brackets
  m_validator_name = new QRegExpValidator( rx2, this );
}

/***************************************** createEditor ******************************************/

QWidget*  ResourcesDelegate::createEditor( QWidget *parent,
                                           const QStyleOptionViewItem& option,
                                           const QModelIndex& index ) const
{
  // create different editors based on the column
  switch ( index.column() )
  {
    case Resource::SECTION_INITIALS:
    {
      QLineEdit* editor = dynamic_cast<QLineEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setValidator( m_validator_initials );
      return editor;
    }

    case Resource::SECTION_NAME:
    case Resource::SECTION_ORG:
    case Resource::SECTION_GROUP:
    case Resource::SECTION_ROLE:
    case Resource::SECTION_ALIAS:
    {
      QLineEdit* editor = dynamic_cast<QLineEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setValidator( m_validator_name );
      return editor;
    }

    case Resource::SECTION_START:
    case Resource::SECTION_END:
    {
      QDateEdit* editor = dynamic_cast<QDateEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setCalendarPopup( TRUE );
      return editor;
    }

    default:
      return QStyledItemDelegate::createEditor( parent, option, index );
  }
}
