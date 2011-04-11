/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2009                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef UMLLISTVIEWITEM_H
#define UMLLISTVIEWITEM_H

#include <q3listview.h>
#include <QtCore/QMap>
#include <qdom.h>

#include "umlnamespace.h"
#include "icon_utils.h"

// forward declarations
class UMLListView;
class UMLObject;
class UMLClassifierListItem;

/**
 * Items used by the class @ref UMLListView.  This is needed as the type
 * and object information is required to be stored.
 *
 * @short  Items used by @ref UMLListView.
 * @author Paul Hensgen <phensgen@techie.com>
 * @see    UMLListView
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class UMLListViewItem : public Q3ListViewItem
{
public:

    UMLListViewItem(UMLListView * parent, const QString &name, Uml::ListView_Type t, UMLObject*o=0);
    UMLListViewItem(UMLListView * parent);
    UMLListViewItem(UMLListViewItem * parent);
    UMLListViewItem(UMLListViewItem * parent, const QString &name, Uml::ListView_Type t, UMLObject*o=0);
    UMLListViewItem(UMLListViewItem * parent, const QString &name, Uml::ListView_Type t, Uml::IDType id);

    ~UMLListViewItem();

    Uml::ListView_Type getType() const;

    void setID(Uml::IDType id);
    Uml::IDType getID() const;

    void setUMLObject(UMLObject * obj);
    UMLObject * getUMLObject() const;

    bool isOwnParent(Uml::IDType listViewItemID);

    void updateObject();
    void updateFolder();

    void setOpen( bool open );

    void setText(int column, const QString &text);
    void setText(const QString &text );
    QString getText() const;

    void setCreating( bool creating );

    void setIcon(Icon_Utils::Icon_Type iconType);

    void cancelRename( int col );

    void addClassifierListItem(UMLClassifierListItem *child, UMLListViewItem *childItem);

    void deleteChildItem(UMLClassifierListItem *child);

    virtual int compare(Q3ListViewItem *other, int col, bool ascending) const;

    int childCount() const;

    UMLListViewItem* deepCopy(UMLListViewItem *newParent);

    UMLListViewItem* findUMLObject(const UMLObject *o);
    UMLListViewItem* findChildObject(UMLClassifierListItem *cli);
    UMLListViewItem* findItem(Uml::IDType id);

    void saveToXMI( QDomDocument& qDoc, QDomElement& qElement);
    bool loadFromXMI(QDomElement& qElement);

protected:
    void init(UMLListView * parent = 0);

    void okRename( int col );

    void cancelRenameWithMsg();

    /**
     * This list view all the instance of this class are displayed on.
     */
    static UMLListView * s_pListView;

    /**
     * Flag used to set the state of creating.
     */
    bool m_bCreating;

    /**
     * Auxiliary map of child UMLLisViewItems keyed by UMLClassifierListItem.
     * Used by findChildObject() for efficiency instead of looping using
     * firstChild()/nextSibling() because the latter incur enforceItemVisible()
     * and thus expensive sorting.
     */
    typedef QMap<UMLClassifierListItem*, UMLListViewItem*> ChildObjectMap;

    Uml::ListView_Type m_Type;
    Uml::IDType m_nId;
    int m_nChildren;
    UMLObject * m_pObject;
    QString m_Label;
    ChildObjectMap m_comap;

};

#endif
