#ifndef RULESVIEWFACTORY_H
#define RULESVIEWFACTORY_H

#include <extensions/IPluginFactory.h>

#include "RulesViewPlugin.h"

using namespace composer::extension;

/*!
 * \brief Handles the creation and deletion of RulesViewPlugin objects.
 */
class RulesViewFactory : public QObject,
    public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "ncl-rules-view.json")

public:
  RulesViewFactory();
  ~RulesViewFactory ();

  IPlugin* createPluginInstance() { return new RulesViewPlugin (); }
  void releasePluginInstance(IPlugin *);
  QList<LanguageType> getSupportedLanguages();
  QString id() const { return "br.puc-rio.telemidia.RulesView"; }
  QIcon icon() const { return QIcon (); }

};

#endif // RULESVIEWFACTORY_H
