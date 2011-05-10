#ifndef UMLHOLDER_H
#define UMLHOLDER_H

#include <QtGui/QWidget>

class UMLView;
class QTabWidget;
class QHBoxLayout;

/**
 * This class provide a tempoarary way to handle multiple documents view at once
 * @todo Replace this class with proper support for multiple view per model
 */
class UmlHolder : public QWidget {
  Q_OBJECT
public:
  UmlHolder(QTabWidget* tabW, QWidget* parent);
  //virtual ~UmlHolder(); //TODO ELV
  UMLView* view();
  void setView(UMLView* aView);
private:
  static QList<QTabWidget*> m_psTabWidgets;
  static QHash<UMLView* ,QList<UmlHolder*> > m_psViews;
  UMLView* m_pView;
  QTabWidget* m_pTabWidget;
  QHBoxLayout* m_pMainLayout;
public slots:
  void slotTabChanged(int index);
private slots:
  void remove();
};

#endif