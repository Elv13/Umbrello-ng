#ifndef PARAMWIDGET_H
#define PARAMWIDGET_H

#include <QtGui/QWidget>
#include "rowwidgetpopupbase.h"

class QPushButton;
class QToolButton;
class UMLOperation;
class KLineEdit;
class QComboBox;

class ParamWidget : public QWidget, public RowWidgetPopupBase
{
  Q_OBJECT
public:
  ParamWidget(QWidget* parent);
  void setParam(UMLOperation* param);
  
private:
  QPushButton* m_pParamPb;
  QToolButton* m_pAddParamPb;
  KLineEdit* m_pLineName;
  QComboBox* m_pInOut;
  KLineEdit* m_pInitVal;
  KLineEdit* m_pDoc;
  QPushButton* m_pOkButton;
  
protected:
  virtual bool popupHasFocus();
  virtual void initPopup();
  
private slots:
  void slotAddParamClicked();
  void slotParamClicked();
  void slotOkClicked();
  
signals:
  void addParamClicked();
  void paramClicked();
  void addParamater(QString name, QString init, QString type, QString doc);
};

#endif