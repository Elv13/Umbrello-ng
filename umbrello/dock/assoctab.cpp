/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#include "assoctab.h"

#include "assocpropdlg.h"
#include "umlwidget.h"
#include "assocrules.h"
#include "association.h"

#include <klocale.h>
#include <kdebug.h>
#include <kcombobox.h>

#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QHeaderView>

AssocTab::AssocTab(QWidget *parent, UMLView * v, UMLObject * o) : QWidget(parent)
{
    m_pObject = o;
    m_pView = v;
    int margin = fontMetrics().height();

    QHBoxLayout * mainLayout = new QHBoxLayout(this);

    m_pAssocTW = new QTableWidget(this);
    m_pAssocTW->setContextMenuPolicy(Qt::CustomContextMenu);
    m_pAssocTW->setSelectionBehavior(QAbstractItemView::SelectRows);
    setupCols();
    
    m_pAssocTW->verticalHeader()->setVisible(false);
    
    mainLayout->addWidget(m_pAssocTW);
    //fillListBox();
    m_pMenu = 0;

    connect(m_pAssocTW, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
            this, SLOT(slotDoubleClick(QTableWidgetItem *)));
    connect(m_pAssocTW, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(slotRightButtonPressed(const QPoint &)));
}

AssocTab::~AssocTab()
{
    disconnect(m_pAssocTW, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
               this, SLOT(slotDoubleClick(QTableWidgetItem *)));
    disconnect(m_pAssocTW, SIGNAL(customContextMenuRequested(const QPoint &)),
               this, SLOT(slotRightButtonPressed(const QPoint &)));
}

void AssocTab::setupCols()
{
  m_pAssocTW->setColumnCount(6);
  m_pAssocTW->setHorizontalHeaderItem(0,new QTableWidgetItem("Name"));
  m_pAssocTW->setHorizontalHeaderItem(1,new QTableWidgetItem("From"));
  m_pAssocTW->setHorizontalHeaderItem(2,new QTableWidgetItem("To"));
  m_pAssocTW->setHorizontalHeaderItem(3,new QTableWidgetItem("Association type"));
  m_pAssocTW->setHorizontalHeaderItem(4,new QTableWidgetItem("More"));
  m_pAssocTW->setHorizontalHeaderItem(5,new QTableWidgetItem("Doc"));
  
  m_pAssocTW->horizontalHeader()->setResizeMode(0,QHeaderView::Stretch);

  for (int i=1;i<12;i++) {
    m_pAssocTW->horizontalHeader()->setResizeMode(i,QHeaderView::ResizeToContents);
  }
}

void AssocTab::setObject(UMLObject * o, UMLView * v)
{
  if (m_pObject != o) {
    m_pObject = o;
    m_pView = v;
    fillListBox();
  }
}

void AssocTab::slotDoubleClick(QTableWidgetItem * item)
{
    if (!item) {
        return;
    }

    int row = m_pAssocTW->currentRow();
    if ( row == -1 ) {
        return;
    }

    AssociationWidget * a = m_List.at(row);
    if (a->showDialog()) {
        fillListBox();
    }
}

/**
 *  Fills the list box with the objects associations.
 */
void AssocTab::fillListBox()
{
    m_List.clear();
    m_pAssocTW->clear();
    setupCols();
    m_pAssocTW->setRowCount(0);
    m_pView->getWidgetAssocs(m_pObject, m_List);
    int i = 0;
    foreach( AssociationWidget* assocwidget, m_List ) {
        if( assocwidget->associationType() != Uml::at_Anchor) {
            m_pAssocTW->setRowCount(m_pAssocTW->rowCount()+1);
            
            QTableWidgetItem* nameWidget = new QTableWidgetItem("");
            nameWidget->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            nameWidget->setText(assocwidget->getName());
            m_pAssocTW->setItem(m_pAssocTW->rowCount()-1,0,nameWidget);
          
            /*QTableWidgetItem* newItem = new QTableWidgetItem(assocwidget->toString());
            m_pAssocTW->setItem(m_pAssocTW->rowCount()-1,1,newItem);*/
            KComboBox *typeCB = new KComboBox(this);
            m_pAssocTW->setCellWidget(m_pAssocTW->rowCount()-1,3,typeCB);
            
            Uml::Association_Type currentType =  assocwidget->associationType();
            
            int found=-1;
            QStringList assocTypeStrings;
            QList<Uml::Association_Type> assocType;
            // dynamically load all allowed associations
            for ( int i = Uml::at_Generalization; i<= Uml::at_Relationship ;  ++i ) {
                // we don't need to check for current type
                if (  ( Uml::Association_Type )i == currentType )
                    continue;

                if ( AssocRules::allowAssociation( ( Uml::Association_Type )i, assocwidget->getWidget( Uml::A ),
                                                  assocwidget->getWidget( Uml::B ))
                    ) {
                    assocType << (Uml::Association_Type)i;
                    assocTypeStrings << UMLAssociation::toString((Uml::Association_Type)i);
                    if ((Uml::Association_Type)i == currentType) 
                      found=true;
                }
            }

  
            if (found == -1) {
                assocType.insert(0, currentType);
                assocTypeStrings.insert(0,UMLAssociation::toString(currentType));
            }
            else {
              typeCB->setCurrentIndex(found);
            }
            typeCB->addItems(assocTypeStrings);
            
            QTableWidgetItem* newItem2 = new QTableWidgetItem(assocwidget->getWidget(Uml::A)->name());
            m_pAssocTW->setItem(m_pAssocTW->rowCount()-1,1,newItem2);
            
            QTableWidgetItem* newItem3 = new QTableWidgetItem(assocwidget->getWidget(Uml::B)->name());
            m_pAssocTW->setItem(m_pAssocTW->rowCount()-1,2,newItem3);
            
            QPushButton* morePb = new QPushButton(this);
            morePb->setText("Doc");
            m_pAssocTW->setCellWidget(m_pAssocTW->rowCount()-1,4,morePb);
            
            QPushButton* docPb = new QPushButton(this);
            docPb->setText("Doc");
            m_pAssocTW->setCellWidget(m_pAssocTW->rowCount()-1,5,docPb);
            
            i++;
        }
    }
}

void AssocTab::slotRightButtonPressed(const QPoint &p)
{
    QTableWidgetItem* item = m_pAssocTW->itemAt(p);
    if (!item) {
        return;
    }
    if (m_pMenu) {
        m_pMenu->hide();
        disconnect(m_pMenu, SIGNAL(activated(int)), this, SLOT(slotPopupMenuSel(int)));
        delete m_pMenu;
        m_pMenu = 0;
    }
    m_pMenu = new ListPopupMenu(this, ListPopupMenu::mt_Association_Selected);
    connect(m_pMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotPopupMenuSel(QAction*)));
    m_pMenu->exec(m_pAssocTW->mapToGlobal(p));
}

void AssocTab::slotPopupMenuSel(QAction* action)
{
    int currentItemIndex = m_pAssocTW->currentRow();
    if ( currentItemIndex == -1 ) {
        return;
    }
    AssociationWidget * a = m_List.at(currentItemIndex);
    ListPopupMenu::Menu_Type id = m_pMenu->getMenuType(action);
    switch (id) {
    case ListPopupMenu::mt_Delete:
        m_pView->removeAssocInViewAndDoc(a);
        fillListBox();
        break;

    case ListPopupMenu::mt_Line_Color:
        //:TODO:
        uDebug() << "Menu_Type mt_Line_Color not yet implemented!";
        break;

    case ListPopupMenu::mt_Properties:
        slotDoubleClick(m_pAssocTW->currentItem());
        break;

    default:
        uDebug() << "Menu_Type " << id << " not implemented";
    }
}

#include "assoctab.moc"
