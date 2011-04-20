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
bool pGuiModel::init = false;
QPalette* pGuiModel::m_greenTint = 0;
QPalette* pGuiModel::m_redTint = 0;

void pGuiModel::connectSlots() 
{
  if (!init) {
    QPushButton* tmp = new QPushButton(0);
    m_greenTint = new QPalette();
    (*m_greenTint) = tmp->palette();
    QColor greenTint = m_greenTint->color(tmp->backgroundRole());
    greenTint.setGreen(greenTint.green()+10);
    m_greenTint->setColor(tmp->backgroundRole(), greenTint);

    m_redTint = new QPalette();
    (*m_redTint) = tmp->palette();
    QColor redTint = m_redTint->color(tmp->backgroundRole());
    redTint.setRed(redTint.red()+10);
    m_redTint->setColor(tmp->backgroundRole(), redTint);
    delete tmp;
    init = true;
  }
  
  //connect(m_pTopArrowB, SIGNAL( clicked() ), this, SLOT( slotTopClicked() ) );
  //connect(name, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* stereotype, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* initial, SIGNAL(  ), this, SLOT(  ) );
  //connect(QTableWidgetItem* defaultValue, SIGNAL(  ), this, SLOT(  ) );
  if (parameters)
    connect(parameters, SIGNAL( clicked(bool) ), this, SLOT( parametersChanged(bool) ) );
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
  if (documentation)
    connect(documentation, SIGNAL( clicked(bool) ), this, SLOT( documentationChanged(bool) ) );
  if (source)
    connect(source, SIGNAL( clicked(bool) ), this, SLOT( sourceChanged(bool) ) );
}


void pGuiModel::parametersChanged(bool)
{
  if (!m_pIsModified) {
    emit(addNew());
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
    m_pIsModified = true;
  }
  qDebug() << "autoIncrementChanged Changed";
}

void pGuiModel::allowNullChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "allowNull Changed";
}

void pGuiModel::indexedChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "indexed Changed";
}

void pGuiModel::documentationChanged(bool)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  if (classifier) {
    
  }
  qDebug() << "documentationChanged Changed";
}

void pGuiModel::sourceChanged(bool)
{
  if (!m_pIsModified) {
    emit(addNew());
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
      m_pIsModified = true;
    }
    qDebug() << "defaultValue Changed";
  }
  else if (item == stereotype) {
    if (!m_pIsModified) {
      emit(addNew());
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
          QString newText;
          UMLAttributeList paramList = qobject_cast<UMLOperation*>(classifier)->getParmList();
          if (paramList.size()) {
            parameters->setStyleSheet("text-align:left;");
            parameters->setFlat(true);
          }
          foreach (UMLAttribute* attr, paramList) {
            newText += attr->getTypeName() + " " + attr->name();
            if (!(attr->getInitialValue().isEmpty()))
              newText += "(" + attr->getInitialValue() + ")";
            newText +=", ";
          }
          parameters->setText(newText);
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
      
    if (source)
      if (qobject_cast<UMLOperation*>(classifier))
        source->setToolTip(qobject_cast<UMLOperation*>(classifier)->getSourceCode());
    
    if (documentation)
      documentation->setToolTip(classifier->doc());
    
    if (stereotype)
      stereotype->setText(classifier->stereotype());
    
    if (initial)
      if (qobject_cast<UMLAttribute*>(classifier))
        initial->setText(qobject_cast<UMLAttribute*>(classifier)->getInitialValue());
      
    //QTableWidgetItem* defaultValue;//TODO

    connect( classifier, SIGNAL(modified()),this,SLOT(slotListItemModified()) );
    
    if (classifier->doc().isEmpty()) {
      documentation->setPalette(*m_greenTint);
    }
    else {
      documentation->setPalette(*m_redTint);
    }
    
    if (source)
      if (qobject_cast<UMLOperation*>(classifier))
        if (!qobject_cast<UMLOperation*>(classifier)->getSourceCode().isEmpty())
          source->setPalette(*m_greenTint);
        else
          source->setPalette(*m_redTint);
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
  disconnect(documentation, SIGNAL( clicked(bool) ), this, SLOT( documentationChanged(bool) ) );
  disconnect(source, SIGNAL( clicked(bool) ), this, SLOT( sourceChanged(bool) ) );*/
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
