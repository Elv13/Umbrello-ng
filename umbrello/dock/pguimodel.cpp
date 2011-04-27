//Own header
#include "pguimodel.h"

//app include
#include "attribute.h"
#include "operation.h"
#include "classifier.h"
#include "template.h"
#include "umloperationdialog.h"
#include "umltemplatelist.h"
#include "uml.h"
#include "umldoc.h"

//Qt
#include <QtGui/QLineEdit>
#include <QtGui/QPalette>

QHash< QTableWidgetItem*, pGuiModel* > pGuiModel::linker;

void pGuiModel::connectSlots() 
{
  
  //connect(m_pTopArrowB, SIGNAL( clicked() ), this, SLOT( slotTopClicked() ) );
  //connect(name, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* stereotype, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* initial, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* defaultValue, SIGNAL(  ), this, SLOT(  ) );
  if (parameters) {
    connect(parameters, SIGNAL( paramClicked() ), this, SLOT( parametersChanged() ) );
    connect(parameters, SIGNAL( addParamater(QString,QString,QString,QString) ), this, SLOT(slotAddParamater(QString,QString,QString,QString)) );
  }
  if (type)
    connect(type, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
  if (visibility)
    connect(visibility, SIGNAL( currentIndexChanged(int) ), this, SLOT( visibilityChanged(int) ) );
  if (attributes)
    connect(attributes, SIGNAL( currentIndexChanged(int) ), this, SLOT( attributesChanged(int) ) );
  if (staticV)
    connect(staticV, SIGNAL( stateChanged(int) ), this, SLOT( staticVChanged(int) ) );
  if (abstract)
    connect(abstract, SIGNAL( stateChanged(int) ), this, SLOT( abstractChanged(int) ) );
  if (constV)
    connect(constV, SIGNAL( stateChanged(int) ), this, SLOT( constVChanged(int) ) );
  if (autoIncrement)
    connect(autoIncrement, SIGNAL( stateChanged(int) ), this, SLOT( autoIncrementChanged(int) ) );
  if (allowNull)
    connect(allowNull, SIGNAL( stateChanged(int) ), this, SLOT( allowNullChanged(int) ) );
  if (indexed)
    connect(indexed, SIGNAL( stateChanged(int) ), this, SLOT( indexedChanged(int) ) );
  if (m_pDocumentation)
    connect(m_pDocumentation, SIGNAL( changed(QString) ), this, SLOT( documentationChanged(QString) ) );
  if (m_pSource)
    connect(m_pSource, SIGNAL( changed(QString) ), this, SLOT( sourceChanged(QString) ) );
}


void pGuiModel::parametersChanged()
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  
  if (qobject_cast<UMLOperation*>(classifier)) {
    QPointer<UMLOperationDialog> dialog = new UMLOperationDialog(UMLApp::app(), qobject_cast<UMLOperation*>(classifier));
    dialog->exec();
    delete dialog;
  }
  
  qDebug() << "parametersChanged Changed";
}

void pGuiModel::typeChanged(QString)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    classifier->setTypeName(type->lineEdit()->text());
  }
  qDebug() << "typeChanged Changed";
}

void pGuiModel::visibilityChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    if (visibility->currentIndex() == 3) //TODO ELV find why only Uml::Visibility::FromParent -need- to be "3"
      classifier->setVisibility(Uml::Visibility::Implementation);
    else if (visibility->currentIndex() == Uml::Visibility::Public)
      classifier->setVisibility(Uml::Visibility::Public);
    else if (visibility->currentIndex() == Uml::Visibility::Private)
      classifier->setVisibility(Uml::Visibility::Private);
    else if (visibility->currentIndex() == Uml::Visibility::Protected)
      classifier->setVisibility(Uml::Visibility::Protected);
    
  }
  qDebug() << "visibilityChanged Changed";
}

void pGuiModel::attributesChanged(int)
{
  qDebug() << "attributesChanged Changed";
}

void pGuiModel::staticVChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    classifier->setStatic(staticV->isChecked());
  }
  qDebug() << "staticVChanged Changed";
}

void pGuiModel::abstractChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    classifier->setAbstract(
      abstract->isChecked()
    );
  }
  qDebug() << "abstractChanged Changed";
}

void pGuiModel::constVChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    if (qobject_cast<UMLOperation*>(classifier))
      qobject_cast<UMLOperation*>(classifier)->setConst(constV->isChecked());
  }
  qDebug() << "constVChanged Changed";
}

void pGuiModel::autoIncrementChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  qDebug() << "autoIncrementChanged Changed";
}

void pGuiModel::allowNullChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  qDebug() << "allowNull Changed";
}

void pGuiModel::indexedChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  qDebug() << "indexed Changed";
}

void pGuiModel::documentationChanged(QString text)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    
  }
  qDebug() << "documentationChanged Changed";
}

void pGuiModel::sourceChanged(QString text)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    
  }
  qDebug() << "sourceChanged Changed";
}
  
void pGuiModel::cellChanged(QTableWidgetItem* item) 
{
  if (!name) {
    kDebug() << "item is null";
  }
  else if (item == name) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    if (classifier) {
      classifier->setName(name->text()); 
    }
    qDebug() << "name Changed";
  }
  else if (item == initial) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    if (classifier) {
      if (qobject_cast<UMLAttribute*>(classifier))
        qobject_cast<UMLAttribute*>(classifier)->setInitialValue(initial->text());
    }
    qDebug() << "initial Changed";
  }
  else if (item == defaultValue) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    qDebug() << "defaultValue Changed";
  }
  else if (item == stereotype) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    if (classifier) {
      classifier->setStereotype(stereotype->text());
    }
    qDebug() << "stereotype Changed";
  }
  else if (item == length) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    //TODO ELV
    qDebug() << "length Changed";
  }
}

void pGuiModel::setClassifier(UMLClassifierListItem* _classifier)
{
  classifier = _classifier;
  connect(classifier, SIGNAL(modified()),this,SLOT(reload()));
  
  reload();
}

void pGuiModel::reload()
{
  if (classifier) {
    if (name && (name->text() != classifier->name())) 
        name->setText(classifier->name());
        
    if (staticV) //TODO ELV check if its not the same value too
      staticV->setChecked(classifier->isStatic());
    
    if (abstract)
      abstract->setChecked(classifier->isAbstract());
    
      if (parameters) {
        if (qobject_cast<UMLOperation*>(classifier)) {
          parameters->setParam(qobject_cast<UMLOperation*>(classifier));
        }
      }
      
    if (type)
      insertTypesSorted(classifier->getTypeName());//type->setText(classifier->getTypeName());
    
    if (visibility) {
      if (classifier->visibility() == Uml::Visibility::Implementation) //TODO ELV find why only Uml::Visibility::FromParent -need- to be "3"
        visibility->setCurrentIndex(3);
      else
        visibility->setCurrentIndex(classifier->visibility());
    }
    
    if (constV)
      if (qobject_cast<UMLOperation*>(classifier))
        constV->setChecked(qobject_cast<UMLOperation*>(classifier)->getConst());
      
    if (m_pSource)
      if (qobject_cast<UMLOperation*>(classifier))
        m_pSource->setPopupText(qobject_cast<UMLOperation*>(classifier)->getSourceCode());
    
    if (m_pDocumentation)
      m_pDocumentation->setPopupText(classifier->doc());
    
    if (stereotype)
      stereotype->setText(classifier->stereotype());
    
    if (initial)
      if (qobject_cast<UMLAttribute*>(classifier))
        initial->setText(qobject_cast<UMLAttribute*>(classifier)->getInitialValue());
      
    //QTableWidgetItem* defaultValue;//TODO

    connect( classifier, SIGNAL(modified()),this,SLOT(slotListItemModified()) ); //TODO ELV
    
    if (classifier->doc().isEmpty()) {
      //m_pDocumentation->setPalette(*m_greenTint);
    }
    else {
      //m_pDocumentation->setPalette(*m_redTint);
    }
    
    if (m_pSource)
      if (qobject_cast<UMLOperation*>(classifier))
        if (!qobject_cast<UMLOperation*>(classifier)->getSourceCode().isEmpty()) {
          //m_pSource->setPalette(*m_greenTint);
        }
        else {
          //m_pSource->setPalette(*m_redTint);
        }
  }
    
}

void pGuiModel::disconnectAndDelete()
{
  //TODO ELV
  /*disconnect(classifier, SIGNAL(modified()),this,SLOT(reload()));
  disconnect(parameters, SIGNAL( clicked(bool) ), this, SLOT( parametersChanged(bool) ) );
  disconnect(type, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
  disconnect(visibility, SIGNAL( currentIndexChanged(int) ), this, SLOT( visibilityChanged(int) ) );
  disconnect(staticV, SIGNAL( stateChanged(int) ), this, SLOT( staticVChanged(int) ) );
  disconnect(abstract, SIGNAL( stateChanged(int) ), this, SLOT( abstractChanged(int) ) );
  disconnect(constV, SIGNAL( stateChanged(int) ), this, SLOT( constVChanged(int) ) );
  disconnect(m_pDocumentation, SIGNAL( clicked(bool) ), this, SLOT( m_pDocumentationChanged(bool) ) );
  disconnect(m_pSource, SIGNAL( clicked(bool) ), this, SLOT( sourceChanged(bool) ) );*/
  linker[name]=NULL;
  linker[stereotype]=NULL;
  linker[initial]=NULL;
  linker[defaultValue]=NULL;
  disconnect(); //Everything else
}

/**
 * Should never happen (none of the case), but if it does. at least dont segfault
 */
/*void pGuiModel::destroyTableItem(QObject *obj)
{
  if (obj == name) {
    name = 0;
    qDebug() << "name have been destroyed externally";
  }
  else if (obj == initial) {
    initial = 0;
    qDebug() << "initial have been destroyed externally";
  }
  else if (obj == defaultValue) {
    defaultValue = 0;
    qDebug() << "defaultValue have been destroyed externally";
  }
  else if (obj == stereotype) {
    stereotype = 0;
    qDebug() << "stereotype have been destroyed externally";
  }
  else {
    qDebug() << "an unknow tableitem have been destroyed externally";
  }
}*/


/**
 * Inserts @p type into the type-combobox as well as its completion object.
 * The combobox is cleared and all types together with the optional new one
 * sorted and then added again.
 * @param type   a new type to add and selected
 */
void pGuiModel::insertTypesSorted(const QString& currentType)
{
  if (type) {
    disconnect(type, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
    type->clear();
    QStringList types;
    // add template parameters
    UMLClassifier *pConcept = 0;
    
    if (qobject_cast<UMLAttribute*>(classifier))
      pConcept = qobject_cast<UMLClassifier*>(classifier->parent());
    else if (qobject_cast<UMLOperation*>(classifier))
      pConcept = qobject_cast<UMLClassifier*>(classifier->parent());

    if (!qobject_cast<UMLClassifier*>(pConcept)) {
      type->setText(currentType);
      connect(type, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
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

    
    //type->clear();
    
    type->addItems(types);
    

    // select the given parameter
    int currentIndex = type->findText(currentType);
    if (currentIndex > -1) {
        type->setCurrentIndex(currentIndex);
    }
    else {
      type->setText(currentType);
    }
    connect(type, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
  }
}

PopupButton* pGuiModel::getDocumentation()
{
  return m_pDocumentation;
}

void pGuiModel::setDocumentation(PopupButton* widget)
{
  m_pDocumentation = widget;
}

PopupButton* pGuiModel::getSource()
{
  return m_pSource;
}

void pGuiModel::setSource(PopupButton* widget)
{
  m_pSource = widget;
}

void pGuiModel::slotAddParamater(QString name,QString init,QString type, QString doc)
{
  if (qobject_cast<UMLOperation*>(classifier)) {
    UMLAttribute* attr = new UMLAttribute(classifier);
    attr->setName(name);
    attr->setInitialValue(init);
    //TODO ELV type
    attr->setDoc(doc);
    
    qobject_cast<UMLOperation*>(classifier)->addParm(attr);
  }
}

void pGuiModel::init()
{
  if (m_pRowType == Uml::ot_Operation) {
    UMLOperation* oper = new UMLOperation(0/*m_pParent*/);
    oper->setName(name->text());
    //parameters
    oper->setTypeName(type->lineEdit()->text());
    /*//TODO ELV visibility
    stereotype->text()
    staticV->ischecked()
    abstract->isChecked()
    constV->ischecked()*/
    //m_pDocumentation
    //m_pSource
    
    
    ((UMLClassifier*)m_pParent)->addOperation(oper);
    //setClassifier(oper);
  }
}