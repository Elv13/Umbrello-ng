#include "pguimodel.h"

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
  qDebug() << "parametersChanged Changed";
}

void pGuiModel::typeChanged(QString)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "typeChanged Changed";
}

void pGuiModel::visibilityChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "visibilityChanged Changed";
}

void pGuiModel::staticVChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "staticVChanged Changed";
}

void pGuiModel::abstractChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "abstractChanged Changed";
}

void pGuiModel::constVChanged(int)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "constVChanged Changed";
}

void pGuiModel::documentationChanged(bool)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "documentationChanged Changed";
}

void pGuiModel::sourceChanged(bool)
{
  if (!m_pIsModified) {
    emit(addNew());
    m_pIsModified = true;
  }
  qDebug() << "sourceChanged Changed";
}

  
void pGuiModel::cellChanged(QTableWidgetItem* item) 
{
  if (item == name) {
    if (!m_pIsModified) {
      emit(addNew());
      m_pIsModified = true;
    }
    if (classifier && m_pIsReady) {
      classifier->setName("test25"); 
    }
    qDebug() << "name Changed";
  }
  else if (item == initial) {
    if (!m_pIsModified) {
      emit(addNew());
      m_pIsModified = true;
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
}

void pGuiModel::setClassifier(UMLClassifierListItem* _classifier)
{
  classifier = _classifier;
  connect(classifier, SIGNAL(modified()),this,SLOT(reload()));
}

void pGuiModel::reload()
{
  name->setText(classifier->name());
  staticV->setChecked(classifier->isStatic());
  abstract->setChecked(classifier->isAbstract());
}

void pGuiModel::disconnectAndDelete()
{
  //TODO ELV
}

/**
 * Prevent a looping
 */
void pGuiModel::setReady()
{
  m_pIsReady = true;
}