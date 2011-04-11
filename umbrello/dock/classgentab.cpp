/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2010                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// my own header
#include "classgentab.h"

// app includes
#include "umlobject.h"
#include "objectwidget.h"
#include "uml.h"
#include "umldoc.h"
#include "artifact.h"
#include "component.h"
#include "umlview.h"
#include "stereotype.h"
#include "umlpackagelist.h"
#include "model_utils.h"
#include "package.h"
#include "folder.h"
#include "import_utils.h"

// kde includes
#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kcombobox.h>
#include <klineedit.h>
#include <ktextedit.h>

// qt includes
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>

ClassGenTab::ClassGenTab(UMLDoc* d, QWidget* parent, UMLObject* o)
  : QWidget(parent), m_pObject(o), m_pWidget(0), m_pInstanceWidget(0), m_pUmldoc(d)
{
    int margin = fontMetrics().height();

    setMinimumSize(310,330);
    QHBoxLayout * topLayout = new QHBoxLayout(this);
    topLayout->setSpacing(6);
    
    QFormLayout * formLayout = new QFormLayout(this);
    topLayout->addItem(formLayout);

    // setup name
    QString name;
    Uml::Object_Type t;
    
    if (o)
      t = o->baseType();
    
    switch (t) {
    case Uml::ot_Class:
        name = i18n("Class &name:");
        break;
    case Uml::ot_Actor:
        name = i18n("Actor &name:");
        break;
    case  Uml::ot_Package:
        name = i18n("Package &name:");
        break;
    case  Uml::ot_UseCase:
        name = i18n("Use case &name:");
        break;
    case  Uml::ot_Interface:
        name = i18n("Interface &name:");
        break;
    case  Uml::ot_Component:
        name = i18n("Component &name:");
        break;
    case  Uml::ot_Node:
        name = i18n("Node &name:");
        break;
    case  Uml::ot_Artifact:
        name = i18n("Artifact &name:");
        break;
    case  Uml::ot_Enum:
        name = i18n("Enum &name:");
        break;
    case  Uml::ot_Datatype:
        name = i18n("Datatype &name:");
        break;
    case  Uml::ot_Entity:
        name = i18n("Entity &name:");
        break;
    default:
        name = i18n("Class &name:");
        uWarning() << "creating class gen page for unknown widget type";
        break;
    }
    QGridLayout * m_pNameLayout = new QGridLayout();
    m_pNameLayout->setSpacing(6);
    topLayout->addLayout(m_pNameLayout, 4);
    m_pNameL = new QLabel(this);
    m_pNameL->setText(name);
    //m_pNameLayout->addWidget(m_pNameL, 0, 0);

    m_pClassNameLE = new KLineEdit(this);
    //m_pNameLayout->addWidget(m_pClassNameLE, 0, 1);
    m_pClassNameLE->setFocus();
    m_pNameL->setBuddy(m_pClassNameLE);
    formLayout->addRow(m_pNameL,m_pClassNameLE);

    m_pStereoTypeCB = 0;
    m_pPackageLE = 0;
    m_pAbstractCB = 0;
    m_pDeconCB = 0;

    m_pStereoTypeL = new QLabel(i18n("&Stereotype name:"), this);
   // m_pNameLayout->addWidget(m_pStereoTypeL, 1, 0);

    m_pStereoTypeCB = new KComboBox(true, this);
    //m_pNameLayout->addWidget(m_pStereoTypeCB, 1, 1);
    formLayout->addRow(m_pStereoTypeL,m_pStereoTypeCB);

    //m_pStereoTypeCB->setItemText( m_pStereoTypeCB->currentIndex(), o->stereotype() ); //TODO ELV
    m_pStereoTypeL->setBuddy(m_pStereoTypeCB);

    if (t == Uml::ot_Interface || t == Uml::ot_Datatype || t == Uml::ot_Enum) {
        m_pStereoTypeCB->setEditable(false);
    }

    //if (t == Uml::ot_Class || t == Uml::ot_Interface) { //TODO ELV
        m_pPackageL = new QLabel(i18n("Package name:"), this);
        //m_pNameLayout->addWidget(m_pPackageL, 2, 0);

        m_pPackageCB = new KComboBox(this);
        m_pPackageCB->setEditable(true);
        //m_pNameLayout->addWidget(m_pPackageCB, 2, 1);

        formLayout->addRow(m_pPackageL,m_pPackageCB);
        
        UMLPackageList packageList = m_pUmldoc->packages();
        QStringList packages;
        foreach( UMLPackage* package, packageList ) {
            packages << package->name();
        }
        packages.sort();
        m_pPackageCB->insertItems(-1, packages);
        //UMLPackage* parentPackage = o->umlPackage(); //TODO ELV

        // if parent package == NULL
        // or if the parent package is the Logical View folder
        /*if ( parentPackage == NULL || parentPackage== static_cast<UMLPackage*>(m_pUmldoc->rootFolder(Uml::mt_Logical)))
            m_pPackageCB->setEditText( QString() );
        else
            m_pPackageCB->setEditText(parentPackage->name());*///TODO ELV
    //}//TODO ELV

    if (t == Uml::ot_Class || t == Uml::ot_UseCase ) {
        QString abstractCaption;
        if ( t == Uml::ot_Class  ) {
            abstractCaption = i18n("A&bstract class");
        } else {
            abstractCaption = i18n("A&bstract use case");
        }
        m_pAbstractCB = new QCheckBox( abstractCaption, this );
        m_pAbstractCB->setChecked( o->isAbstract() );
        m_pNameLayout->addWidget( m_pAbstractCB, 3, 0 );
    }

    if (t == Uml::ot_Component) {
        m_pExecutableCB = new QCheckBox(i18nc("component is executable", "&Executable"), this);
        m_pExecutableCB->setChecked( (static_cast<UMLComponent*>(o))->getExecutable() );
        m_pNameLayout->addWidget( m_pExecutableCB, 3, 0 );
    }

    if (t == Uml::ot_Artifact) {
        // setup artifact draw as
        m_pDrawAsGB = new QGroupBox(i18n("Draw As"), this);
        QHBoxLayout* drawAsLayout = new QHBoxLayout(m_pDrawAsGB);
        drawAsLayout->setMargin(margin);

        m_pDefaultRB = new QRadioButton(i18nc("draw as default", "&Default"), m_pDrawAsGB);
        drawAsLayout->addWidget(m_pDefaultRB);

        m_pFileRB = new QRadioButton(i18n("&File"), m_pDrawAsGB);
        drawAsLayout->addWidget(m_pFileRB);

        m_pLibraryRB = new QRadioButton(i18n("&Library"), m_pDrawAsGB);
        drawAsLayout->addWidget(m_pLibraryRB);

        m_pTableRB = new QRadioButton(i18n("&Table"), m_pDrawAsGB);
        drawAsLayout->addWidget(m_pTableRB);

        topLayout->addWidget(m_pDrawAsGB);

        UMLArtifact::Draw_Type drawAs = (static_cast<UMLArtifact*>(o))->getDrawAsType();

        if (drawAs == UMLArtifact::file) {
            m_pFileRB->setChecked(true);
        } else if (drawAs == UMLArtifact::library) {
            m_pLibraryRB->setChecked(true);
        } else if (drawAs == UMLArtifact::table) {
            m_pTableRB->setChecked(true);
        } else {
            m_pDefaultRB->setChecked(true);
        }
    }

    // setup scope
    
    m_pVisibilityL = new QLabel(this);
    m_pVisibilityL->setText(i18n("Visibility:"));
    
    m_pVisibility = new KComboBox(this);
    QStringList visibility;
    visibility << i18n("P&ublic") << i18n("P&rivate") << i18n("Pro&tected") << i18n("Imple&mentation");
    m_pVisibility->addItems(visibility);
    QHBoxLayout * scopeLayout = new QHBoxLayout(this);
    //topLayout->addWidget(m_pVisibility);
    formLayout->addRow(m_pVisibilityL,m_pVisibility);
    /*m_pButtonGB = new QGroupBox(i18n("Visibility"), this);
    scopeLayout->setMargin(margin);

    m_pPublicRB = new QRadioButton(, m_pButtonGB);

    m_pPrivateRB = new QRadioButton(, m_pButtonGB);
    scopeLayout->addWidget(m_pPrivateRB);

    m_pProtectedRB = new QRadioButton(, m_pButtonGB);
    scopeLayout->addWidget(m_pProtectedRB);
    topLayout->addWidget(m_pButtonGB);

    m_pImplementationRB = new QRadioButton(, m_pButtonGB);
    scopeLayout->addWidget(m_pImplementationRB);
    topLayout->addWidget(m_pButtonGB);*/

    // setup documentation
    m_pDocGB = new QGroupBox(this);
    QHBoxLayout * docLayout = new QHBoxLayout(m_pDocGB);
    docLayout->setMargin(margin);
    m_pDocGB->setTitle(i18n("Documentation"));

    m_pDoc = new KTextEdit(m_pDocGB);
    docLayout->addWidget(m_pDoc);
    topLayout->addWidget(m_pDocGB);

    // setup fields
    //m_pClassNameLE->setText(o->name()); //TODO ELV
    //m_pDoc->setText(o->doc());//TODO ELV
    if (o) { //TODO ELV PORT TO FUNCTION
      Uml::Visibility s = o->visibility();
      if (s == Uml::Visibility::Public)
          m_pVisibility->setCurrentIndex(ClassGenTab::Public);
      else if (s == Uml::Visibility::Private)
          m_pVisibility->setCurrentIndex(ClassGenTab::Private);
      else if (s == Uml::Visibility::Protected)
            m_pVisibility->setCurrentIndex(ClassGenTab::Protected);
      else
          m_pVisibility->setCurrentIndex(ClassGenTab::Implementation);
    }

    // manage stereotypes
    m_pStereoTypeCB->setDuplicatesEnabled(false);  // only allow one of each type in box
    m_pStereoTypeCB->setCompletionMode( KGlobalSettings::CompletionPopup );
    if (m_pObject) //TODO ELV
      insertStereotypesSorted(m_pObject->stereotype());

    m_pDoc->setLineWrapMode(QTextEdit::WidgetWidth);
}

ClassGenTab::ClassGenTab(UMLDoc* d, QWidget* parent, ObjectWidget* o)
  : QWidget(parent), m_pObject(0), m_pWidget(o), m_pInstanceWidget(0), m_pUmldoc(d)
{
    m_pDeconCB = 0;
    m_pMultiCB = 0;

    int margin = fontMetrics().height();

    setMinimumSize(310,330);
    QGridLayout * topLayout = new QGridLayout(this);
    topLayout->setSpacing(6);

    // setup name
    QGridLayout * m_pNameLayout = new QGridLayout();
    m_pNameLayout->setSpacing(6);
    topLayout->addLayout(m_pNameLayout, 3, 2);
    m_pNameL = new QLabel(this);
    m_pNameL->setText(i18n("Class name:"));
    m_pNameLayout->addWidget(m_pNameL, 0, 0);

    m_pClassNameLE = new KLineEdit(this);
    m_pClassNameLE->setText(o->name());
    m_pNameLayout->addWidget(m_pClassNameLE, 0, 1);

    m_pInstanceL = new QLabel(this);
    m_pInstanceL->setText(i18n("Instance name:"));
    m_pNameLayout->addWidget(m_pInstanceL, 1, 0);

    m_pInstanceLE = new KLineEdit(this);
    m_pInstanceLE->setText(o->instanceName());
    m_pNameLayout->addWidget(m_pInstanceLE, 1, 1);
    UMLView *view = UMLApp::app()->currentView();

    m_pDrawActorCB = new QCheckBox( i18n( "Draw as actor" ) , this );
    m_pDrawActorCB->setChecked( o->drawAsActor() );
    m_pNameLayout->addWidget( m_pDrawActorCB, 2, 0 );

    if (view->getType() == Uml::dt_Collaboration) {
        m_pMultiCB = new QCheckBox(i18n("Multiple instance"), this);
        m_pMultiCB->setChecked(o->multipleInstance());
        m_pNameLayout->addWidget(m_pMultiCB, 2,1);
        if ( m_pDrawActorCB->isChecked() )
            m_pMultiCB->setEnabled( false );
    } else {  // sequence diagram
        m_pDeconCB = new QCheckBox(i18n("Show destruction"), this);
        m_pDeconCB->setChecked(o->showDestruction());
        m_pNameLayout->addWidget(m_pDeconCB, 2,1);
    }
    // setup documentation
    m_pDocGB = new QGroupBox(this);
    topLayout->addWidget(m_pDocGB, 1, 0);
    QHBoxLayout * docLayout = new QHBoxLayout(m_pDocGB);
    docLayout->setMargin(margin);
    m_pDocGB->setTitle(i18n("Documentation"));

    m_pDoc = new KTextEdit(m_pDocGB);
    m_pDoc->setLineWrapMode(QTextEdit::WidgetWidth);
    m_pDoc->setText(o->documentation());
    docLayout->addWidget(m_pDoc);
    if (m_pMultiCB) {
        connect( m_pDrawActorCB, SIGNAL( toggled( bool ) ), this, SLOT( slotActorToggled( bool ) ) );
    }
}

ClassGenTab::ClassGenTab(UMLDoc* d, QWidget* parent, UMLWidget* widget)
    : QWidget(parent), m_pObject(0), m_pWidget(0), m_pInstanceWidget(widget), m_pUmldoc(d)
{
    m_pDeconCB = 0;
    m_pMultiCB = 0;

    int margin = fontMetrics().height();

    setMinimumSize(310,330);
    QGridLayout* topLayout = new QGridLayout(this);
    topLayout->setSpacing(6);

    //setup name
    QGridLayout* m_pNameLayout = new QGridLayout();
    m_pNameLayout->setSpacing(6);
    topLayout->addLayout(m_pNameLayout, 3, 2);
    m_pNameL = new QLabel(this);
    if (widget->baseType() == Uml::wt_Component) {
        m_pNameL->setText(i18n("Component name:"));
    } else if (widget->baseType() == Uml::wt_Node) {
        m_pNameL->setText(i18n("Node name:"));
    } else {
        uWarning() << "ClassGenTab called on unknown widget type";
    }
    m_pNameLayout->addWidget(m_pNameL, 0, 0);

    m_pClassNameLE = new KLineEdit(this);
    m_pClassNameLE->setText(widget->name());
    m_pNameLayout->addWidget(m_pClassNameLE, 0, 1);

    m_pStereoTypeL = new QLabel(i18n("Stereotype name:"), this);
    m_pNameLayout->addWidget(m_pStereoTypeL, 1, 0);

    m_pStereoTypeCB = new KComboBox(true, this);
    m_pNameLayout->addWidget(m_pStereoTypeCB, 1, 1);

    m_pStereoTypeCB->setItemText( m_pStereoTypeCB->currentIndex(), widget->umlObject()->stereotype() );
    m_pStereoTypeCB->setCompletionMode( KGlobalSettings::CompletionPopup );

    m_pInstanceL = new QLabel(this);
    m_pInstanceL->setText(i18n("Instance name:"));
    m_pNameLayout->addWidget(m_pInstanceL, 2, 0);

    m_pInstanceLE = new KLineEdit(this);
    m_pInstanceLE->setText(widget->instanceName());
    m_pNameLayout->addWidget(m_pInstanceLE, 2, 1);

    //setup documentation
    m_pDocGB = new QGroupBox(this);
    topLayout->addWidget(m_pDocGB, 1, 0);
    QHBoxLayout* docLayout = new QHBoxLayout(m_pDocGB);
    docLayout->setMargin(margin);
    m_pDocGB->setTitle(i18n("Documentation"));

    m_pDoc = new KTextEdit(m_pDocGB);
    m_pDoc->setLineWrapMode(QTextEdit::WidgetWidth);
    m_pDoc->setText(widget->documentation());
    docLayout->addWidget(m_pDoc);
}

ClassGenTab::~ClassGenTab()
{
}

void ClassGenTab::insertStereotypesSorted(const QString& type)
{
    if (!&type)
      return;
    QStringList types;
    types << "";  // an empty stereotype is the default
    foreach (UMLStereotype* ust, m_pUmldoc->stereotypes()) {
        types << ust->name();
    }
    // add the given parameter
    if ( !types.contains(type) ) {
        types << type;
    }
    types.sort();

    m_pStereoTypeCB->clear();
    m_pStereoTypeCB->insertItems(-1, types);

    // select the given parameter
    int currentIndex = m_pStereoTypeCB->findText(type);
    if (currentIndex > -1) {
        m_pStereoTypeCB->setCurrentIndex(currentIndex);
    }
    m_pStereoTypeCB->completionObject()->addItem( type );
}

/**
 * Will move information from the dialog into the object.
 * Call when the ok or apply button is pressed.
 */
void ClassGenTab::updateObject()
{
    if (m_pObject) {
        QString name = m_pClassNameLE->text();

        m_pObject->setDoc(m_pDoc->toPlainText());

        if (m_pStereoTypeCB) {
            m_pObject->setStereotype(m_pStereoTypeCB->currentText());
        }

        Uml::Object_Type t = m_pObject->baseType();
        if (t == Uml::ot_Class || t == Uml::ot_Interface) {
            QString packageName = m_pPackageCB->currentText().trimmed();
            UMLObject* newPackage = NULL;

            if ( !packageName.isEmpty()) {
                if ( ( newPackage = m_pUmldoc->findUMLObject(packageName, Uml::ot_Package) ) == NULL ) {
                    newPackage = Import_Utils::createUMLObject(Uml::ot_Package, packageName);
                }
            } else {
                newPackage = m_pUmldoc->rootFolder( Uml::mt_Logical );
            }

            // adjust list view items
            Model_Utils::treeViewMoveObjectTo(newPackage, m_pObject);
        }

        if ( m_pAbstractCB ) {
            m_pObject->setAbstract( m_pAbstractCB->isChecked() );
        }

        //make sure unique name
        UMLObject *o = m_pUmldoc->findUMLObject(name);
        if (o && m_pObject != o) {
             KMessageBox::sorry(this, i18n("The name you have chosen\nis already being used.\nThe name has been reset."),
                                i18n("Name is Not Unique"), false);
             m_pClassNameLE->setText( m_pObject->name() );
        } else {
             m_pObject->setName(name);
        }

        Uml::Visibility s;
        if (m_pVisibility->currentIndex() == ClassGenTab::Public)
            s = Uml::Visibility::Public;
        else if (m_pVisibility->currentIndex() == ClassGenTab::Private)
            s = Uml::Visibility::Private;
        else if (m_pVisibility->currentIndex() == ClassGenTab::Protected)
            s = Uml::Visibility::Protected;
        else
            s = Uml::Visibility::Implementation;
        m_pObject->setVisibility(s);

        if (m_pObject->baseType() == Uml::ot_Component) {
            (static_cast<UMLComponent*>(m_pObject))->setExecutable( m_pExecutableCB->isChecked() );
        }

        if (m_pObject->baseType() == Uml::ot_Artifact) {
            UMLArtifact::Draw_Type drawAsType;
            if ( m_pFileRB->isChecked() ) {
                drawAsType = UMLArtifact::file;
            } else if ( m_pLibraryRB->isChecked() ) {
                drawAsType = UMLArtifact::library;
            } else if (m_pTableRB->isChecked() ) {
                drawAsType = UMLArtifact::table;
            } else {
                drawAsType = UMLArtifact::defaultDraw;
            }
            (static_cast<UMLArtifact*>(m_pObject))->setDrawAsType(drawAsType);
            m_pObject->emitModified();
        }
    } // end if m_pObject
    else if (m_pWidget) {
        m_pWidget->setInstanceName(m_pInstanceLE->text());
        if (m_pMultiCB) {
            m_pWidget->setMultipleInstance(m_pMultiCB->isChecked());
        }
        m_pWidget->setDrawAsActor( m_pDrawActorCB->isChecked() );
        if (m_pDeconCB) {
            m_pWidget->setShowDestruction( m_pDeconCB->isChecked() );
        }
        QString name = m_pClassNameLE->text();
        m_pWidget->setDocumentation(m_pDoc->toPlainText());
        UMLObject * o = m_pWidget->umlObject();
        UMLObject * old = m_pUmldoc->findUMLObject(name);
        if (old && o != old) {
            KMessageBox::sorry(this, i18n("The name you have chosen\nis already being used.\nThe name has been reset."),
                               i18n("Name is Not Unique"), false);
        } else {
            o->setName(name);
        }
    } // end if m_pWidget
    else if (m_pInstanceWidget) {
        m_pInstanceWidget->setInstanceName(m_pInstanceLE->text());
        QString name = m_pClassNameLE->text();
        m_pInstanceWidget->setDocumentation(m_pDoc->toPlainText());
        UMLObject* o = m_pInstanceWidget->umlObject();
        UMLObject* old = m_pUmldoc->findUMLObject(name);
        if (old && o != old) {
            KMessageBox::sorry(this, i18n("The name you have chosen\nis already being used.\nThe name has been reset."),
                               i18n("Name is Not Unique"), false);
        } else {
            o->setName(name);
        }
        o->setStereotype( m_pStereoTypeCB->currentText() );
    } // end if m_pInstanceWidget
}

/**
 * When the draw as actor check box is toggled, the draw
 * as multi instance need to be enabled/disabled. They
 * both can't be available at the same time.
 */
void ClassGenTab::slotActorToggled( bool state )
{
    if (m_pMultiCB) {
        m_pMultiCB->setEnabled( !state );
    }
}

#include "classgentab.moc"
