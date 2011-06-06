#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>
#include <QMap>

typedef enum {
      NCL,
      SMIL,
      HTML,
      NONE
}LanguageType;

namespace composer {
    namespace core {
        namespace util {
            class Utilities
            {
            private:
                static QMap<QString,LanguageType> types;
            public:
                static LanguageType getLanguageTypeByExtension(QString ext);
                static QString getExtensionForLanguageType(LanguageType type);
            };


        }
    }
}

#endif // DEFINITIONS_H
