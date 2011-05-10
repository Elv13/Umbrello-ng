/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef CLASSGENTAB_H
#define CLASSGENTAB_H

#include <QtGui/QWidget>

class QGroupBox;
class QLabel;
class QRadioButton;
class QCheckBox;
class KComboBox;
class KLineEdit;
class KTextEdit;
class KColorButton;

class UMLObject;
class UMLWidget;
class ObjectWidget;
class UMLDoc;
class KFontComboBox;



/**
 * Displays properties of a UMLObject in a dialog box.  This is not usually directly
 * called.  The class @ref ClassPropDlg will set this up for you.
 *
 * @short Display properties on a UMLObject.
 * @author Paul Hensgen <phensgen@techie.com>
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class ClassGenTab : public QWidget
{
    Q_OBJECT
public:

    /**
     * Sets up the ClassGenTab.
     * @param  d       The UMLDoc which controls controls object creation.
     * @param  parent  The parent to the ClassGenTab.
     * @param  o       The UMLObject to display the properties of.
     */
    ClassGenTab(UMLDoc *d, QWidget *parent, UMLObject * o);

    /**
     * Sets up the ClassGenTab for an ObjectWidget
     * @param  d       The UMLDoc which controls controls object creation.
     * @param  parent  The parent to the ClassGenTab.
     * @param  o       The ObjectWidget to display the properties of.
     */
    ClassGenTab(UMLDoc *d, QWidget *parent, ObjectWidget * o);

    /**
     *  Sets up the ClassGenTab for a UMLWidget instance (used
     *  for component instances on deployment diagrams)
     *
     *  @param  d       The UMLDoc which controls controls object creation.
     *  @param  parent  The parent to the ClassGenTab.
     *  @param  widget  The UMLWidget to display the properties of.
     */
    ClassGenTab(UMLDoc* d, QWidget* parent, UMLWidget* widget);

    /**
     * Standard deconstructor.
     */
    ~ClassGenTab();
    
    void setObject(UMLObject * o);
    void setObjectWidget(ObjectWidget *o);
    void setUMLWidget(UMLWidget *w);

    void updateObject();
    
    void focusName();

private:
  enum Visibility { //TODO ELV remove
      Public,
      Private,
      Protected,
      Implementation
    };
    UMLObject * m_pObject;
    ObjectWidget * m_pWidget;
    UMLWidget * m_pInstanceWidget;
    UMLDoc * m_pUmldoc;

    QGroupBox * m_pDocGB, * m_pInfGB;
    QLabel * m_pNameL, * m_pInstanceL, * m_pStereoTypeL, * m_pPackageL, * m_pVisibilityL, * m_pBorderL, * m_pBackgroundL, * m_pFontL;
    KLineEdit * m_pClassNameLE, * m_pInstanceLE, * m_pPackageLE;
    KComboBox * m_pStereoTypeCB, * m_pPackageCB, * m_pVisibility;
    KColorButton * m_pBorderColor, * m_pBackgroundColor;
    KFontComboBox * m_pFontCombo;
    QCheckBox * m_pMultiCB, * m_pDrawActorCB, * m_pAbstractCB, * m_pDeconCB;
    KTextEdit * m_pDoc;
    QGroupBox * m_pDrawAsGB;
    QRadioButton * m_pDefaultRB, * m_pFileRB, * m_pLibraryRB, * m_pTableRB;
    QCheckBox * m_pExecutableCB;

    void insertStereotypesSorted(const QString& type);
    void resetTab();
    
    enum Page_Type {
        pt_Unset = 1, //Not selected yet
        pt_Object,    // Show General page + Assoc. page if Class i.e. no colours page
        pt_ObjectWidget,  // Shows pages needed for an ObjectWidget
        pt_Widget         // Shows pages needed for any other widget
    };
    
    Page_Type m_Type;

public slots:

    void slotActorToggled( bool state );
};

#endif
