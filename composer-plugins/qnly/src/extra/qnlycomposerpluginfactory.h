#ifndef QNLYCOMPOSERPLUGINFACTORY_H
#define QNLYCOMPOSERPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

#include "qnlycomposerplugin.h"
using namespace composer::plugin::layout;

namespace composer {
namespace plugin {
namespace layout {

class QnlyComposerPluginFactory : public QObject, public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

public:
    QnlyComposerPluginFactory(QObject* parent = 0);

    ~QnlyComposerPluginFactory();

    IPlugin* createPluginInstance();

    void releasePluginInstance(IPlugin *);

    QList<LanguageType> getSupportedLanguages();

    QString id() const;

    QString name() const;

    QIcon icon() const;

    QWidget* getPreferencePageWidget();

    void setDefaultValues();

    void applyValues();
};

}
}
}

#endif // QNLYCOMPOSERPLUGINFACTORY_H
