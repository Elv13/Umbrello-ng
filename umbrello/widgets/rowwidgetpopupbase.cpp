#include "rowwidgetpopupbase.h"

#include "uml.h"

#include <QtGui/QLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QStyle>
#include <QRect>
#include <QApplication>
#include <QPoint>
#include <QKeyEvent>
#include <QEvent>

PopupPrivate::PopupPrivate(RowWidgetPopupBase* base, QWidget* parent, Qt::WindowFlags f) : QFrame(parent,f) {
  m_pBase = base;
}

PopupPrivate::PopupPrivate(RowWidgetPopupBase* base) {
    m_pBase = base;
  }

//Copied from QComboBox
bool PopupPrivate::eventFilter(QObject *obj, QEvent *event)
{
  if ((event->type() != 12) && (event->type() != 77) && (event->type() != 5) && (event->type() != 129))
  if (event->type() == QEvent::FocusOut) {
    if (!m_pBase->popupHasFocus()) {
      m_pBase->hidePopup();
      //exit(0);
      return true;
    }
  }
  return false;
    /*switch (e->type()) {
    case QEvent::ShortcutOverride:
        switch (static_cast<QKeyEvent*>(e)->key()) {
        case Qt::Key_Enter:
          m_pBase->hidePopup();
        case Qt::Key_Return:
#ifdef QT_KEYPAD_NAVIGATION
        case Qt::Key_Select:
#endif
            return true;
        case Qt::Key_Down:
            if (!(static_cast<QKeyEvent*>(e)->modifiers() & Qt::AltModifier))
                break;
            // fall through
        case Qt::Key_F4:
        case Qt::Key_Escape:
            m_pBase->hidePopup();
            return true;
        default:
            break;
        }
        break;
    case QEvent::MouseMove:
        break;
    case QEvent::MouseButtonRelease: {
        QMouseEvent *m = static_cast<QMouseEvent *>(e);
        if (m_pBase->isVisible() && m_pBase->popupGeometry().contains(m->pos())) {
            m_pBase->hidePopup();
            return true;
        }
        break;
    }
    default:
        break;
    }
    return QFrame::eventFilter(obj, event);*/
}

RowWidgetPopupBase::RowWidgetPopupBase() : m_pPopup(0),m_pMinimumWidth(0),m_pVisible(0) {
    m_pPopup = new PopupPrivate(this,UMLApp::app(), Qt::Tool | Qt::Window | Qt::FramelessWindowHint);
    
    m_pPopup->installEventFilter(m_pPopup);
    m_pPopup->setVisible(false);
  };

void RowWidgetPopupBase::showPopup(QWidget* base, int height)
{
  //QStyle * const style = this->style();
  //QRect listRect(style->subControlRect(QStyle::CC_ComboBox, &opt,
                                          //QStyle::SC_ComboBoxListBoxPopup, this));
  QRect listRect;
  QRect screen = popupGeometry(QApplication::desktop()->screenNumber(base)); //Subclasses need to be widgets
  int belowHeight,aboveHeight;
  QPoint above = base->mapToGlobal(QPoint(0,0));
  
  //aboveHeight = above.y() - screen.y();
  
  listRect.setX(above.x());
  
  listRect.setY(above.y()+base->height());
  
  listRect.setHeight(height);
  
  if (m_pMinimumWidth < base->width())
    listRect.setWidth(base->width());
  else
    listRect.setWidth(m_pMinimumWidth);
  
  m_pPopup->setGeometry(listRect);
    
  m_pPopup->show();
  m_pVisible = true;
}

void RowWidgetPopupBase::hidePopup()
{
  if (m_pPopup)
    m_pPopup->setVisible(false);
  m_pVisible = false;
}

void RowWidgetPopupBase::setPopupLayout(QLayout* layout)
{
  m_pPopup->setLayout(layout);
}

QLayout* RowWidgetPopupBase::popupLayout()
{
  return m_popupLayout;
}

QRect RowWidgetPopupBase::popupGeometry(int screen) const
{
#ifdef Q_WS_WIN
    return QApplication::desktop()->screenGeometry(screen);
#elif defined Q_WS_X11
    return QApplication::desktop()->screenGeometry(screen);
#else
    return QApplication::desktop()->availableGeometry(screen);
#endif
}

void RowWidgetPopupBase::setMinimumPopupWidth(int width)
{
  m_pMinimumWidth = width;
}

int RowWidgetPopupBase::minimumPopupWidth()
{
  return m_pMinimumWidth;
}


bool RowWidgetPopupBase::isVisible()
{
  return m_pVisible;
}

QRect RowWidgetPopupBase::popupGeometry() 
{
  return m_pRect;
}