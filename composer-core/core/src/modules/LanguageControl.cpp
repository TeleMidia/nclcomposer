#include "modules/LanguageControl.h"

namespace composer {
    namespace core {
        namespace module {

        LanguageControl::LanguageControl()
        {

        }

        LanguageControl::~LanguageControl()
        {
            QMap<LanguageType, ILanguageProfile*>::iterator it;

            for(it = profiles.begin(); it != profiles.end(); it++) {
                ILanguageProfile *pf = it.value();
                delete pf;
                pf = NULL;
            }
            profiles.clear();
        }

        void LanguageControl::loadProfiles(QString profilesDirPath)
        {
                //QMutexLocker lock(&lockProfiles);
                ILanguageProfile *lProfile = NULL;

                QDir profileDir = QDir(profilesDirPath);

                if(!profileDir.exists()) {
                    emit notifyError(tr("The Language Profile extension "
                         "directory (%1) does not exist!").arg(profilesDirPath));
                    return;
                }
                QStringList filter;
                filter.append("*.so");
                filter.append("*.dylib");
                filter.append("*.dll");
                profileDir.setNameFilters(filter);

                foreach (QString fileName, profileDir.entryList(QDir::Files
                     | QDir::NoSymLinks)) {

                     QPluginLoader loader(profileDir.absoluteFilePath
                                          (fileName));
                     qDebug() << "trying to load: " << fileName;
                     QObject *profile = loader.instance();
                     if (profile) {
                         lProfile = qobject_cast<ILanguageProfile*> (profile);
                         if (lProfile) {
                             LanguageType type = lProfile->getLanguageType();
                             if (profiles.contains(type))
                             {
                                 qDebug() << "LanguageControl::loadProfiles" <<
                                 "Profile for language (" <<
                                 Utilities::getExtensionForLanguageType(type) <<
                                         ") already exists";
                                 loader.unload();
                             } else {
                               profiles[type] = lProfile;
                               emit notifyLoadedProfile(
                                       lProfile->getProfileName(),fileName);
                             }
                         } else  {
                            qDebug() << "cast:" << loader.errorString();
                            //FIXME: MemoryLeak, but if call unload segfault
                            if (loader.isLoaded()) {
                                qDebug() << "unloading";
                                //loader.unload();
                            }
                         }
                     } else qDebug() << "instance: " << loader.errorString();
                }
        }

        ILanguageProfile* LanguageControl::getProfileFromType
                (LanguageType type)
        {
                if (profiles.contains(type))
                    return profiles[type];
                else return NULL;
        }

        }
    }
}


