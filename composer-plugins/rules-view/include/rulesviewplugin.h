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

  void onEntityAdded(QString, Entity *);
  void onEntityChanged(QString pluginID, Entity *entity);
  void onEntityRemoved(QString, QString entityID);

  void changeSelectedEntity(QString pluginID, void *param);

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
