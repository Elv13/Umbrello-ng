#include "popupbutton.h"

#include <QtGui/QVBoxLayout>

#include <kicon.h>
#include <ktextedit.h>

QPalette* PopupButton::m_greenTint = 0;
QPalette* PopupButton::m_redTint = 0;
bool PopupButton::init = false;

PopupButton::PopupButton(QWidget* parent) : QPushButton(parent),m_pTextEdit(0), m_pOkPB(0)
{
  //Create the color for the tinted buttons
  if (!init) {
    QPushButton* tmp = new QPushButton(0);
    m_greenTint = new QPalette();
    (*m_greenTint) = tmp->palette();
    QColor greenTint = m_greenTint->color(tmp->backgroundRole());
    greenTint.setGreen(greenTint.green()+10);
    m_greenTint->setColor(tmp->backgroundRole(), greenTint);

    m_redTint = new QPalette();
    (*m_redTint) = tmp->palette();
    QColor redTint = m_redTint->color(tmp->backgroundRole());
    redTint.setRed(redTint.red()+10);
    m_redTint->setColor(tmp->backgroundRole(), redTint);
    delete tmp;
    init = true;
  }
  
  connect(this,SIGNAL(clicked()),this,SLOT(slotClicked()));
}

void PopupButton::slotClicked()
{
  if (!m_pTextEdit)
    initPopup();
  showPopup(this,150);
}

void PopupButton::slotReload() //TODO ELV dead code
{
  /*if (newText.isEmpty())
    setPalette(*m_greenTint);
  else
    setPalette(*m_redTint);*/
}

void PopupButton::slotTextChanged()
{
  emit changed(m_pTextEdit->text());
}

void PopupButton::setPopupText(QString text)
{
  if (!m_pTextEdit)
    initPopup();
  if (text.isEmpty())
    setPalette(*m_redTint);
  else
    setPalette(*m_greenTint);
  m_pTextEdit->setText(text);
}

void PopupButton::initPopup()
{
  QVBoxLayout* layout = new QVBoxLayout(m_pPopup);
  
  m_pTextEdit = new KTextEdit(m_pPopup);
  m_pTextEdit->installEventFilter(m_pPopup);
  layout->addWidget(m_pTextEdit);
  
  m_pOkPB = new QPushButton(m_pPopup);
  m_pOkPB->installEventFilter(m_pPopup);
  m_pOkPB->setIcon(KIcon("dialog-ok"));
  layout->addWidget(m_pOkPB);
  
  setMinimumPopupWidth(300);
  connect(m_pTextEdit,SIGNAL(textChanged()),this,SLOT(slotTextChanged()));
}

bool PopupButton::isSet()
{
  //return (m_pfSetter && m_pfGetter)?true:false;
  return false;//TODO ELV
}

bool PopupButton::popupHasFocus()
{
  return true;
}