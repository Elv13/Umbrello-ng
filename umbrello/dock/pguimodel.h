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
#include "popupbutton.h"
#include "enumliteral.h"

class QPalette;

class pGuiModel : public QObject {
 Q_OBJECT
  public:
    pGuiModel(Uml::Object_Type itemType, UMLObject *parent = 0) :  name(0), parameters(0), type(0), visibility(0), stereotype(0), staticV(0), 
    abstract(0), constV(0), m_pDocumentation(0), m_pSource(0), initial(0), defaultValue(0), classifier(0),
    m_pIsModified(false),attributes(0),autoIncrement(0),allowNull(0),indexed(0),length(0),m_pParent(parent),m_pRowType(itemType) {}
    UMLObject* m_pParent;
    Uml::Object_Type m_pRowType;
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
    static QHash< QTableWidgetItem*, pGuiModel* > linker; //That suck, but QTableWidgetItems dont have their own signals...

    void connectSlots();
    void cellChanged(QTableWidgetItem* item);
    void setClassifier(UMLClassifierListItem* _classifier);
    void disconnectAndDelete();
    void insertTypesSorted(const QString& currentTypes);
    
    PopupButton* getDocumentation();
    void setDocumentation(PopupButton* widget);
    PopupButton* getSource();
    void setSource(PopupButton* widget);
    
    
  private:
    bool m_pIsModified;
    UMLClassifierListItem* classifier;
    PopupButton* m_pDocumentation;
    PopupButton* m_pSource;
    
    void init();
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
      void documentationChanged(QString);
      void sourceChanged(QString);
      void slotAddParamater(QString,QString,QString,QString);
  signals:
    void addNew();
    
};

#endif
