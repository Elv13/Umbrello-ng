/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2009                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef CLASSIFIERLISTTAB_H
#define CLASSIFIERLISTTAB_H

//app includes
#include "umlnamespace.h"
#include "umlclassifierlistitemlist.h"

//qt  includes
#include <QtGui/QWidget>

class CodeTextEdit;
class ListPopupMenu;
class UMLObject;
class UMLClassifier;
class UMLDoc;
class KTextEdit;
class QGroupBox;
class QHBoxLayout;
class QListWidget;
class QListWidgetItem;
class QTableWidget;
class QPushButton;
class QToolButton;
class QVBoxLayout;

class RowWidgetController;

/**
 * A dialog page to display classifier list properties.  This is not normally setup
 * by you.  It is used by the @ref ClassPropDlg and new class wizard.
 *
 * @short A dialog page to display classifier properties.
 * @author Paul Hensgen, Jonathan Riddell
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class ClassifierListTab : public QWidget
{
    Q_OBJECT
public:

    ClassifierListTab(QWidget* parent, UMLClassifier* classifier, UMLDoc* doc, Uml::Object_Type type);
    virtual ~ClassifierListTab();
    
    void setClassifier(UMLClassifier* classifier);

    void updateObject();

private:

    void setupPage();

    void setupListGroup(int margin);
    void setupDocumentationGroup(int margin);
    void setupMoveButtons(QHBoxLayout* parentLayout);
    void setupActionButtons(const QString& itemType, QVBoxLayout* parentLayout);

    
    void enableWidgets(bool state);

    void saveCurrentItemDocumentation();

    UMLClassifierListItemList getItemList();

    bool addClassifier(UMLClassifierListItem* classifier, int position = -1);

    bool takeItem(UMLClassifierListItem* listitem,
                  bool seekPeerBefore, int &peerIndex);

    void deleteMenu();

    void printItemList(const QString &prologue);

    QGroupBox*     m_pDocGB;
    QGroupBox*     m_pItemListGB;
    KTextEdit*     m_pDocTE;
    CodeTextEdit*  m_pCodeTE;
    QListWidget*   m_pItemListLB;
    QTableWidget*  m_centralTableTW;

    QToolButton*   m_pTopArrowB;
    QToolButton*   m_pUpArrowB;
    QToolButton*   m_pDownArrowB;
    QToolButton*   m_pBottomArrowB;

    UMLDoc*        m_pDoc;
    ListPopupMenu* m_pMenu;
    UMLClassifierListItem* m_pOldListItem;
    
    uint m_rowCount;
    
    QList<RowWidgetController*> rowList;

protected:

    void reloadItemListBox();

    void hideArrowButtons(bool hide);
    
    bool eventFilter(QObject *o, QEvent *e);

    Uml::Object_Type m_itemType;
    UMLClassifier* m_pClassifier;
    UMLClassifierListItem* m_pLastObjectCreated;

    QPushButton* m_pNewClassifierListItemButton;
    QPushButton* m_pDeleteListItemButton;
    QPushButton* m_pPropertiesButton;

    bool m_bSigWaiting;

protected slots:

    void slotActivateItem(QListWidgetItem* item);

    void slotPopupMenuSel(QAction* action);

    void slotListItemCreated(UMLObject* object);
    void slotListItemModified();
    void slotRightButtonPressed(const QPoint& p);

    void slotDoubleClick(QListWidgetItem* item);

    void slotTopClicked();
    void slotUpClicked();
    void slotDownClicked();
    void slotBottomClicked();

    void slotNewListItem();

    void slotDelete();

    void slotProperties();
    
    void itemChanged(int row, int col);
    
    RowWidgetController* addRow(bool ignoreModified = false);
    void addEmtpyRow();
    
    void slotSelectLastRow();
};

#endif
