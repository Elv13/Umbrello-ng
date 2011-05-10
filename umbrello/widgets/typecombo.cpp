#include "typecombo.h"

#include "classifierlistitem.h"
#include "attribute.h"
#include "operation.h"
#include "classifier.h"
#include "template.h"
#include "uml.h"
#include "umldoc.h"

#include <QtGui/QWidget>


TypeCombo::TypeCombo(QWidget* parent) : CompactCombo(parent)
{
    
}

void TypeCombo::fill(const QString& currentType, UMLClassifierListItem* classifier)
{
    //disconnect(this, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
    clear();
    QStringList types;
    // add template parameters
    UMLClassifier *pConcept = 0;
    
    if (qobject_cast<UMLAttribute*>(classifier))
      pConcept = qobject_cast<UMLClassifier*>(classifier->parent());
    else if (qobject_cast<UMLOperation*>(classifier))
      pConcept = qobject_cast<UMLClassifier*>(classifier->parent());

    if (!qobject_cast<UMLClassifier*>(pConcept)) {
      setText(currentType);
      //connect(m_pType, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
      return;
    }
    
    
    UMLTemplateList tmplParams( pConcept->getTemplateList() );
    foreach( UMLTemplate* t, tmplParams ) {
        types << t->name();
    }
        
    // now add the Conceptsm_pUmldoc
    UMLClassifierList namesList( UMLApp::app()->document()->concepts() );
    foreach(UMLClassifier* obj, namesList ) {
        types << obj->fullyQualifiedName();
    }
    // add the given parameter
    if ( !types.contains(currentType) ) {
        types << currentType;
    }
    types.sort();

    
    //m_pType->clear();
    
    addItems(types);
    

    // select the given parameter
    int currentIndex = findText(currentType);
    if (currentIndex > -1) {
        setCurrentIndex(currentIndex);
    }
    else {
      setText(currentType);
    }
}