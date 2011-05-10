#include "compactCombo.h"
#include <QItemSelectionModel>
#include <QAbstractItemView>

#include <private/qcombobox_p.h>
 
void CompactCombo::showPopup()
{
  view()->setFixedWidth(view()->sizeHintForColumn(0));
  KComboBox::showPopup();
}

void CompactCombo::setText(const QString &text)
{
  lineEdit()->setText(text);
  //setMaximumSize(lineEdit()->sizeHint()); //TODO ELV
}

void CompactCombo::update()
{
  //setMaximumSize(lineEdit()->sizeHint());
  //setMinimumSize(lineEdit()->sizeHint());
}