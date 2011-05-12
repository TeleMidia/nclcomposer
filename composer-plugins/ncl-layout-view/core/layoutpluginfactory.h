#ifndef LAYOUTPLUGINFACTORY_H
#define LAYOUTPLUGINFACTORY_H

#include "core/extensions/IPluginFactory.h"
using namespace composer::core::extension::plugin;

#include "layoutplugin.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutPluginFactory : public QObject, public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

public:
    LayoutPluginFactory();

    virtual ~LayoutPluginFactory();

    virtual IPlugin* createPluginInstance();

    virtual void releasePluginInstance(IPlugin* plugin);

    virtual QList<LanguageType> getSupportedLanguages();

    virtual QString id() const;

    virtual QString name() const;

    virtual QIcon icon() const;

    virtual QWidget* getPreferencePageWidget();

    virtual void setDefaultValues();

    virtual void applyValues();
};

}
}
}
}
}

#endif // LAYOUTPLUGINFACTORY_H
