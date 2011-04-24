#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QtGui/QCheckBox>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QHash>

#include <kcombobox.h>

#include "classifierwidget.h"
#include "classifierlistitem.h"
#include "compactCombo.h"
#include "paramwidget.h"

class QPalette;

class pGuiModel : public QObject {
 Q_OBJECT
  public:
    pGuiModel() :  name(0), parameters(0), type(0), visibility(0), stereotype(0), staticV(0), 
    abstract(0), constV(0), documentation(0), source(0), initial(0), defaultValue(0), classifier(0),
    m_pIsModified(false),attributes(0),autoIncrement(0),allowNull(0),indexed(0),length(0) {}
    QTableWidgetItem* name;
    QTableWidgetItem* stereotype;
    QTableWidgetItem* initial;
    QTableWidgetItem* defaultValue;
    QTableWidgetItem* length;
    ParamWidget* parameters;
    //QPushButton* parameters;
    //QToolButton* addParameters;
    CompactCombo* type;
    KComboBox* visibility;
    KComboBox* attributes;
    QCheckBox* staticV;
    QCheckBox* abstract;
    QCheckBox* constV;
    QCheckBox* autoIncrement;
    QCheckBox* allowNull;
    QCheckBox* indexed;
    QPushButton* documentation;
    QPushButton* source;
    static QHash< QTableWidgetItem*, pGuiModel* > linker; //That suck, but QTableWidgetItems dont have their own signals...

    void connectSlots();
    void cellChanged(QTableWidgetItem* item);
    void setClassifier(UMLClassifierListItem* _classifier);
    void disconnectAndDelete();
    void insertTypesSorted(const QString& currentTypes);
    
  private:
    bool m_pIsModified;
    UMLClassifierListItem* classifier;
    static QPalette* m_greenTint;
    static QPalette* m_redTint;
    static bool init;
    
  public slots:
    //void destroyTableItem(QObject *obj);
    void reload();
       
  private slots:
      void parametersChanged();
      void typeChanged(QString);
      void visibilityChanged(int);
      void staticVChanged(int);
      void abstractChanged(int);
      void attributesChanged(int);
      void constVChanged(int);
      void autoIncrementChanged(int);
      void allowNullChanged(int);
      void indexedChanged(int);
      void documentationChanged(bool);
      void sourceChanged(bool);
  signals:
    void addNew();
    
};

#endif
