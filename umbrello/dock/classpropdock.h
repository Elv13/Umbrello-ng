/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2010                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef CLASSPROPDOCK_H
#define CLASSPROPDOCK_H

// qt class includes
#include <QtGui/QFrame>
#include <QtGui/QDockWidget>
// kde class includes
#include <kpagedialog.h>
#include <kfontdialog.h>
#include <KDialog>

#include "umlnamespace.h"
#include "icon_utils.h"

class ClassGenTab;
class ClassifierListTab;
class ConstraintListPage;
class PkgContentsPage;
class AssocPage;
class ClassOptionsTab;
class UMLWidgetColorPage;

class ObjectWidget;
class UMLDoc;
class UMLObject;
class UMLWidget;

/**
 * @author Paul Hensgen <phensgen@techie.com>
 * @version 1.0
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class ClassPropDock : public QDockWidget
{
    Q_OBJECT
public:
    ClassPropDock();
    ClassPropDock(QWidget *parent, UMLObject *c, bool assoc = false);
    ClassPropDock(QWidget *parent, ObjectWidget * o);
    ClassPropDock(QWidget *parent, UMLWidget * o);
    
    void setObject(UMLObject * c, bool assoc = false);
    void setObjectWidget(ObjectWidget *o);
    void setUMLWidget(UMLWidget *w);
    
    void updatePages();

    ~ClassPropDock();

    enum Page{page_gen = 0, page_att, page_op, page_entatt, page_constraint , page_template,
              page_assoc, page_options, page_color, page_font};

protected slots:
    void slotOk();
    void slotApply();

protected:
    void setupPages(bool assoc = false);
    void setupGeneralPage();
    void setupColorPage();
    void setupDisplayPage();
    void setupAttributesPage();
    void setupOperationsPage();
    void setupTemplatesPage();
    void setupEnumLiteralsPage();
    void setupEntityAttributesPage();
    void setupEntityConstraintsPage();
    void setupContentsPage();
    void setupAssociationsPage();
    void setupInstancePages();
    void setupFontPage();

private:
    KFontChooser*        m_pChooser;
    ClassGenTab*         m_pGenPage;
    ClassifierListTab*   m_pAttPage;
    ClassifierListTab*   m_pOpsPage;
    ClassifierListTab*   m_pTemplatePage;
    ClassifierListTab*   m_pEnumLiteralPage;
    ClassifierListTab*   m_pEntityAttributePage;
    ConstraintListPage*  m_pEntityConstraintPage;
    PkgContentsPage*     m_pPkgContentsPage;
    AssocPage*           m_pAssocPage;
    ClassOptionsTab*    m_pOptionsPage;
    UMLWidgetColorPage*  m_pColorPage;
    QTabWidget*          m_pTabWidget;

    UMLDoc*              m_pDoc;
    UMLObject*           m_pObject;
    UMLWidget*           m_pWidget;

    enum Page_Type {
        pt_Object = 1,    // Show General page + Assoc. page if Class i.e. no colours page
        pt_ObjectWidget,  // Shows pages needed for an ObjectWidget
        pt_Widget         // Shows pages needed for any other widget
    };

    Page_Type m_Type;

private:
    void init();

    //QFrame* createPage(const QString& name, const QString& header, Icon_Utils::Icon_Type icon);

};

#endif
