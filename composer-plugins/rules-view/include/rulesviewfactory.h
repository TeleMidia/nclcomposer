#ifndef RULESVIEWFACTORY_H
#define RULESVIEWFACTORY_H

#include <core/extensions/IPluginFactory.h>

#include "rulesviewplugin.h"

using namespace composer::extension;

/*!
 * \brief Handles the creation and deletion of RulesViewPlugin objects.
 */
class RulesViewFactory : public QObject,
    public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "rules_view.json")
#endif

public:
  RulesViewFactory();
  ~RulesViewFactory ();

  IPlugin* createPluginInstance() { return new RulesViewPlugin (); }

  void releasePluginInstance(IPlugin *);

  QList<LanguageType> getSupportedLanguages();

  QString id() const { return "br.puc-rio.telemidia.DebugConsole"; }

  QIcon icon() const { return QIcon (); }

#if QT_VERSION < 0x050000
  /*!
   * \brief Returns the name of the plug-in.
   *
   * \return QString the name of the plug-in.
   */
  QString name() { return "Rules View"; }

  /*!
   * \brief Returns the version of Rules View implementation.
   *
   * \return QString the version number as string.
   */
  QString version() { return NCLCOMPOSER_PLUGINS_VERSION; }

  /*!
   * \brief Returns the core version that is compatible with this plugin.
   *
   * \return QString the core version that is compatible with this plugin
   * as a string.
   */
  QString compatVersion() {return "0.1";}

  /*!
   * \brief Returns the vendor of Rules View (i.e. Telemidia Lab).
   *
   * \return QString the name of the vendor of Rules View.
   */
  QString vendor() {return "Telemidia Lab";}

  /*!
   * \brief Returns the copyright of Rules View.
   *
   * \return QString the copyright of Rules View.
   */
  QString copyright() {return "Telemidia/PUC-Rio";}

  /*!
   * \brief Returns the license of Rules View plugin (i.e. LGPL).
   * \return QString the license of Rules View.
   */
  QString license() {return "LGPL";}

  /*!
   * \brief Returns a description of the Rules View
   *
   * \return QString the description of Rules View.
   */
  QString description() {return "Rules View allows user to"
                                " graphically construct adaptation rules.";}

  /*!
   * \brief Returns the URL to find more information about the plugin.
   *
   * \return QString the description of Rules View.
   */
  QString url() {return "http://composer.telemidia.puc-rio.br/rules";}

  /*!
   * \brief Returns the plugin category.
   *
   * \return QString the description of Rules View.
   */
  QString category() {return "General";}
#endif

};

#endif // RULESVIEWFACTORY_H
