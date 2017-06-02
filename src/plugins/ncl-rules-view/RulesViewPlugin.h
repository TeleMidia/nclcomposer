#ifndef RULESVIEWPLUGIN_H
#define RULESVIEWPLUGIN_H

#include <extensions/IPlugin.h>
#include <QMap>

#include "RulesView.h"
#include "RulesTreeWidget.h"

using namespace cpr::core;

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

  void onEntityAdded(const QString &pluginID, Entity *);
  void onEntityChanged(const QString &pluginID, Entity *entity);
  void onEntityRemoved(const QString &pluginID, const QString &entityID);

  void changeSelectedEntity(const QString &pluginID, void *param);

private slots:
  void updateValue(QTreeWidgetItem*);

  void sendAddEntitySignal (QTreeWidgetItem *, int);
  void sendRemoveEntitySignal (QTreeWidgetItem *);
  void sendSelectionChangedSignal();

private:
  void findAllRules (Entity *);
  void releaseItemChildren (QTreeWidgetItem *);

  Project * _currentProject;

  QString * _selectedUId;

  RulesTreeWidget * _rulesTable;
  Entity * _ruleBaseEntity;
  QMap <QTreeWidgetItem *, QString> _items;
};

#endif // RULESVIEWPLUGIN_H
