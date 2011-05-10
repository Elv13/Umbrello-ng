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
class TypeCombo;

class RowWidgetController : public QObject {
 Q_OBJECT
  public:
    RowWidgetController(Uml::Object_Type itemType, UMLObject *parent = 0, bool ignoreModified = false) :  m_pName(0), m_pParameters(0), m_pType(0), m_pVisibility(0), m_pStereotype(0), m_pStaticV(0), 
    m_pAbstract(0), m_pConstV(0), m_pDocumentation(0), m_pSource(0), m_pInitial(0), m_pDefaultValue(0), classifier(0),
    m_pIsModified(ignoreModified),m_pAttributes(0),m_pAutoIncrement(0),m_pAllowNull(0),m_pIndexed(0),m_pLength(0),m_pParent(parent),m_pRowType(itemType) {}
    UMLObject* m_pParent;
    Uml::Object_Type m_pRowType;
    static QHash< QTableWidgetItem*, RowWidgetController* > linker; //That suck, but QTableWidgetItems dont have their own signals...

    void connectSlots();
    void cellChanged(QTableWidgetItem* item);
    void setClassifier(UMLClassifierListItem* _classifier);
    void disconnectAndDelete();
    void insertTypesSorted(const QString& currentTypes);
    
    PopupButton* getDocumentation();
    void setDocumentation(PopupButton* widget);
    PopupButton* getSource();
    void setSource(PopupButton* widget);
    
    QTableWidgetItem* getName();
    QTableWidgetItem* getStereotype();
    QTableWidgetItem* getInitial();
    QTableWidgetItem* getDefaultValue();
    QTableWidgetItem* getLength();
    ParamWidget* getParameters();
    TypeCombo* getType();
    KComboBox* getVisibility();
    KComboBox* getAttributes();
    QCheckBox* getStaticV();
    QCheckBox* getAbstract();
    QCheckBox* getConstV();
    QCheckBox* getAutoIncrement();
    QCheckBox* getAllowNull();
    QCheckBox* getIndexed();
    void setName(QTableWidgetItem* name);
    void setStereotype(QTableWidgetItem* steroetype);
    void setInitial(QTableWidgetItem* initial);
    void setDefaultValue(QTableWidgetItem* defaultValue);
    void setLength(QTableWidgetItem* length);
    void setParameters(ParamWidget* params);
    void setType(TypeCombo* type);
    void setVisibility(KComboBox* visibility);
    void setAttributes(KComboBox* attributes);
    void setStaticV(QCheckBox* staticV);
    void setAbstract(QCheckBox* abstract);
    void setConstV(QCheckBox* constV);
    void setAutoIncrement(QCheckBox* autoIncrement);
    void setAllowNull(QCheckBox* allowNull);
    void setIndexed(QCheckBox* indexed);

    
    
  private:
    bool m_pIsModified;
    UMLClassifierListItem* classifier;
    PopupButton*           m_pDocumentation;
    PopupButton*           m_pSource;    
    QTableWidgetItem*      m_pName;
    QTableWidgetItem*      m_pStereotype;
    QTableWidgetItem*      m_pInitial;
    QTableWidgetItem*      m_pDefaultValue;
    QTableWidgetItem*      m_pLength;
    ParamWidget*           m_pParameters;
    TypeCombo*             m_pType;
    KComboBox*             m_pVisibility;
    KComboBox*             m_pAttributes;
    QCheckBox*             m_pStaticV;
    QCheckBox*             m_pAbstract;
    QCheckBox*             m_pConstV;
    QCheckBox*             m_pAutoIncrement;
    QCheckBox*             m_pAllowNull;
    QCheckBox*             m_pIndexed;
    
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
