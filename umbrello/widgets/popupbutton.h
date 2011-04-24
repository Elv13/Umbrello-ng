#ifndef POPUPBUTTON_H
#define POPUPBUTTON_H

class PopupButton : public QPushButton, public RowWidgetPopupBase
{
  Q_OBJECT
public:
  PopupButton(QWidget* parent);
  void setGetter(void* function);
  void setSetter(void* function);
private:
  void initPopup();
  KTextEdit* m_pTextEdit;
};
#endif