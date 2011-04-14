//Own header
#include "pguimodel.h"

//app include
#include "attribute.h"
#include "operation.h"

//Qt
#include <QtGui/QLineEdit>

QHash< QTableWidgetItem*, pGuiModel* > pGuiModel::linker;

void pGuiModel::connectSlots() 
{
    //connect(m_pTopArrowB, SIGNAL( clicked() ), this, SLOT( slotTopClicked() ) );
    //connect(name, SIGNAL(  ), this, SLOT(  ) );
    //connect(QTableWidgetItem* stereotype, SIGNAL(  ), this, SLOT(  ) );
    //connect(QTableWidgetItem* initial, SIGNAL(  ), this, SLOT(  ) );
    //connect(QTableWidgetItem* defaultValue, SIGNAL(  ), this, SLOT(  ) );
    connect(parameters, SIGNAL( clicked(bool) ), this, SLOT( parametersChanged(bool) ) );
    connect(type, SIGNAL( editTextChanged(QString) ), this, SLOT( typeChanged(QString) ) );
    connect(visibility, SIGNAL( currentIndexChanged(int) ), this, SLOT( visibilityChanged(int) ) );
    connect(staticV, SIGNAL( stateChanged(int) ), this, SLOT( staticVChanged(int) ) );
    connect(abstract, SIGNAL( stateChanged(int) ), this, SLOT( abstractChanged(int) ) );
    connect(constV, SIGNAL( stateChanged(int) ), this, SLOT( constVChanged(int) ) );
    connect(documentation, SIGNAL( clicked(bool) ), this, SLOT( documentationChanged(bool) ) );
    connect(source, SIGNAL( clicked(bool) ), this, SLOT( sourceChanged(bool) ) );
}


void pGuiModel::parametersChanged(bool)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  if (classifier && m_pIsReady) {
    
  }
  qDebug() << "parametersChanged Changed";
}

void pGuiModel::typeChanged(QString)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  if (classifier && m_pIsReady) {
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
  if (classifier && m_pIsReady) {
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

void pGuiModel::staticVChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  if (classifier && m_pIsReady) {
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
  if (classifier && m_pIsReady) {
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
  if (classifier && m_pIsReady) {
    if (qobject_cast<UMLOperation*>(classifier))
      qobject_cast<UMLOperation*>(classifier)->setConst(constV->isChecked());
  }
  qDebug() << "constVChanged Changed";
}

void pGuiModel::documentationChanged(bool)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  if (classifier && m_pIsReady) {
    
  }
  qDebug() << "documentationChanged Changed";
}

void pGuiModel::sourceChanged(bool)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  if (classifier && m_pIsReady) {
    
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
    if (classifier && m_pIsReady) {
      classifier->setName(name->text()); 
    }
    qDebug() << "name Changed";
  }
  else if (item == initial) {
    if (!m_pIsModified) {
      emit(addNew());
      m_pIsModified = true;
    }
    if (classifier && m_pIsReady) {
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
    if (classifier && m_pIsReady) {
      classifier->setStereotype(stereotype->text());
    }
    qDebug() << "stereotype Changed";
  }
}

void pGuiModel::setClassifier(UMLClassifierListItem* _classifier)
{
  classifier = _classifier;
  connect(classifier, SIGNAL(modified()),this,SLOT(reload()));
}

void pGuiModel::reload()
{
  if (m_pIsReady) {
    if (classifier) {
      if (name) {
        if (name->text() != classifier->name())
          name->setText(classifier->name());//TODO ELV
      }
      if (staticV)
        staticV->setChecked(classifier->isStatic());
      if (abstract)
        abstract->setChecked(classifier->isAbstract());
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
  disconnect(documentation, SIGNAL( clicked(bool) ), this, SLOT( documentationChanged(bool) ) );
  disconnect(source, SIGNAL( clicked(bool) ), this, SLOT( sourceChanged(bool) ) );*/
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
 * Prevent a looping
 */
void pGuiModel::setReady()
{
  m_pIsReady = true;
}