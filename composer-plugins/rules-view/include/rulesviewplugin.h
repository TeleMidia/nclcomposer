#ifndef RULESVIEWPLUGIN_H
#define RULESVIEWPLUGIN_H

#include <IPlugin.h>
#include <QMap>

#include "rulesview.h"
#include "rulestreewidget.h"

using namespace composer::extension;

/*!
 * \brief Rules View is a simple plugin allows users to graphically
 *        define rules for content adapt
 */
class RulesViewPlugin : public IPlugin
{
  Q_OBJECT
public:
  explicit RulesViewPlugin();
  ~RulesViewPlugin ();

  QWidget* getWidget() {return _rulesTable; }


  void addRule (Entity *);

public slots:
  void init();

  void updateValue(QTreeWidgetItem*);

  void onEntityAdded(QString ID, Entity *);
  void onEntityChanged(QString ID, Entity *) {};
  void onEntityRemoved(QString ID, QString entityID) {};

private:
  void findAllRules (Entity *);

  RulesTreeWidget * _rulesTable;
  Entity * _ruleBaseEntity;
  QMap <QTreeWidgetItem *, Entity *> _items;
};

#endif // RULESVIEWPLUGIN_H
