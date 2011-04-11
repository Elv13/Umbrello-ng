#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QtGui/QCheckBox>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QPushButton>
#include <QHash>

#include <kcombobox.h>

#include "classifierwidget.h"
#include "classifierlistitem.h"

class pGuiModel : public QObject {
 Q_OBJECT
  public:
    pGuiModel() :  name(0), parameters(0), type(0), visibility(0), stereotype(0), staticV(0), 
    abstract(0), constV(0), documentation(0), source(0), initial(0), defaultValue(0), classifier(0),
    m_pIsModified(false),m_pIsReady(false) {}
    QTableWidgetItem* name;
    QPushButton* parameters;
    KComboBox* type;
    KComboBox* visibility;
    QTableWidgetItem* stereotype;
    QCheckBox* staticV;
    QCheckBox* abstract;
    QCheckBox* constV;
    QPushButton* documentation;
    QPushButton* source;
    QTableWidgetItem* initial;
    QTableWidgetItem* defaultValue;
    static QHash< QTableWidgetItem*, pGuiModel* > linker; //That suck, but QTableWidgetItems dont have their own signals...

    void connectSlots();
    void cellChanged(QTableWidgetItem* item);
    void setClassifier(UMLClassifierListItem* _classifier);
    void disconnectAndDelete();
    void setReady();
    
   private:
    bool m_pIsModified;
    bool m_pIsReady;
    UMLClassifierListItem* classifier;
       
  private slots:
      void parametersChanged(bool);
      void typeChanged(QString);
      void visibilityChanged(int);
      void staticVChanged(int);
      void abstractChanged(int);
      void constVChanged(int);
      void documentationChanged(bool);
      void sourceChanged(bool);
      void reload();
  signals:
    void addNew();
    
};

#endif
