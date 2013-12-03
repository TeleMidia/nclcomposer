#include "rulesviewfactory.h"

RulesViewFactory::RulesViewFactory()
{
}

RulesViewFactory::~RulesViewFactory ()
{
}

void RulesViewFactory::releasePluginInstance(IPlugin *plugin)
{
  RulesViewPlugin *rulesView = qobject_cast<RulesViewPlugin*>(plugin);

  if (rulesView)
  {
    delete rulesView;
  }
}

QList<LanguageType> RulesViewFactory::getSupportedLanguages()
{
  QList<LanguageType> lTypes;
  lTypes.append(NCL);
  return lTypes;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(RulesView, RulesViewFactory)
#endif
