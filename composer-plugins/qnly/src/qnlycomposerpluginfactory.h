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

    /*!
     \brief Returns the version of Layout View implementation.

     \return QString the version number as string.
    */
    QString version() { return "0.1"; }
    /*!
     \brief Returns the core version that is compatible with this plugin.

     \return QString the core version that is compatible with this plugin
               as a string.
    */
    QString compatVersion() {return "0.1";}
    /*!
     \brief Returns the vendor of Layout View (i.e. Telemidia Lab).

     \return QString the name of the vendor of Layout View.
    */
    QString vendor() {return "Telemidia Lab";}
    /*!
     \brief Returns the copyright of Layout View.

     \return QString the copyright of Layout View.
    */
    QString copyright() {return "Telemidia/PUC-Rio";}
    /*!
      \brief Returns the license of Layout View plugin (i.e. GPLv3).
      \todo The complete license description.

      \return QString the license of Layout View.
    */
    QString license() {return "GPLv3";}

    /*!
      \brief Returns a description of the Layout View

      \return QString the description of Layout View.
    */
    QString description() {return "Layout View enable the user to edit the \
                                NCL regions visually.";}

    QString url() {return "http://composer.telemidia.puc-rio.br/debug";}

    QString category() {return "NCL";}
};

} } } // end namespace

#endif // QNLYCOMPOSERPLUGINFACTORY_H
