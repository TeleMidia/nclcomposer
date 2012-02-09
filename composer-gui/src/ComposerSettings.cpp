#include "ComposerSettings.h"
namespace composer {
    namespace gui {
#ifdef Q_WS_MAC
ComposerSettings::ComposerSettings() : QSettings ("br.puc-rio.telemidia", "Composer")
#else
ComposerSettings::ComposerSettings() : QSettings(QSettings::IniFormat, QSettings::UserScope, "telemidia", "composer")
#endif

{

}

} } // end namespace
