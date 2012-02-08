#ifndef MEDIAPREVIEWFACTORY_H
#define MEDIAPREVIEWFACTORY_H

#include <mediapreviewplugin.h>
#include <core/extensions/IPluginFactory.h>
using namespace composer::core;

class MediaPreviewFactory : public QObject, public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)

public:
  ~MediaPreviewFactory();
  MediaPreviewFactory();

  virtual IPlugin* createPluginInstance()
  {
    return new MediaPreviewPlugin();
  }

  virtual void releasePluginInstance(IPlugin *mediaPlug)
  {
    delete mediaPlug;
  }

  virtual QString id() const { return "br.puc-rio.telemidia.mediapreview"; }
  virtual QString name() const {return "Media Preview"; }
};

#endif // MEDIAPREVIEWFACTORY_H
