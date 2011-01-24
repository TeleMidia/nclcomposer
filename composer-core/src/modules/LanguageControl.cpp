#include "modules/LanguageControl.h"

namespace composer {
    namespace core {
        namespace module {

        LanguageControl::LanguageControl()
        {

        }

        LanguageControl::~LanguageControl()
        {
            QMutexLocker lock(&lockProfiles);
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
                QMutexLocker lock(&lockProfiles);
                ILanguageProfile *lProfile = NULL;

                QDir profileDir = QDir(profilesDirPath);

                if(!profileDir.exists()) {
                    emit notifyError(tr("The Language Profile extension "
                         "directory (%1) does not exist!").arg(profilesDirPath));
                    return;
                }

                foreach (QString fileName, profileDir.entryList(QDir::Files)) {
                     QPluginLoader loader(profileDir.absoluteFilePath(fileName));
                     QObject *profile = loader.instance();
                     if (profile) {
                         lProfile = qobject_cast<ILanguageProfile*> (profile);
                         if (lProfile) {
                             LanguageType type = lProfile->getLanguageType();
                             if (profiles.contains(type)) {
                                 qDebug() << "LanguageControl::loadProfiles" <<
                                 "Profile for language (" <<
                                 Utilities::getExtensionForLanguageType(type) <<
                                         ") already exists";
                             } else {
                               profiles[type] = lProfile;
                               emit notifyLoadedProfile(
                                       lProfile->getProfileName(),fileName);
                             }
                         } else {
                             emit notifyError(tr("ERROR: Language Extension (%1)").
                                              arg(fileName));
                         }
                     } else {
                        emit notifyError(tr("ERROR: Language Extension (%1)").
                                         arg(fileName));
                     }
                }
        }

        ILanguageProfile* LanguageControl::getProfileFromType
                (LanguageType type)
        {
                QMutexLocker lock(&lockProfiles);
                if (profiles.contains(type))
                    return profiles[type];
                else return NULL;
        }

        }
    }
}


