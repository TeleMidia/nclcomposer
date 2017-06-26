#include "RulesView.h"

#include <QStringList>
#include <QVBoxLayout>

RulesView::RulesView (QWidget *parent) : QWidget (parent)
{
  _table = new QTreeWidget;

  _table->setColumnCount (4);

  QStringList headers;
  headers << "id"
          << "Variable"
          << "Operator"
          << "Value";

  _table->setHeaderLabels (headers);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget (_table);

  setLayout (mainLayout);
}
