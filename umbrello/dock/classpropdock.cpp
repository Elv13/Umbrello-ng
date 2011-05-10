/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2010                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "classpropdock.h"

// app includes
#include "assoctab.h"
#include "classifierlisttab.h"
#include "classifierwidget.h"
#include "classoptionstab.h"
#include "componentwidget.h"
#include "constraintlistpage.h"
#include "entity.h"
#include "objectwidget.h"
#include "pkgcontentspage.h"
#include "uml.h"
#include "umldoc.h"
#include "umlobject.h"
#include "umlview.h"
#include "umlwidgetcolorpage.h"
#include "classgentab.h"

// kde includes
#include <klocale.h>
#include <kdebug.h>
#include <kvbox.h>

// qt includes
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>


/**
 *  Sets up a ClassPropDock.
 *
 *  @param parent    The parent of the ClassPropDock
 *  @param c         The UMLObject to display properties of.
 *  @param assoc     Determines whether to display associations
 */
ClassPropDock::ClassPropDock(QWidget *parent, UMLObject * c, bool assoc)
        : QDockWidget(parent),m_pGenPage(0)
{
    init();
    m_pWidget = 0;
    m_Type = pt_Object;
    m_pObject = c;

    setupPages(assoc);

    connect(this,SIGNAL(okClicked()),this,SLOT(slotOk()));
    connect(this,SIGNAL(applyClicked()),this,SLOT(slotApply()));
}

ClassPropDock::ClassPropDock()
        : QDockWidget(0),m_pGenPage(0)
{
    m_pWidget = 0;
    m_pObject = 0;

    init();
}

void ClassPropDock::setObject(UMLObject * c, bool assoc) {
  if (m_pObject != c) {
    m_pWidget = 0;
    m_Type = pt_Object;
    m_pObject = c;
    //setupPages(assoc);
    updatePages();
    
    //add extra pages for class
    Uml::Object_Type ot = m_pObject->baseType();
    if (ot == Uml::ot_Class ) {
        m_pAttPage->setVisible(true);
    }
    if (ot == Uml::ot_Class || ot == Uml::ot_Interface) {
        m_pOpsPage->setVisible(true);
    }
    if (ot == Uml::ot_Class || ot == Uml::ot_Interface) {
        m_pTemplatePage->setVisible(true);
    }
    if (ot == Uml::ot_Enum) {
        m_pEnumLiteralPage->setVisible(true);
    }
    if (ot == Uml::ot_Entity) {
        m_pEntityAttributePage->setVisible(true);
        //setupEntityConstraintsPage();
    }
    if (ot == Uml::ot_Package ) {
        //setupContentsPage();
    }
    if (assoc) {
        m_pAssocTab->setVisible(true);
    } else {
        m_pAssocTab = 0;
    }
  }
}

/**
 *  Sets up a ClassPropDock.
 *
 *  @param  parent  The parent of the ClassPropDock
 *  @param  o       The ObjectWidget to display properties of.
 */
ClassPropDock::ClassPropDock(QWidget *parent, ObjectWidget *o)
        : QDockWidget(parent)
{
    init();
    m_pWidget = o;
    m_Type = pt_ObjectWidget;
    m_pObject = m_pWidget->umlObject();
    m_pDoc = UMLApp::app()->document();

    setupGeneralPage();

    connect(this,SIGNAL(okClicked()),this,SLOT(slotOk()));
    connect(this,SIGNAL(applyClicked()),this,SLOT(slotApply()));
}

void ClassPropDock::setObjectWidget(ObjectWidget *o) {
  if (m_pWidget != o) {
    m_pWidget = o;
    m_Type = pt_ObjectWidget;
    m_pObject = m_pWidget->umlObject();
    m_pDoc = UMLApp::app()->document();
    //setupPages();//TODO ELV
    setupGeneralPage();
  }
}

/**
 *  Sets up a ClassPropDock.
 *
 *  @param  parent  The parent of the ClassPropDock
 *  @param  o       The UMLWidget to display properties of.
 */
ClassPropDock::ClassPropDock(QWidget *parent, UMLWidget *w)
        : QDockWidget(parent)
{
    init();
    m_pWidget = w;
    m_Type = pt_Widget;
    m_pObject = w->umlObject();

    if (w->baseType() == Uml::wt_Class
            || w->baseType() == Uml::wt_Interface
            || w->baseType() == Uml::wt_Package) {
        setupPages(true);
    } else if (w->baseType() == Uml::wt_Component) {
        if ( w->isInstance() ) {
            setupInstancePages();
        } else {
            setupPages();
        }
    } else if (w->baseType() == Uml::wt_Node) {
        setupInstancePages();
    } else {
        setupPages();
    }

    // now setup the options page for classes
    if (w->baseType() == Uml::wt_Class || w->baseType() == Uml::wt_Interface) {
        setupDisplayPage();
    }
}

void ClassPropDock::setUMLWidget(UMLWidget *w) {
  if (m_pWidget != w) {
    m_pWidget = w;
    m_Type = pt_Widget;
    m_pObject = w->umlObject();
    
    if (m_pGenPage)
      m_pGenPage->setUMLWidget(w);

    if (w->baseType() == Uml::wt_Class
            || w->baseType() == Uml::wt_Interface
            || w->baseType() == Uml::wt_Package) {
        //setupPages(true);
        updatePages();
    } else if (w->baseType() == Uml::wt_Component) {
        if ( w->isInstance() ) {
            setupInstancePages();
        } else {
            //setupPages();
        updatePages();
        }
    } else if (w->baseType() == Uml::wt_Node) {
        setupInstancePages(); //TODO ELV
    } else {
        //setupPages();
        updatePages();
    }
    
  }
}

#include <QPushButton>

void ClassPropDock::init()
{
    setCaption( i18n("Properties") );
    m_pGenPage = 0;
    m_pAttPage = 0;
    m_pOpsPage = 0;
    m_pTemplatePage = 0;
    m_pEnumLiteralPage = 0;
    m_pEntityAttributePage = 0;
    m_pEntityConstraintPage = 0;
    m_pOptionsPage = 0;
    m_pColorPage = 0;
    m_pDoc = UMLApp::app()->document();
    m_pTabWidget = new QTabWidget();
    setWidget(m_pTabWidget);
    setupPages();
}

/**
 *  Standard destructor.
 */
ClassPropDock::~ClassPropDock()
{
}

/**
 * Calls slotApply() and accepts (closes) the dialog.
 */
void ClassPropDock::slotOk()
{
    slotApply();
    //QDockWidget::accept();
}

/**
 * Applies the settings in the dialog to the widget and object.
 */
void ClassPropDock::slotApply()
{
    if (m_pGenPage) {
        m_pGenPage->updateObject();
    }
    if (m_pAttPage) {
        m_pAttPage->updateObject();
    }
    if (m_pOpsPage) {
        m_pOpsPage->updateObject();
    }
    if (m_pTemplatePage) {
        m_pTemplatePage->updateObject();
    }
    if (m_pEnumLiteralPage) {
        m_pEnumLiteralPage->updateObject();
    }
    if (m_pEntityAttributePage) {
        m_pEntityAttributePage->updateObject();
    }
    if (m_pEntityConstraintPage) {
        m_pEntityConstraintPage->updateObject();
    }
    if (m_pOptionsPage) {
        m_pOptionsPage->updateUMLWidget();
    }
    if (m_pColorPage) {
        m_pColorPage->updateUMLWidget();
    }
    if (m_pWidget) {
        m_pWidget->setFont( m_pChooser->font() );
    }
}

/**
 * Sets up the general, attribute, operations, template and association pages as appropriate.
 */
void ClassPropDock::setupPages(bool assoc)
{
    setupGeneralPage();

    //TODO That's not very efficient, they dont need to be created now, but tabs index would be affacted
    // add extra pages for class
    setupAttributesPage();
    setupOperationsPage();
    setupTemplatesPage();
    setupEnumLiteralsPage();
    setupEntityAttributesPage();
    setupEntityConstraintsPage();
    setupContentsPage();
    setupAssociationsPage();
    setupDisplayPage();
        
    /*if (m_pWidget) {
      if (m_pWidget->baseType() == Uml::wt_Class || m_pWidget->baseType() == Uml::wt_Interface) {
          setupDisplayPage();
      }
    }*/
}

void ClassPropDock::updatePages()
{
    m_pGenPage->setObject(m_pObject);
    
    if (qobject_cast<UMLClassifier*>(m_pObject)) {
        
        Uml::Object_Type ot = m_pObject->baseType();
        
        if (ot == Uml::ot_Class ) {
            m_pAttPage->setClassifier((UMLClassifier*)m_pObject);
        }
        
        if (ot == Uml::ot_Class || ot == Uml::ot_Interface) {
            m_pOpsPage->setClassifier((UMLClassifier*)m_pObject);
        }
        
        if (ot == Uml::ot_Class || ot == Uml::ot_Interface) {
            m_pTemplatePage->setClassifier((UMLClassifier*)m_pObject);
        }
        
        if (ot == Uml::ot_Enum) {
            m_pEnumLiteralPage->setClassifier((UMLClassifier*)m_pObject);
        }
        
        if (ot == Uml::ot_Entity) {
            m_pEntityAttributePage->setClassifier((UMLClassifier*)m_pObject);
        }
    }
    
    if (m_pWidget) {
      if (m_pWidget->baseType() == Uml::wt_Class || m_pWidget->baseType() == Uml::wt_Interface) {
          //setupDisplayPage(); //TODO ELV
      }
    }
    
    if (m_pObject)
      setWindowTitle(m_pObject->name());
}

/**
 * Sets up the page "General" for the component.
 */
void ClassPropDock::setupGeneralPage()
{
    m_pGenPage = new ClassGenTab(m_pDoc, m_pTabWidget, m_pObject);
    m_pTabWidget->addTab(m_pGenPage,i18n("General Settings"));
}

/**
 * Sets up the page "Display" for the component.
 */
void ClassPropDock::setupDisplayPage()
{
    ClassifierWidget *cw = static_cast<ClassifierWidget*>(m_pWidget); //TODO ELV
    m_pTabWidget->addTab(m_pOptionsPage,i18n("Display"));
}

/**
 * Sets up the page "Attributes" for the component.
 */
void ClassPropDock::setupAttributesPage()
{
    m_pAttPage = new ClassifierListTab(m_pTabWidget, (UMLClassifier *)m_pObject, m_pDoc, Uml::ot_Attribute);
    m_pTabWidget->addTab(m_pAttPage,i18n("Attribute"));
}

/**
 * Sets up the page "Operations" for the component.
 */
void ClassPropDock::setupOperationsPage()
{
    m_pOpsPage = new ClassifierListTab(m_pTabWidget, (UMLClassifier*)m_pObject, m_pDoc, Uml::ot_Operation);
    m_pTabWidget->addTab(m_pOpsPage,i18n("Operations"));
}

/**
 * Sets up the page "Templates" for the component.
 */
void ClassPropDock::setupTemplatesPage()
{
    m_pTemplatePage = new ClassifierListTab(m_pTabWidget, (UMLClassifier *)m_pObject, m_pDoc, Uml::ot_Template);
    m_pTabWidget->addTab(m_pTemplatePage,i18n("Templates"));
}

/**
 * Sets up the page "Enum Literals" for the component.
 */
void ClassPropDock::setupEnumLiteralsPage()
{
    m_pEnumLiteralPage = new ClassifierListTab(m_pTabWidget, (UMLClassifier*)m_pObject, m_pDoc, Uml::ot_EnumLiteral);
    m_pTabWidget->addTab(m_pEnumLiteralPage,i18n("Enum Literals"));
}

/**
 * Sets up the page "Entity Attributes" for the component.
 */
void ClassPropDock::setupEntityAttributesPage()
{
    m_pEntityAttributePage = new ClassifierListTab(m_pTabWidget, (UMLEntity*)m_pObject, m_pDoc, Uml::ot_EntityAttribute);
    m_pTabWidget->addTab(m_pEntityAttributePage,i18n("Entity Attributes"));
}

/**
 * Sets up the page "Entity Constraints" for the component.
 */
void ClassPropDock::setupEntityConstraintsPage()
{
    /*m_pEntityConstraintPage = new ConstraintListPage(page, (UMLClassifier*)m_pObject, m_pDoc, Uml::ot_EntityConstraint );
    QHBoxLayout* entityConstraintsLayout = new QHBoxLayout(page);
    entityConstraintsLayout->addWidget(m_pEntityConstraintPage);
    m_pTabWidget->addTab(m_pEntityConstraintPage,i18n("Entity Constraints"));*///TODO ELV port to the tab
}

/**
 * Sets up the page "Contents" for the component.
 */
void ClassPropDock::setupContentsPage()
{
    /*m_pPkgContentsPage = new PkgContentsPage(m_pTabWidget, (UMLPackage*)m_pObject);
    QHBoxLayout* contentsLayout = new QHBoxLayout(page);
    contentsLayout->addWidget(m_pPkgContentsPage);
    m_pTabWidget->addTab(m_pPkgContentsPage,i18nc("contents settings page name", "Contents"));*///TODO ELV port to the tab
}

/**
 * Sets up the page "Associations" for the component.
 */
void ClassPropDock::setupAssociationsPage()
{
    m_pAssocTab = new AssocTab(m_pTabWidget, UMLApp::app()->currentView(), m_pObject);
    m_pTabWidget->addTab(m_pAssocTab,i18n("Associations"));
}

/**
 * Sets up the general page for the component.
 */
void ClassPropDock::setupInstancePages()
{
    m_pGenPage = new ClassGenTab(m_pDoc, m_pTabWidget, m_pWidget);
    m_pTabWidget->addTab(m_pGenPage,i18nc("instance general settings page name", "General"));
    m_pAssocTab = 0;
}

void ClassPropDock::focusName()
{
    m_pGenPage->focusName();
}

/**
 * Enable or disable tabs for the current type of object
 */
void ClassPropDock::enableTabs()
{
    Uml::Object_Type ot = m_pObject->baseType();
    m_pTabWidget->setTabEnabled(m_pTabWidget->indexOf(m_pAttPage),ot == Uml::ot_Class);
    m_pTabWidget->setTabEnabled(m_pTabWidget->indexOf(m_pOpsPage),ot == Uml::ot_Class || ot == Uml::ot_Interface);
    m_pTabWidget->setTabEnabled(m_pTabWidget->indexOf(m_pTemplatePage),ot == Uml::ot_Class || ot == Uml::ot_Interface);
    m_pTabWidget->setTabEnabled(m_pTabWidget->indexOf(m_pEnumLiteralPage),ot == Uml::ot_Enum);
    //setupEntityConstraintsPage();  //TODO ELV
    m_pTabWidget->setTabEnabled(m_pTabWidget->indexOf(m_pEntityAttributePage),ot == Uml::ot_Entity);
    if (ot == Uml::ot_Package ) {
        //setupContentsPage();
    }
}



#include "classpropdock.moc"
