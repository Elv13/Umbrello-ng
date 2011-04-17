#ifndef COMPACT_COMBO_H
#define COMPACT_COMBO_H

#include <kcombobox.h>
#include <QSize>

class CompactCombo : public KComboBox {
  Q_OBJECT
  public:
    CompactCombo(QWidget* parent) : KComboBox(parent) {
      setMaximumSize(100,9999);
      setEditable(true);
      setDuplicatesEnabled(false);
      setCompletionMode( KGlobalSettings::CompletionPopup );
      connect(this,SIGNAL(editTextChanged(QString)),this,SLOT(update()));
    }
    virtual void showPopup();
    void setText(const QString &text);
  private slots:
    void update();  
};

#endif