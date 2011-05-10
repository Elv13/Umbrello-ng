#ifndef TYPE_COMBO_H
#define TYPE_COMBO_H

#include "compactCombo.h"

class UMLClassifierListItem;
class QWidget;

class TypeCombo : public CompactCombo {
    Q_OBJECT
public:
    TypeCombo(QWidget* parent);
    void fill(const QString& currentType, UMLClassifierListItem* classifier);
};

#endif