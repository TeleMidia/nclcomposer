#ifndef PLUGINVALIDATOR_H
#define PLUGINVALIDATOR_H

#include <QWidget>
#include <validation/Validator.h>
#include <core/extensions/IPluginFactory.h>
#include "ValidatorPlugin.h"

using namespace composer::extension;

namespace composer{
    namespace plugin{
        namespace validator{


class ValidatorFactory : public QObject,
                         public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "media_preview_plugin.json")
#endif

public:
    ValidatorFactory(QWidget *parent = 0);
    ~ValidatorFactory();

    IPlugin* createPluginInstance();

    void releasePluginInstance(IPlugin *);

    QList<LanguageType> getSupportedLanguages();

    QString id() const {return "br.ufma.deinf.laws.validator";}

    QString name() const {return "Validator Plugin";}

    QIcon icon() const {return QIcon ();}

    QWidget* getPreferencePageWidget() {return 0;}

    void setDefaultValues() {};

    void applyValues() {};



    /* Informações úteis que devem estar no plugin (em um XML?)*/
    QString version() { return "0.1"; }
    QString compatVersion() {return "0.1";}
    QString vendor() {return "Laws Lab";}
    QString copyright() {return "LAWS/UFMA";}
    QString license() {return "Unknown";}
    QString description() {return "Unknown";}
    QString url() {return "Unknown";}
    QString category() {return "NCL";}

};

        }
    }
}


#endif // PLUGINVALIDATOR_H
