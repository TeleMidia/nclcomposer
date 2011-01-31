#ifndef LANGUAGECONTROL_H
#define LANGUAGECONTROL_H

#include <QObject>
#include <QMap>
#include <QPluginLoader>
#include <QDir>

#include "../extensions/ILanguageProfile.h"
using namespace composer::core::extension;

#include "../util/Singleton.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

namespace composer {
    namespace core {
            namespace module {
                    class LanguageControl : public QObject,
                                            public Singleton<LanguageControl>
                    {
                        Q_OBJECT
                        friend class Singleton<LanguageControl>;
                    private:
                        LanguageControl();
                        ~LanguageControl();
                        QMap<LanguageType, ILanguageProfile*> profiles;
                    public:
                        void loadProfiles(QString profilesDirPath);
                        void loadProfile(QString fileName);
                        ILanguageProfile*
                                getProfileFromType(LanguageType type);
                        QList<ILanguageProfile*> getLoadedProfiles();
                    signals:
                        void notifyError(QString);
                        void notifyLoadedProfile(QString name,QString fileName);
                    public slots:

                    };

                }
        }
}

#endif // LANGUAGECONTROL_H
