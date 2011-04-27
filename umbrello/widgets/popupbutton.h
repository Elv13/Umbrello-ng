#ifndef POPUPBUTTON_H
#define POPUPBUTTON_H

#include "rowwidgetpopupbase.h"

#include <QtGui/QPushButton>

#include <ktextedit.h>

class KTextEdit;
class QPushButton;
class QString;

class PopupButton : public QPushButton, public RowWidgetPopupBase
{
  Q_OBJECT
public:
  PopupButton(QWidget* parent);
  void setPopupText(QString text);
  bool isSet();
private:
  KTextEdit* m_pTextEdit;
  QPushButton* m_pOkPB;
  static QPalette* m_greenTint;
  static QPalette* m_redTint;
  static bool init;
protected:
  virtual bool popupHasFocus();
  virtual void initPopup();
private slots:
  void slotClicked();
  void slotTextChanged();
public slots:
  void slotReload();
signals:
  void changed(QString);
};
#endif