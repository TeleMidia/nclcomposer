#ifndef COMPOSERSETTINGS_H
#define COMPOSERSETTINGS_H

#include <QSettings>

namespace composer {
    namespace gui {

class ComposerSettings: public QSettings
{
public:
  ComposerSettings();
};

} } // end namespace
#endif // COMPOSERSETTINGS_H
