#include "paramwidget.h"


#include "operation.h"
#include "classifierlistitem.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

#include <kicon.h>
#include <klineedit.h>
#include <kcombobox.h>



ParamWidget::ParamWidget(QWidget* parent) : QWidget(parent),
m_pParamPb(0), m_pAddParamPb(0), m_pLineName(0), m_pInOut(0), pInitVal(0), m_pDoc(0), m_pOkButton(0){
  m_pParamPb = new QPushButton(this);
  m_pParamPb->setFlat(true);
  m_pParamPb->setMinimumSize(0,0);
  m_pParamPb->setContentsMargins(0,0,0,0);
  m_pParamPb->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred));
  m_pParamPb->setStyleSheet("text-align:left;");
  m_pParamPb->setFlat(true);
  //TODO ELV aRow->parameters = m_pParamPb;
  
  m_pAddParamPb = new QToolButton(this);
  m_pAddParamPb->setIcon(KIcon("list-add"));
  m_pAddParamPb->setToolTip("Add a new parameter");
  //TODO ELV aRow->addParameters = m_pAddParamPb;
  
  setContentsMargins(0,0,0,0);
  setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred));
  QHBoxLayout* paramLayout = new QHBoxLayout(this);
  paramLayout->setContentsMargins(0,0,0,0);
  paramLayout->addWidget(m_pParamPb);
  paramLayout->addWidget(m_pAddParamPb);
  
  connect(m_pAddParamPb, SIGNAL( clicked(bool) ), this, SLOT( slotAddParamClicked() ) );
  connect(m_pParamPb, SIGNAL( clicked(bool) ), this, SLOT( slotParamClicked() ) );
}

void ParamWidget::slotAddParamClicked()
{
  emit addParamClicked();
  setMinimumPopupWidth(250);
  if (!RowWidgetPopupBase::isVisible()) {
    if (!m_pLineName)
      initPopup();
    showPopup(this,45);
  }
  else
    hidePopup();
}

void ParamWidget::initPopup()
{
  QGridLayout* popupLayout = new QGridLayout(0);
  popupLayout->setContentsMargins(0,0,0,0);
  
  m_pLineName = new KLineEdit(0);
  m_pLineName->setContentsMargins(0,0,0,0);
  m_pLineName->setClickMessage("name");
  popupLayout->addWidget(m_pLineName,0,0,1,1);
  
  m_pInOut = new QComboBox(0);
  m_pInOut->setContentsMargins(0,0,0,0);
  QStringList caseList;
  caseList << "In" << "Out" << "In/Out";
  m_pInOut->addItems(caseList);
  popupLayout->addWidget(m_pInOut,0,1,1,1);
  
  m_pInitVal = new KLineEdit(0);
  m_pInitVal->setContentsMargins(0,0,0,0);
  m_pInitVal->setClickMessage("value");
  m_pInitVal->setMaximumWidth(100);
  popupLayout->addWidget(m_pInitVal,0,2,1,1);
  
  m_pDoc = new KLineEdit(0);
  m_pDoc->setContentsMargins(0,0,0,0);
  m_pDoc->setClickMessage("documentation");
  popupLayout->addWidget(m_pDoc,1,0,1,3);
  
  m_pOkButton = new QPushButton(0);
  m_pOkButton->setMinimumSize(0,0);
  m_pOkButton->setMaximumWidth(30);
  m_pOkButton->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding));
  m_pOkButton->setContentsMargins(0,0,0,0);
  m_pOkButton->setIcon(KIcon("dialog-ok"));
  popupLayout->addWidget(m_pOkButton,0,3,2,1);
  setPopupLayout(popupLayout);
}

void ParamWidget::slotParamClicked()
{
  emit paramClicked();
}

void ParamWidget::setParam(UMLOperation* param)
{
  QString newText;
  UMLAttributeList paramList = param->getParmList();

  foreach (UMLAttribute* attr, paramList) {
    newText += attr->getTypeName() + " " + attr->name();
    if (!(attr->getInitialValue().isEmpty()))
      newText += "(" + attr->getInitialValue() + ")";
    newText +=", ";
  }
  m_pParamPb->setText(newText);
}