#include "umlholder.h"

#include "umlview.h"

#include <QtGui/QTabWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QWidget>

QList<QTabWidget*> UmlHolder::m_psTabWidgets = QList<QTabWidget*>();
QHash<UMLView* ,QList<UmlHolder*> >  UmlHolder::m_psViews = QHash<UMLView* ,QList<UmlHolder*> >();


UmlHolder::UmlHolder(QTabWidget* tabW, QWidget* parent) : QWidget(parent),m_pView(0),m_pTabWidget(tabW)
{
  m_pMainLayout = new QHBoxLayout(this);
  if (m_psTabWidgets.indexOf(m_pTabWidget) == -1)
    m_psTabWidgets << m_pTabWidget;
}

/*~UmlHolder::UmlHolder()
{
  
}*/
  
void UmlHolder::slotTabChanged(int index)
{
  /*if (!m_pView) {
    remove();
    return;
  }
  foreach (QTabWidget* tabW, m_psTabWidgets) {
    if (tabW != m_pTabWidget) {
      //tabW->setTabEnabled(index,false);
    }
    else {
      foreach (UmlHolder* holder, m_psViews[m_pView]) {
        if (holder != this) {
          foreach (QTabWidget* tabW, m_psTabWidgets) {
            if (tabW->indexOf(holder) != -1) {
              //tabW->setTabEnabled(tabW->indexOf(holder),false);
            }
          }
        }
      }
      tabW->setTabEnabled(index,true);
      //m_pView->setLayout(m_pMainLayout);
      //m_pView->setParent(tabW);
      //m_pMainLayout->addWidget( m_pView);
    }
  }*/
}


UMLView* UmlHolder::view()
{
  return m_pView;
}

void UmlHolder::setView(UMLView* aView)
{
  if (m_psViews[aView].size() != 0) {
    //m_pTabWidget->setEnabled(false);//TODO ELV
  }
  else {
    m_psViews[aView] << this;
  }
  m_pView = aView;
  //m_pView->setParent(m_pTabWidget);
  m_pMainLayout->addWidget(m_pView);
  connect(m_pView,SIGNAL(destroyed()),this,SLOT(remove()));
}

void UmlHolder::remove()
{
  foreach (UmlHolder* holder, m_psViews[m_pView]) {
    foreach (QTabWidget* tabW, m_psTabWidgets) {
      tabW->removeTab(tabW->indexOf(holder));
    }
  }
  m_pView = 0;
}