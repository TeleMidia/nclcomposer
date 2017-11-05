#ifndef RULESVIEWPLUGIN_H
#define RULESVIEWPLUGIN_H

#include <QMap>
#include <extensions/IPlugin.h>
#include <extensions/IPluginFactory.h>
using namespace cpr::core;

#include "RulesTreeWidget.h"
#include "RulesView.h"

/*!
 * \brief Rules View is a simple plugin allows users to graphically define
 * rules for content adapt
 */
class RulesViewPlugin : public IPlugin
{
  Q_OBJECT
public:
  explicit RulesViewPlugin ();
  ~RulesViewPlugin ();

  QWidget *
  widget () override
  {
    return _rulesTable;
  }

  void addRule (Entity *);

public slots:
  void init () override;

  void onEntityAdded (const QString &pluginID, Entity *) override;
  void onEntityChanged (const QString &pluginID, Entity *entity) override;
  void onEntityRemoved (const QString &pluginID,
                        const QString &entityID) override;

  void changeSelectedEntity (const QString &pluginID, void *param);

private slots:
  void updateValue (QTreeWidgetItem *);

  void sendAddEntitySignal (QTreeWidgetItem *, int);
  void sendRemoveEntitySignal (QTreeWidgetItem *);
  void sendSelectionChangedSignal ();

private:
  void findAllRules (Entity *);
  void releaseItemChildren (QTreeWidgetItem *);

  Project *_currentProject;

  QString *_selectedUId;

  RulesTreeWidget *_rulesTable;
  Entity *_ruleBaseEntity;
  QMap<QTreeWidgetItem *, QString> _items;
};

/*!
 * \brief Handles the creation and deletion of RulesViewPlugin objects.
 */
class RulesViewFactory : public QObject,
                         public IPluginFactoryTpl<RulesViewPlugin>
{
  Q_OBJECT
  CPR_PLUGIN_METADATA ("ncl-rules-view.json")

public:
  QString
  id () const override
  {
    return "br.puc-rio.telemidia.RulesView";
  }
  QIcon
  icon () const override
  {
    return QIcon ();
  }
};

#endif // RULESVIEWPLUGIN_H
