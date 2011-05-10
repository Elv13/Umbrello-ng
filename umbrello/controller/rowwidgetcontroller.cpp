//Own header
#include "rowwidgetcontroller.h"

//app include
#include "attribute.h"
#include "operation.h"
#include "classifier.h"
#include "template.h"
#include "umloperationdialog.h"
#include "umltemplatelist.h"
#include "uml.h"
#include "umldoc.h"
#include "typecombo.h"

//Qt
#include <QtGui/QLineEdit>
#include <QtGui/QPalette>

QHash< QTableWidgetItem*, RowWidgetController* > RowWidgetController::linker;

void RowWidgetController::connectSlots() 
{
  
  //connect(m_pTopArrowB, SIGNAL( clicked() ), this, SLOT( slotTopClicked() ) );
  //connect(m_pName, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* m_pStereotype, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* m_pInitial, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* m_pDefaultValue, SIGNAL(  ), this, SLOT(  ) );
  if (m_pParameters) {
    connect(m_pParameters, SIGNAL( paramClicked() ), this, SLOT( parametersChanged() ) );
    connect(m_pParameters, SIGNAL( addParamater(QString,QString,QString,QString) ), this, SLOT(slotAddParamater(QString,QString,QString,QString)) );
  }
  if (m_pType)
    connect(m_pType, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
  if (m_pVisibility)
    connect(m_pVisibility, SIGNAL( currentIndexChanged(int) ), this, SLOT( visibilityChanged(int) ) );
  if (m_pAttributes)
    connect(m_pAttributes, SIGNAL( currentIndexChanged(int) ), this, SLOT( attributesChanged(int) ) );
  if (m_pStaticV)
    connect(m_pStaticV, SIGNAL( stateChanged(int) ), this, SLOT( staticVChanged(int) ) );
  if (m_pAbstract)
    connect(m_pAbstract, SIGNAL( stateChanged(int) ), this, SLOT( abstractChanged(int) ) );
  if (m_pConstV)
    connect(m_pConstV, SIGNAL( stateChanged(int) ), this, SLOT( constVChanged(int) ) );
  if (m_pAutoIncrement)
    connect(m_pAutoIncrement, SIGNAL( stateChanged(int) ), this, SLOT( autoIncrementChanged(int) ) );
  if (m_pAllowNull)
    connect(m_pAllowNull, SIGNAL( stateChanged(int) ), this, SLOT( allowNullChanged(int) ) );
  if (m_pIndexed)
    connect(m_pIndexed, SIGNAL( stateChanged(int) ), this, SLOT( indexedChanged(int) ) );
  if (m_pDocumentation)
    connect(m_pDocumentation, SIGNAL( changed(QString) ), this, SLOT( documentationChanged(QString) ) );
  if (m_pSource)
    connect(m_pSource, SIGNAL( changed(QString) ), this, SLOT( sourceChanged(QString) ) );
}


void RowWidgetController::parametersChanged()
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

void RowWidgetController::typeChanged(QString)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    classifier->setTypeName(m_pType->lineEdit()->text());
  }
  qDebug() << "typeChanged Changed";
}

void RowWidgetController::visibilityChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    if (m_pVisibility->currentIndex() == 3) //TODO ELV find why only Uml::Visibility::FromParent -need- to be "3"
      classifier->setVisibility(Uml::Visibility::Implementation);
    else if (m_pVisibility->currentIndex() == Uml::Visibility::Public)
      classifier->setVisibility(Uml::Visibility::Public);
    else if (m_pVisibility->currentIndex() == Uml::Visibility::Private)
      classifier->setVisibility(Uml::Visibility::Private);
    else if (m_pVisibility->currentIndex() == Uml::Visibility::Protected)
      classifier->setVisibility(Uml::Visibility::Protected);
    
  }
  qDebug() << "visibilityChanged Changed";
}

void RowWidgetController::attributesChanged(int)
{
  qDebug() << "attributesChanged Changed";
}

void RowWidgetController::staticVChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    classifier->setStatic(m_pStaticV->isChecked());
  }
  qDebug() << "staticVChanged Changed";
}

void RowWidgetController::abstractChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    classifier->setAbstract(
      m_pAbstract->isChecked()
    );
  }
  qDebug() << "abstractChanged Changed";
}

void RowWidgetController::constVChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    if (qobject_cast<UMLOperation*>(classifier))
      qobject_cast<UMLOperation*>(classifier)->setConst(m_pConstV->isChecked());
  }
  qDebug() << "constVChanged Changed";
}

void RowWidgetController::autoIncrementChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  qDebug() << "autoIncrementChanged Changed";
}

void RowWidgetController::allowNullChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  qDebug() << "allowNull Changed";
}

void RowWidgetController::indexedChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  qDebug() << "indexed Changed";
}

void RowWidgetController::documentationChanged(QString text)
{
  if (!m_pIsModified) {
    emit(addNew()); 
    init();
    m_pIsModified = true;
  }
  if (classifier) {
    classifier->setDoc(text);
    
  }
  qDebug() << "documentationChanged Changed";
}

void RowWidgetController::sourceChanged(QString text)
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
  
void RowWidgetController::cellChanged(QTableWidgetItem* item) 
{
  if (!m_pName) {
    kDebug() << "item is null";
  }
  else if (item == m_pName) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    if (classifier) {
      classifier->setName(m_pName->text()); 
    }
    qDebug() << "name Changed";
  }
  else if (item == m_pInitial) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    if (classifier) {
      if (qobject_cast<UMLAttribute*>(classifier))
        qobject_cast<UMLAttribute*>(classifier)->setInitialValue(m_pInitial->text());
    }
    qDebug() << "initial Changed";
  }
  else if (item == m_pDefaultValue) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    qDebug() << "defaultValue Changed";
  }
  else if (item == m_pStereotype) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    if (classifier) {
      classifier->setStereotype(m_pStereotype->text());
    }
    qDebug() << "stereotype Changed";
  }
  else if (item == m_pLength) {
    if (!m_pIsModified) {
      emit(addNew()); 
      init();
      m_pIsModified = true;
    }
    //TODO ELV
    qDebug() << "length Changed";
  }
}

void RowWidgetController::setClassifier(UMLClassifierListItem* _classifier)
{
  classifier = _classifier;
  
    if (m_pType) { //TODO ELV is it ok to place it here?
        disconnect(m_pType, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
        m_pType->fill(classifier->getTypeName(),classifier);//type->setText(classifier->getTypeName());
        connect(m_pType, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
    }
  
  connect(classifier, SIGNAL(modified()),this,SLOT(reload()));
  
  reload();
}

void RowWidgetController::reload()
{
  if (classifier) {
    if (m_pName && (m_pName->text() != classifier->name())) 
        m_pName->setText(classifier->name());
        
    if (m_pStaticV) //TODO ELV check if its not the same value too
      m_pStaticV->setChecked(classifier->isStatic());
    
    if (m_pAbstract)
      m_pAbstract->setChecked(classifier->isAbstract());
    
      if (m_pParameters) {
        if (qobject_cast<UMLOperation*>(classifier)) {
          m_pParameters->setParam(qobject_cast<UMLOperation*>(classifier));
        }
      }
      
      if (m_pType) {
        m_pType->setText(classifier->getTypeName());
      }
    
    if (m_pVisibility) {
      if (classifier->visibility() == Uml::Visibility::Implementation) //TODO ELV find why only Uml::Visibility::FromParent -need- to be "3"
        m_pVisibility->setCurrentIndex(3);
      else
        m_pVisibility->setCurrentIndex(classifier->visibility());
    }
    
    if (m_pConstV)
      if (qobject_cast<UMLOperation*>(classifier))
        m_pConstV->setChecked(qobject_cast<UMLOperation*>(classifier)->getConst());
      
    if (m_pSource)
      if (qobject_cast<UMLOperation*>(classifier))
        m_pSource->setPopupText(qobject_cast<UMLOperation*>(classifier)->getSourceCode());
    
    if (m_pDocumentation)
      m_pDocumentation->setPopupText(classifier->doc());
    
    if (m_pStereotype)
      m_pStereotype->setText(classifier->stereotype());
    
    if (m_pInitial)
      if (qobject_cast<UMLAttribute*>(classifier))
        m_pInitial->setText(qobject_cast<UMLAttribute*>(classifier)->getInitialValue());
      
    //QTableWidgetItem* m_pDefaultValue;//TODO

    connect( classifier, SIGNAL(modified()),this,SLOT(slotListItemModified()) ); //TODO ELV
  }
    
}

void RowWidgetController::disconnectAndDelete()
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
  linker[m_pName]=NULL;
  linker[m_pStereotype]=NULL;
  linker[m_pInitial]=NULL;
  linker[m_pDefaultValue]=NULL;
  disconnect(); //Everything else
}

PopupButton* RowWidgetController::getDocumentation()
{
  return m_pDocumentation;
}

void RowWidgetController::setDocumentation(PopupButton* widget)
{
  m_pDocumentation = widget;
}

PopupButton* RowWidgetController::getSource()
{
  return m_pSource;
}

void RowWidgetController::setSource(PopupButton* widget)
{
  m_pSource = widget;
}

void RowWidgetController::slotAddParamater(QString name,QString init,QString type, QString doc)
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

void RowWidgetController::init()
{
    if (!m_pParent || !qobject_cast<UMLClassifier*>(m_pParent))
        return; //TODO ELV how to handle this?
  switch (m_pRowType) {
    case Uml::ot_Operation: {
      UMLOperation* oper = new UMLOperation(0/*m_pParent*/);
      oper->setName(m_pName->text());
      //parameters
      oper->setTypeName(m_pType->lineEdit()->text());
      /*//TODO ELV visibility
      stereotype->text()
      staticV->ischecked()
      m_pAbstract->isChecked()
      m_pConstV->ischecked()*/
      //m_pDocumentation
      //m_pSource
      
      
      ((UMLClassifier*)m_pParent)->addOperation(oper);
      setClassifier(oper);
    }
    break;
    case Uml::ot_Attribute: {
      UMLAttribute* attr = new UMLAttribute(classifier);
      attr->setName(m_pName->text());
      //TODO ELV
      attr->setTypeName(m_pType->lineEdit()->text());
    }
    break;
    case Uml::ot_Template: {
      
    }
    break;
    case Uml::ot_EnumLiteral: {
      
    }
    break;
    case Uml::ot_EntityAttribute: {
      
    }
    break;
    case Uml::ot_EntityConstraint:
      break;
    default:
      break;
  }
      
}

QTableWidgetItem* RowWidgetController::getName()
{
  return m_pName;
}

QTableWidgetItem* RowWidgetController::getStereotype()
{
  return m_pStereotype;
}

QTableWidgetItem* RowWidgetController::getInitial()
{
  return m_pInitial;
}

QTableWidgetItem* RowWidgetController::getDefaultValue()
{
  return m_pDefaultValue;
}

QTableWidgetItem* RowWidgetController::getLength()
{
  return m_pLength;
}

ParamWidget* RowWidgetController::getParameters()
{
  return m_pParameters;
}

TypeCombo* RowWidgetController::getType()
{
  return m_pType;
}

KComboBox* RowWidgetController::getVisibility()
{
  return m_pVisibility;
}

KComboBox* RowWidgetController::getAttributes()
{
  return m_pAttributes;
}

QCheckBox* RowWidgetController::getStaticV()
{
  return m_pStaticV;
}

QCheckBox* RowWidgetController::getAbstract()
{
  return m_pAbstract;
}

QCheckBox* RowWidgetController::getConstV()
{
  return m_pConstV;
}

QCheckBox* RowWidgetController::getAutoIncrement()
{
  return m_pAutoIncrement;
}

QCheckBox* RowWidgetController::getAllowNull()
{
  return m_pAllowNull;
}

QCheckBox* RowWidgetController::getIndexed()
{
  return m_pIndexed;
}

void RowWidgetController::setName(QTableWidgetItem* name)
{
  m_pName = name;
}

void RowWidgetController::setStereotype(QTableWidgetItem* steroetype)
{
  m_pStereotype = steroetype;
}

void RowWidgetController::setInitial(QTableWidgetItem* initial)
{
  m_pInitial = initial;
}

void RowWidgetController::setDefaultValue(QTableWidgetItem* defaultValue)
{
  m_pDefaultValue = defaultValue;
}

void RowWidgetController::setLength(QTableWidgetItem* length)
{
  m_pLength = length;
}

void RowWidgetController::setParameters(ParamWidget* params)
{
  m_pParameters = params;
}

void RowWidgetController::setType(TypeCombo* type)
{
  m_pType = type;
}

void RowWidgetController::setVisibility(KComboBox* visibility)
{
  m_pVisibility = visibility;
}

void RowWidgetController::setAttributes(KComboBox* attributes)
{
  m_pAttributes = attributes;
}

void RowWidgetController::setStaticV(QCheckBox* staticV)
{
  m_pStaticV = staticV;
}

void RowWidgetController::setAbstract(QCheckBox* abstract)
{
  m_pAbstract = abstract;
}

void RowWidgetController::setConstV(QCheckBox* constV)
{
  m_pConstV = constV;
}

void RowWidgetController::setAutoIncrement(QCheckBox* autoIncrement)
{
  m_pAutoIncrement = autoIncrement;
}

void RowWidgetController::setAllowNull(QCheckBox* allowNull)
{
  m_pAllowNull = allowNull;
}

void RowWidgetController::setIndexed(QCheckBox* indexed)
{
  m_pIndexed = indexed;
}

