#include "util/Utilities.h"

QMap<QString,LanguageType> createMap() {
    QMap<QString,LanguageType> types;
    types["ncl"]      = NCL;
    types["smil"]     = SMIL;
    types["html"]     = HTML;
    return types;
}

namespace composer {
    namespace core {
        namespace util {
            QMap<QString,LanguageType> Utilities::types = createMap();

            LanguageType Utilities::getLanguageTypeByExtension(QString ext)
            {
                if (!types.contains(ext)) return NONE;
                else return types[ext];
            }

            QString Utilities::getExtensionForLanguageType(LanguageType type)
            {
                QMap<QString,LanguageType>::iterator it;
                for (it = types.begin(); it != types.end(); it++)
                    if(type == it.value())
                        return it.key();
                return "";
            }

        }
    }
}

