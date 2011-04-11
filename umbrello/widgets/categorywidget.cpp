/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2010                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header file
#include "categorywidget.h"
// system includes
#include <QtGui/QPainter>
#include <kdebug.h>
// local includes
#include "category.h"
#include "umlview.h"
#include "listpopupmenu.h"

CategoryWidget::CategoryWidget(UMLView * view, UMLCategory *o)
  : UMLWidget(view, o)
{
    UMLWidget::setBaseType(Uml::wt_Category);
    //updateComponentSize();  Doing this during loadFromXMI() gives futile updates.
    //                  Instead, it is done afterwards by UMLWidget::activate()
}

CategoryWidget::~CategoryWidget()
{
}

void CategoryWidget::draw(QPainter & p, int offsetX, int offsetY)
{
    UMLWidget::setPenFromSettings(p);
    if ( UMLWidget::getUseFillColour() )
        p.setBrush( UMLWidget::getFillColour() );
    QFont font = UMLWidget::font();
    font.setUnderline(false);
    font.setBold(false);
    font.setItalic( m_pObject->isAbstract() );
    p.setFont( font );
    const QFontMetrics &fm = getFontMetrics(FT_NORMAL);
    const int fontHeight  = fm.lineSpacing();
    // the height is our radius
    const int h = height();
    const int w = width();
    const int r = h > w ? h : w;

    //int middleX = w / 2;
    const int textStartY = (r / 2) - (fontHeight / 2);

    // draw a circle
    p.drawEllipse(offsetX, offsetY, r, r);
    p.setPen(Qt::black);

    QString letterType('D');
    switch( static_cast<UMLCategory*>( m_pObject )->getType() ) {
       case UMLCategory::ct_Disjoint_Specialisation:
           letterType = 'D';
           break;
       case UMLCategory::ct_Overlapping_Specialisation:
           letterType = 'O';
           break;
       case UMLCategory::ct_Union:
           letterType = 'U';
           break;
       default:
           break;
    }

    p.drawText(offsetX + UC_MARGIN, offsetY + textStartY, r - UC_MARGIN * 2, fontHeight, Qt::AlignCenter, letterType );
    UMLWidget::setPenFromSettings(p);
    if(m_bSelected)
        drawSelected(&p, offsetX, offsetY);
}

QSize CategoryWidget::calculateSize()
{
    const UMLWidget::FontType ft = ( m_pObject->isAbstract() ? FT_BOLD_ITALIC : FT_BOLD );
    const QFontMetrics &fm = UMLWidget::getFontMetrics(ft);
    const int fontHeight = fm.lineSpacing();
    int radius = UC_RADIUS + fontHeight + UC_MARGIN;

    return QSize(radius, radius);
}

void CategoryWidget::saveToXMI( QDomDocument & qDoc, QDomElement & qElement )
{
    QDomElement categoryElement = qDoc.createElement( "categorywidget" );
    UMLWidget::saveToXMI( qDoc, categoryElement );
    qElement.appendChild( categoryElement );
}

void CategoryWidget::slotMenuSelection(QAction* action)
{
    UMLCategory* catObj = static_cast<UMLCategory*>(m_pObject);
    ListPopupMenu::Menu_Type sel = m_pMenu->getMenuType(action);
    switch(sel) {
      case ListPopupMenu::mt_DisjointSpecialisation:
          catObj->setType(UMLCategory::ct_Disjoint_Specialisation);
          break;

      case ListPopupMenu::mt_OverlappingSpecialisation:
          catObj->setType(UMLCategory::ct_Overlapping_Specialisation);
          break;

      case ListPopupMenu::mt_Union:
          catObj->setType(UMLCategory::ct_Union);
          break;

      default:
          UMLWidget::slotMenuSelection(action);
    }
}

