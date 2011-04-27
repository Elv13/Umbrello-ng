#ifndef ROWWIDGETPOPUPBASE_H
#define ROWWIDGETPOPUPBASE_H

#include <QFrame>

class QLayout;
class QRect;
class QFrame;
class QWidget;

class RowWidgetPopupBase {
  friend class PopupPrivate;
public:
  void setMinimumPopupWidth(int width);
  int minimumPopupWidth();
  void hidePopup();
  bool isVisible();
  QRect popupGeometry();
protected:
  RowWidgetPopupBase();
  void showPopup(QWidget* base, int height);
  void setPopupLayout(QLayout* layout);
  QLayout* popupLayout();
  QLayout* m_popupLayout;
  QFrame* m_pPopup;
  virtual bool popupHasFocus() = 0;
  virtual void initPopup() = 0;
private:
  QRect popupGeometry(int screen) const;
  QRect m_pRect;
  int m_pMinimumWidth;
  bool m_pVisible;
};

class PopupPrivate : public QFrame
{
  Q_OBJECT
public:
  PopupPrivate(RowWidgetPopupBase* base, QWidget* parent, Qt::WindowFlags f = 0);
  PopupPrivate(RowWidgetPopupBase* base);
  virtual bool eventFilter(QObject *obj, QEvent *event);
protected:
private:
  RowWidgetPopupBase* m_pBase;
};
#endif