/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2010                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "umlclipboard.h"

// qt/kde includes
#include <QtGui/QPixmap>
#include <QtCore/QMimeData>

#include <kdebug.h>
#include <kmessagebox.h>
#include <klocale.h>

// local includes
#include "umldragdata.h"
#include "idchangelog.h"
#include "associationwidget.h"
#include "attribute.h"
#include "classifier.h"
#include "enum.h"
#include "entity.h"
#include "floatingtextwidget.h"
#include "operation.h"
#include "template.h"
#include "enumliteral.h"
#include "entityattribute.h"
#include "umldoc.h"
#include "umllistview.h"
#include "umllistviewitem.h"
#include "umlobjectlist.h"
#include "umlview.h"
#include "umlwidget.h"
#include "uml.h"
#include "model_utils.h"

/**
 * Constructor.
 */
UMLClipboard::UMLClipboard()
{
    m_type = clip1;
}

/**
 * Deconstructor.
 */
UMLClipboard::~UMLClipboard()
{
}

/**
 * Copy operation.
 * @param fromView   flag if it is from view
 * @return           the mime data
 */
QMimeData* UMLClipboard::copy(bool fromView/*=false*/)
{
    //Clear previous copied data
    m_AssociationList.clear();
    m_ItemList.clear();
    m_ObjectList.clear();
    m_ViewList.clear();

    UMLDragData *data = 0;
    QPixmap* png = 0;

    UMLListView * listView = UMLApp::app()->listView();
    UMLListViewItemList selectedItems;

    if(fromView) {
        m_type = clip4;
        UMLView *view = UMLApp::app()->currentView();
        view->checkSelections();
        if(!view->getSelectedWidgets(m_WidgetList)) {
            return 0;
        }
        //if there is no selected widget then there is no copy action
        if(!m_WidgetList.count()) {
            return 0;
        }
        m_AssociationList = view->getSelectedAssocs();
        view->copyAsImage(png);

    } else { //if the copy action is being performed from the ListView
        if(!listView->getSelectedItems(selectedItems)) {
            return 0;
        }
        //Set What type of copy operation are we performing and
        //also fill m_ViewList with all the selected Diagrams
        setCopyType(selectedItems);

        //if we are copying a diagram or part of a diagram, select the items
        //on the ListView that correspond to a UseCase, Actor or Concept
        //in the Diagram
        if(m_type == clip2) {
            //Fill the member lists with all the object and stuff to be copied
            //to the clipboard
            selectedItems.clear();
            //For each selected view select all the Actors, USe Cases and Concepts
            //widgets in the ListView
            foreach (UMLView* view, m_ViewList ) {
                UMLObjectList objects = view->getUMLObjects();
                foreach (UMLObject* o, objects ) {
                    UMLListViewItem *item = listView->findUMLObject(o);
                    if(item) {
                        listView->setSelected(item, true);
                    }
                }
            }
            if(!listView->getSelectedItems(selectedItems)) {
                return 0;
            }
        }
        if(!fillSelectionLists(selectedItems)) {
            return 0;
        }
    }
    int i =0;
    switch(m_type) {
    case clip1:
        data = new UMLDragData(m_ObjectList);
        break;
    case clip2:
        data = new UMLDragData(m_ObjectList, m_ItemList, m_ViewList);
        break;
    case clip3:
        data = new UMLDragData(m_ItemList);
        break;
    case clip4:
        if(png) {
            UMLView *view = UMLApp::app()->currentView();
            data = new UMLDragData(m_ObjectList, m_WidgetList,
                               m_AssociationList, *png, view->getType());
        } else {
            return 0;
        }
        break;
    case clip5:
        data = new UMLDragData(m_ObjectList, i);
        // The int i is used to differentiate
        // which UMLDragData constructor gets called.
        break;
    }

    return (QMimeData*)data;
}

/**
 * Inserts the clipboard's contents.
 *
 * @param data   Pointer to the MIME format clipboard data.
 * @return       True for successful operation.
 */
bool UMLClipboard::paste(const QMimeData* data)
{
    UMLDoc *doc = UMLApp::app()->document();
    bool result = false;
    doc->beginPaste();
    switch(UMLDragData::getCodingType(data)) {
    case 1:
        result = pasteClip1(data);
        break;
    case 2:
        result = pasteClip2(data);
        break;
    case 3:
        result = pasteClip3(data);
        break;
    case 4:
        result = pasteClip4(data);
        break;
    case 5:
        result = pasteClip5(data);
        break;
    default:
        break;
    }
    doc->endPaste();
    return result;
}

/**
 * Fills the member lists with all the objects and other
 * stuff to be copied to the clipboard.
 * @param selectedItems   list of selected items
 */
bool UMLClipboard::fillSelectionLists(UMLListViewItemList& selectedItems)
{
    Uml::ListView_Type type;
    switch(m_type) {
    case clip4:
        break;
    case clip3:
        foreach ( UMLListViewItem* item , selectedItems ) {
            type = item->getType();
            if ( !Model_Utils::typeIsClassifierList(type) ) {
                m_ItemList.append(item);
                insertItemChildren(item, selectedItems);
                //Because it is being called when m_type is 3
                //it will insert only child empty folders of other folders.
                //If a child folder
                //is not empty that means m_type wouldn't be 3 because if a folder is
                //selected then its complete contents are treated as if
                //they were selected
            }
        }
        break;
    case clip2:
    case clip1:
        foreach ( UMLListViewItem* item , selectedItems ) {
            type = item->getType();
            if ( !Model_Utils::typeIsClassifierList(type) ) {

                m_ItemList.append(item);

                if ( Model_Utils::typeIsCanvasWidget(type) ) {
                    m_ObjectList.append(item->getUMLObject());
                }
                insertItemChildren(item, selectedItems);
            }
        }
        break;
    case clip5:
        foreach ( UMLListViewItem* item , selectedItems ) {
            type = item->getType();
            if( Model_Utils::typeIsClassifierList(type) ) {
                m_ItemList.append(item);
                m_ObjectList.append(item->getUMLObject());

            } else {
                return false;
            }
        }
        break;
    }

    return true;
}

/**
 * Checks the whole list to determine the copy action
 * type to be performed, sets the type in the m_type
 * member variable.
 * @param selectedItems   list of selected items
 */
void UMLClipboard::setCopyType(UMLListViewItemList& selectedItems)
{
    bool withDiagrams = false; //If the selection includes diagrams
    bool withObjects = false; //If the selection includes objects
    bool onlyAttsOps = false; //If the selection only includes Attributes and/or Operations

    foreach ( UMLListViewItem* item, selectedItems ) {
        checkItemForCopyType(item, withDiagrams, withObjects, onlyAttsOps);
    }
    if(onlyAttsOps) {
        m_type = clip5;
    } else if(withDiagrams) {
        m_type = clip2;
    } else if(withObjects) {
        m_type = clip1;
    } else {
        m_type = clip3;
    }
}

/**
 * Searches the child items of a UMLListViewItem to
 * establish which Copy type is to be perfomed.
 * @param item          parent of the children
 * @param withDiagrams  includes diagrams
 * @param withObjects   includes objects
 * @param onlyAttsOps   includes only attributes and/or operations
 */
void UMLClipboard::checkItemForCopyType(UMLListViewItem* item, bool & withDiagrams, bool & withObjects,
                                        bool & onlyAttsOps)
{
    if(!item) {
        return;
    }
    UMLDoc *doc = UMLApp::app()->document();
    onlyAttsOps = true;
    UMLView * view = 0;
    UMLListViewItem * child = 0;
    Uml::ListView_Type type = item->getType();
    if ( Model_Utils::typeIsCanvasWidget(type) ) {
        withObjects = true;
        onlyAttsOps = false;
    } else if ( Model_Utils::typeIsDiagram(type) ) {
        withDiagrams = true;
        onlyAttsOps = false;
        view = doc->findView( item->getID() );
        m_ViewList.append( view );
    } else if ( Model_Utils::typeIsFolder(type) ) {
        onlyAttsOps = false;
        if(item->childCount()) {
            child = (UMLListViewItem*)item->firstChild();
            while(child) {
                checkItemForCopyType(child, withDiagrams, withObjects, onlyAttsOps);
                child = (UMLListViewItem*)child->nextSibling();
            }
        }
    }
}

/**
 * Adds the children of a UMLListViewItem to m_ItemList.
 * @param item            parent of the children to insert
 * @param selectedItems   list of selected items
 * @return                success flag
 */
bool UMLClipboard::insertItemChildren(UMLListViewItem * item, UMLListViewItemList& selectedItems)
{
    if(item->childCount()) {
        UMLListViewItem * child = (UMLListViewItem*)item->firstChild();
        int type;
        while(child) {
            m_ItemList.append(child);
            type = child->getType();
            if(type == Uml::lvt_Actor || type == Uml::lvt_UseCase || type == Uml::lvt_Class) {
                m_ObjectList.append(child->getUMLObject());
            }
            // If the child is selected, remove it from the list of selected items
            // otherwise it will be inserted twice in m_ObjectList.
            if(child->isSelected()) {
                selectedItems.removeAll(child);
            }
            insertItemChildren(child, selectedItems);
            child = (UMLListViewItem*)child->nextSibling();
        }
    }
    return true;
}

/**
 * Pastes the children of a UMLListViewItem (The Parent)
 * @param parent   parent of the children
 * @param chgLog   ID change log
 * @return         success flag
 */
bool UMLClipboard::pasteChildren(UMLListViewItem *parent, IDChangeLog *chgLog)
{
    if (!parent) {
        uWarning() << "Paste Children Error, parent missing";
        return false;
    }
    UMLDoc *doc = UMLApp::app()->document();
    UMLListView *listView = UMLApp::app()->listView();
    UMLListViewItem *childItem = static_cast<UMLListViewItem*>(parent->firstChild());
    while (childItem) {
        Uml::IDType oldID = childItem->getID();
        Uml::IDType newID = chgLog->findNewID(oldID);
        UMLListViewItem *shouldNotExist = listView->findItem(newID);
        if (shouldNotExist) {
            uError() << "new list view item " << ID2STR(newID)
                << " already exists (internal error)";
            childItem = static_cast<UMLListViewItem*>(childItem->nextSibling());
            continue;
        }
        UMLObject *newObj = doc->findObjectById(newID);
        if (newObj) {
            uDebug() << "adjusting lvitem(" << ID2STR(oldID)
                << ") to new UMLObject(" << ID2STR(newID) << ")";
            childItem->setUMLObject(newObj);
            childItem->setText(newObj->name());
        } else {
            uDebug() << "no UMLObject found for lvitem " << ID2STR(newID);
        }
        childItem = static_cast<UMLListViewItem*>(childItem->nextSibling());
    }
    return true;
}

/**
 * Cleans the list of associations taking out the ones that point to an object
 * not in m_ObjectList.
 * @param associations   list of associations
 */
void UMLClipboard::CleanAssociations(AssociationWidgetList& associations)
{
    AssociationWidgetListIt it(associations);

    while ( it.hasNext() ) {
        AssociationWidget* assoc = it.next();
        Q_UNUSED(assoc);
    }
}

/**
 * If clipboard has mime type application/x-uml-clip1,
 * Pastes the data from the clipboard into the current Doc.
 * @param data   mime type
 */
bool UMLClipboard::pasteClip1(const QMimeData* data)
{
    UMLObjectList objects;
    if (! UMLDragData::decodeClip1(data, objects)) {
        return false;
    }
    UMLListView *lv = UMLApp::app()->listView();
    if ( !lv->startedCopy() )
        return true;
    lv->setStartedCopy(false);
    /* If we get here we are pasting after a Copy and need to
    // paste possible children.
    UMLListViewItem* itemdata = 0;
    UMLListViewItemListIt it(itemdatalist);
    while ( (itemdata=it.current()) != 0 ) {
        if(itemdata->childCount()) {
                if(!pasteChildren(itemdata, idchanges)) {
                        return false;
                }
        }
        ++it;
    }
     */
    return true;
}

/**
 * If clipboard has mime type application/x-uml-clip2,
 * Pastes the data from the clipboard into the current Doc.
 * @param data   mime type
 * @return       success flag
 */
bool UMLClipboard::pasteClip2(const QMimeData* data)
{
    UMLDoc*             doc = UMLApp::app()->document();
    UMLListViewItemList itemdatalist;
    UMLObjectList       objects;
    UMLViewList         views;
    IDChangeLog*        idchanges = 0;

    bool result = UMLDragData::decodeClip2(data, objects, itemdatalist, views);
    if(!result) {
        return false;
    }

    idchanges = doc->changeLog();
    if(!idchanges) {
        return false;
    }
    foreach ( UMLObject* obj, objects ) {
        if(!doc->assignNewIDs(obj)) {
            uDebug() << "UMLClipboard: error adding umlobject";
            return false;
        }
    }

    foreach ( UMLView* pView, views ) {
        if( !doc->addUMLView( pView ) ) {
            return false;
        }
    }

    UMLListView *listView = UMLApp::app()->listView();

    foreach ( UMLListViewItem* itemdata, itemdatalist ) {
        UMLListViewItem* item = listView->createItem(*itemdata, *idchanges);
        if(!item) {
            return false;
        }
        if(itemdata->childCount()) {
            if(!pasteChildren(item, idchanges)) {
                return false;
            }
        }
    }

    return result;
}

/**
 * If clipboard has mime type application/x-uml-clip3,
 * Pastes the data from the clipboard into the current Doc.
 * @param data   mime type
 * @return       success flag
 */
bool UMLClipboard::pasteClip3(const QMimeData* data)
{
    UMLDoc *doc = UMLApp::app()->document();
    UMLListViewItemList itemdatalist;
    IDChangeLog* idchanges = doc->changeLog();

    if(!idchanges) {
        return false;
    }

    UMLListView *listView = UMLApp::app()->listView();
    bool result = UMLDragData::decodeClip3(data, itemdatalist, listView);
    if(!result) {
        return false;
    }

    foreach ( UMLListViewItem* itemdata, itemdatalist ) {
        UMLListViewItem* item = listView->createItem(*itemdata, *idchanges);
        if(itemdata->childCount()) {
            if(!pasteChildren(item, idchanges)) {
                return false;
            }
        }
    }

    return result;
}

/**
 * If clipboard has mime type application/x-uml-clip4,
 * Pastes the data from the clipboard into the current Doc.
 * @param data   mime type
 * @return       success flag
 */
bool UMLClipboard::pasteClip4(const QMimeData* data)
{
    UMLDoc *doc = UMLApp::app()->document();

    UMLObjectList objects;
    UMLWidgetList widgets;
    AssociationWidgetList assocs;

    IDChangeLog* idchanges = 0;

    Uml::Diagram_Type diagramType;

    if( !UMLDragData::decodeClip4(data, objects, widgets, assocs, diagramType) ) {
        return false;
    }

    if( diagramType != UMLApp::app()->currentView()->getType() ) {
        if( !checkPasteWidgets(widgets) ) {
            while ( !assocs.isEmpty() ) {
                delete assocs.takeFirst();
            }
            return false;
        }
    }

    idchanges = doc->changeLog();
    if(!idchanges) {
        return false;
    }
     //make sure the file we are pasting into has the objects
     //we need if there are widgets to be pasted
    foreach ( UMLObject* obj, objects ) {
        if(!doc->assignNewIDs(obj)) {
            return false;
        }

     }

    //now add any widget we are want to paste
    bool objectAlreadyExists = false;
    UMLView *currentView = UMLApp::app()->currentView();
    currentView->beginPartialWidgetPaste();

    foreach ( UMLWidget* widget, widgets ) {

        Uml::IDType oldId = widget->id();
        Uml::IDType newId = idchanges->findNewID(oldId);
        if (currentView->findWidget(newId)) {
            uError() << "widget (oldID=" << ID2STR(oldId) << ", newID="
                << ID2STR(newId) << ") already exists in target view.";
            widgets.removeAll(widget);
            delete widget;
            objectAlreadyExists = true;
        } else if (! currentView->addWidget(widget, true)) {
            currentView->endPartialWidgetPaste();
            return false;
        }
    }

    //now paste the associations
    foreach ( AssociationWidget* assoc, assocs ) {
        if (!currentView->addAssociation(assoc, true)) {
            currentView->endPartialWidgetPaste();
            return false;
        }
    }

    //Activate all the pasted associations and widgets
    currentView->activate();
    currentView->endPartialWidgetPaste();

    /*
    UMLListView *listView = UMLApp::app()->listView();
    UMLListViewItem* item = 0;
    UMLListViewItem* itemdata = 0;
    UMLListViewItemListIt it(itemdatalist);
    while ( (itemdata=it.current()) != 0 ) {
        item = listView->createItem(*itemdata, *idchanges);
        if(!item) {
            return false;
        }
        if(itemdata->childCount()) {
            if(!pasteChildren(item, idchanges)) {
                return false;
            }
        }
        ++it;
        }*/

    if (objectAlreadyExists) {
        pasteItemAlreadyExists();
    }
    return true;
}

/**
 * If clipboard has mime type application/x-uml-clip5,
 * Pastes the data from the clipboard into the current Doc.
 * @param data   mime type
 * @return       success flag
 */
bool UMLClipboard::pasteClip5(const QMimeData* data)
{
    UMLDoc *doc = UMLApp::app()->document();
    UMLListView *listView = UMLApp::app()->listView();
    UMLListViewItem* lvitem = dynamic_cast<UMLListViewItem *>( listView->currentItem() );
    if (!lvitem || !Model_Utils::typeIsClassifier( lvitem->getType() )) {
        return false;
    }
    UMLClassifier *parent = dynamic_cast<UMLClassifier*>( lvitem->getUMLObject() );

    if (parent == NULL) {
        uError() << "parent is not a UMLClassifier";
        return false;
    }

    UMLObjectList objects;

    IDChangeLog* idchanges = 0;
    bool result = UMLDragData::decodeClip5(data, objects, parent);

    if(!result) {
        return false;
    }

    doc->setModified(true);
    idchanges = doc->changeLog();
    // Assume success if at least one child object could be pasted
    if (objects.count())
        result = false;

    foreach (UMLObject* obj, objects ) {
        obj->setID(doc->assignNewID(obj->id()));
        switch(obj->baseType()) {
        case Uml::ot_Attribute :
            {
                UMLObject *exist = parent->findChildObject(obj->name(), Uml::ot_Attribute);
                if (exist) {
                    QString newName = parent->uniqChildName(Uml::ot_Attribute, obj->name());
                    obj->setName(newName);
                }
                UMLAttribute *att = static_cast<UMLAttribute*>(obj);
                if (parent->addAttribute(att, idchanges)) {
                    result = true;
                } else {
                    uError() << "" << parent->name() << "->addAttribute("
                             << att->name() << ") failed";
                }
                break;
            }
        case Uml::ot_Operation :
            {
                UMLOperation *op = static_cast<UMLOperation*>(obj);
                UMLOperation *exist = parent->checkOperationSignature(op->name(), op->getParmList());
                if (exist) {
                    QString newName = parent->uniqChildName(Uml::ot_Operation, obj->name());
                    op->setName(newName);
                }
                if (parent->addOperation(op, idchanges)) {
                    result = true;
                } else {
                    uError() << "" << parent->name() << "->addOperation("
                             << op->name() << ") failed";
                }
                break;
            }
        case Uml::ot_Template:
            {
                UMLTemplate* tp = static_cast<UMLTemplate*>( obj );
                UMLTemplate* exist = parent->findTemplate( tp->name() );
                if ( exist ) {
                    QString newName = parent->uniqChildName( Uml::ot_Template, obj->name() );
                    tp->setName( newName );
                }
                if ( parent->addTemplate( tp, idchanges ) ) {
                    result = true;
                } else {
                    uError() << "" << parent->name() << "->addTemplate("
                             << tp->name() << ") failed";
                }
                break;
            }
        case Uml::ot_EnumLiteral:
           {
               UMLEnum* enumParent = dynamic_cast<UMLEnum*>( parent );
               // if parent is not a UMLEnum, bail out immediately;
               if ( !enumParent ) {
                   result = false;
                   uError() << "Parent is not an UMLEnum";
                   break;
               }

               UMLObject* exist = enumParent->findChildObject( obj->name(), Uml::ot_EnumLiteral );
               if ( exist ) {
                   QString newName = enumParent->uniqChildName( Uml::ot_EnumLiteral, obj->name() );
                   obj->setName( newName );
               }
               UMLEnumLiteral* enl = static_cast<UMLEnumLiteral*>( obj );

               if ( enumParent->addEnumLiteral( enl, idchanges ) ) {
                   result = true;
               } else {
                   uError() << "" << enumParent->name() << "->addEnumLiteral("
                            << enl->name() << ") failed";
               }
               break;
           }
        case Uml::ot_EntityAttribute :
            {
                UMLEntity* entityParent = dynamic_cast<UMLEntity*>( parent );
                // if parent is not a UMLEntity, bail out immediately;
                if ( !entityParent ) {
                    result = false;
                    uError() << "Parent is not an UMLEntity";
                    break;
                }
                UMLObject *exist = entityParent->findChildObject(obj->name(), Uml::ot_EntityAttribute);
                if (exist) {
                    QString newName = entityParent->uniqChildName(Uml::ot_EntityAttribute, obj->name());
                    obj->setName(newName);
                }
                UMLEntityAttribute *att = static_cast<UMLEntityAttribute*>(obj);

                if ( entityParent->addEntityAttribute(att, idchanges)) {
                    result = true;
                } else {
                    uError() << "" << parent->name() << "->addEntityAttribute(" << att->name() << ") failed";
                }
                break;
            }
        default :
            uWarning() << "pasting unknown children type in clip type 5";
            return false;
        }
    }

    return result;
}

/**
 * Inserts the data of the children of the given item
 * into the item data list.  Used for clip type 4.  Used
 * to make * sure classes have all the attributes and
 * operations saved.
 * @param item   parent item, its children are inserted
 * @return       success flag
 */
bool UMLClipboard::insertItemChildren( UMLListViewItem * item )
{
    if( item->childCount() ) {
        UMLListViewItem * child =dynamic_cast<UMLListViewItem *>( item->firstChild() );
        while( child ) {
            m_ItemList.append( child );
            insertItemChildren( child );
            child = dynamic_cast<UMLListViewItem *>( child->nextSibling() );
        }
    }
    return true;
}

/**
 * When pasting widgets see if they can be pasted on
 * different diagram types.  Will return true if all the
 * widgets to be pasted can be.  At the moment this only
 * includes NoteWidgets and lines of text.
 *
 * @param widgetList   List of widgets to examine.
 * @return   True if all widgets can be put on different diagrams.
 */
bool UMLClipboard::checkPasteWidgets( UMLWidgetList & widgetList )
{
    bool retval = true;

    foreach ( UMLWidget* p, widgetList ) {
        switch( p->baseType() ) {
        case Uml::wt_Note:
            break;

        case Uml::wt_Text:
            {
                FloatingTextWidget *ft = static_cast<FloatingTextWidget*>(p);
                if (ft->textRole() != Uml::tr_Floating) {
                    widgetList.removeAll(p);
                    delete ft;
                    retval = false;
                }
            }
            break;

        default:
            widgetList.removeAll(p);
            delete p;
            retval = false;
            break;
        }
    }
    return retval;
}

/**
 * Gives a `sorry' message box if you're pasting an item which
 * already exists and can't be duplicated.
 */
void UMLClipboard::pasteItemAlreadyExists()
{
    UMLView *currentView = UMLApp::app()->currentView();
    KMessageBox::sorry( currentView,
                        i18n("At least one of the items in the clipboard "
                             "could not be pasted because an item of the "
                             "same name already exists.  Any other items "
                             "have been pasted."),
                        i18n("Paste Error") );
}

#include "umlclipboard.moc"
