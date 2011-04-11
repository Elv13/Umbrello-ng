/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2009                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "activitydialog.h"

//local includes
#include "umlview.h"
#include "activitywidget.h"
#include "dialog_utils.h"
#include "icon_utils.h"

//kde includes
#include <kvbox.h>
#include <klocale.h>

//qt includes
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>

ActivityDialog::ActivityDialog( UMLView * pView, ActivityWidget * pWidget )
        : KPageDialog(pView)
{
    setCaption( i18n("Properties") );
    setButtons( Ok | Apply | Cancel | Help );
    setDefaultButton( Ok );
    setModal( true );
    setFaceType( KPageDialog::List );
    showButtonSeparator( true );
    m_pView = pView;
    m_pActivityWidget = pWidget;
    m_bChangesMade = false;
    setupPages();
    connect(this,SIGNAL(okClicked()),this,SLOT(slotOk()));
    connect(this,SIGNAL(applyClicked()),this,SLOT(slotApply()));
}

/**
 *   Entered when OK button pressed.
 */
void ActivityDialog::slotOk()
{
    applyPage( pageItemColor );
    applyPage( pageItemFont );
    applyPage( pageItemGeneral );
    accept();
}

/**
 *   Entered when Apply button pressed.
 */
void ActivityDialog::slotApply()
{
    applyPage(currentPage());
}

void ActivityDialog::slotShowActivityParameter()
{
    m_GenPageWidgets.preL->show();
    m_GenPageWidgets.preLE->show();
    m_GenPageWidgets.postL->show();
    m_GenPageWidgets.postLE->show();
    if(!m_pActivityWidget->postconditionText().isEmpty()) {
        m_GenPageWidgets.postLE->setText(m_pActivityWidget->postconditionText());
    }
    if (!m_pActivityWidget->preconditionText().isEmpty()) {
        m_GenPageWidgets.preLE->setText(m_pActivityWidget->preconditionText());
    }
}

void ActivityDialog::slotHideActivityParameter()
{
    m_GenPageWidgets.preL->hide();
    m_GenPageWidgets.preLE->hide();
    m_GenPageWidgets.postL->hide();
    m_GenPageWidgets.postLE->hide();
}

/**
 *   Sets up the pages of the dialog.
 */
void ActivityDialog::setupPages()
{
    setupGeneralPage();
    setupColorPage();
    setupFontPage();
}

/**
 *     Applies changes to the given page.
 */
void ActivityDialog::applyPage( KPageWidgetItem *item )
{
    m_bChangesMade = true;
    if ( item == pageItemGeneral )
    {
        m_pActivityWidget->setName( m_GenPageWidgets.nameLE->text() );
        m_pActivityWidget->setDocumentation( m_GenPageWidgets.docTE->toPlainText() );
        m_pActivityWidget->setPreconditionText( m_GenPageWidgets.preLE->text() );
        m_pActivityWidget->setPostconditionText( m_GenPageWidgets.postLE->text() );

        ActivityWidget::ActivityType newType = ActivityWidget::Normal;
        if ( m_GenPageWidgets.InvokRB->isChecked() )
              newType = ActivityWidget::Invok;
        else if ( m_GenPageWidgets.ParamRB->isChecked() )
              newType = ActivityWidget::Param;
        m_pActivityWidget->setActivityType (newType);

    }
    else if ( item == pageItemFont )
    {
        m_pActivityWidget->setFont( m_pChooser->font() );
    }
    else if ( item == pageItemColor )
    {
        m_pColorPage->updateUMLWidget();
    }
}

/**
 *   Sets up the general page of the dialog.
 */
void ActivityDialog::setupGeneralPage()
{
    QString types[ ] = { i18n("Initial activity"), i18n("Activity"), i18n("End activity"), i18n("Final activity"), i18n( "Branch/Merge"), i18n( "Invoke action"), i18n("Parameter activity") };
    ActivityWidget::ActivityType type = m_pActivityWidget->activityType();

    KVBox *page = new KVBox();
    pageItemGeneral = new KPageWidgetItem( page, i18nc("general properties page", "General") );
    pageItemGeneral->setHeader(i18n("General Properties"));
    pageItemGeneral->setIcon( Icon_Utils::DesktopIcon(Icon_Utils::it_Properties_General) );
    addPage( pageItemGeneral );

    m_GenPageWidgets.generalGB = new QGroupBox( i18n( "Properties"), (QWidget *)page );

    QGridLayout * generalLayout = new QGridLayout( m_GenPageWidgets.generalGB );
    generalLayout->setSpacing( spacingHint() );
    generalLayout->setMargin(  fontMetrics().height()  );

    QString actType ( types[ (int)type ] );
    Dialog_Utils::makeLabeledEditField( m_GenPageWidgets.generalGB, generalLayout, 0,
                                    m_GenPageWidgets.typeL, i18n("Activity type:"),
                                    m_GenPageWidgets.typeLE, actType );
    m_GenPageWidgets.typeLE->setEnabled( false );

    Dialog_Utils::makeLabeledEditField( m_GenPageWidgets.generalGB, generalLayout, 1,
                                    m_GenPageWidgets.nameL, i18n("Activity name:"),
                                    m_GenPageWidgets.nameLE );

    Dialog_Utils::makeLabeledEditField( m_GenPageWidgets.generalGB, generalLayout, 2,
                                    m_GenPageWidgets.preL, i18n("Precondition :"),
                                    m_GenPageWidgets.preLE );

    Dialog_Utils::makeLabeledEditField( m_GenPageWidgets.generalGB, generalLayout, 3,
                                    m_GenPageWidgets.postL, i18n("Postcondition :"),
                                    m_GenPageWidgets.postLE );
    m_GenPageWidgets.preL->hide();
    m_GenPageWidgets.preLE->hide();
    m_GenPageWidgets.postL->hide();
    m_GenPageWidgets.postLE->hide();

    m_GenPageWidgets.NormalRB = new QRadioButton( i18n("&Normal activity"),(QWidget *)page);
    generalLayout->addWidget( m_GenPageWidgets.NormalRB );

    m_GenPageWidgets.InvokRB = new QRadioButton( i18n("&Invoke action "),(QWidget *)page);
    generalLayout->addWidget( m_GenPageWidgets.InvokRB );

    m_GenPageWidgets.ParamRB = new QRadioButton( i18n("&Parameter activity node"),(QWidget *)page);
    generalLayout->addWidget( m_GenPageWidgets.ParamRB );

    if (type == ActivityWidget::Param)
    {
        showParameterActivity();
    }

    connect(m_GenPageWidgets.ParamRB,SIGNAL(clicked()),this,SLOT(slotShowActivityParameter()));
    connect(m_GenPageWidgets.NormalRB,SIGNAL(clicked()),this,SLOT(slotHideActivityParameter()));
    connect(m_GenPageWidgets.InvokRB,SIGNAL(clicked()),this,SLOT(slotHideActivityParameter()));

    ActivityWidget::ActivityType newType = m_pActivityWidget->activityType() ;

    m_GenPageWidgets.NormalRB->setChecked(newType == ActivityWidget::Normal);

    m_GenPageWidgets.InvokRB->setChecked (newType == ActivityWidget::Invok);

    m_GenPageWidgets.ParamRB->setChecked (newType == ActivityWidget::Param);

    m_GenPageWidgets.docGB = new QGroupBox( i18n( "Documentation"), (QWidget *)page );

    QHBoxLayout * docLayout = new QHBoxLayout( m_GenPageWidgets.docGB );
    docLayout->setSpacing( spacingHint() );
    docLayout->setMargin(  fontMetrics().height()  );

    m_GenPageWidgets.docTE = new KTextEdit( m_GenPageWidgets.docGB );
    m_GenPageWidgets.docTE->setText( m_pActivityWidget->documentation() );
    docLayout->addWidget( m_GenPageWidgets.docTE );

    if ( type != ActivityWidget::Normal && type != ActivityWidget::Invok && type != ActivityWidget::Param) {
        m_GenPageWidgets.nameLE->setEnabled( false );
        m_GenPageWidgets.nameLE->setText( "" );
    } else
        m_GenPageWidgets.nameLE->setText( m_pActivityWidget->name() );
}

/**
 *   Sets up the font selection page.
 */
void ActivityDialog::setupFontPage()
{
    KVBox *page = new KVBox();
    pageItemFont = new KPageWidgetItem( page, i18n("Font") );
    pageItemFont->setHeader( i18n("Font Settings") );
    pageItemFont->setIcon( Icon_Utils::DesktopIcon(Icon_Utils::it_Properties_Font) );
    addPage( pageItemFont );
    m_pChooser = new KFontChooser( (QWidget*)page, false, QStringList(), false);
    m_pChooser->setFont( m_pActivityWidget->font() );
}

/**
 *   Show the Activity Parameter entry text.
 */
void ActivityDialog::showParameterActivity()
{
    m_GenPageWidgets.preL->show();
    m_GenPageWidgets.preLE->show();
    m_GenPageWidgets.postL->show();
    m_GenPageWidgets.postLE->show();
    if (!m_pActivityWidget->postconditionText().isEmpty()) {
        m_GenPageWidgets.postLE->setText(m_pActivityWidget->postconditionText());
    }
    if (!m_pActivityWidget->preconditionText().isEmpty()) {
        m_GenPageWidgets.preLE->setText(m_pActivityWidget->preconditionText());
    }
}

/**
 *   Sets up the color page.
 */
void ActivityDialog::setupColorPage()
{
    QFrame *colorPage = new QFrame();
    pageItemColor = new KPageWidgetItem( colorPage, i18nc("widget color page", "Color") );
    pageItemColor->setHeader( i18n("Widget Colors") );
    pageItemColor->setIcon( Icon_Utils::DesktopIcon(Icon_Utils::it_Properties_Color) );
    addPage( pageItemColor );
    QHBoxLayout * m_pColorLayout = new QHBoxLayout(colorPage);
    m_pColorPage = new UMLWidgetColorPage( colorPage, m_pActivityWidget );
    m_pColorLayout->addWidget(m_pColorPage);
}

#include "activitydialog.moc"
