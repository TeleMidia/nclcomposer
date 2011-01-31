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
                assert(pf != NULL);
                delete pf;
                pf = NULL;
            }
            profiles.clear();
        }

        bool LanguageControl::removeProfile(LanguageType type)
        {
            if (!profiles.contains(type)) return false;
            ILanguageProfile *lp = profiles[type];
            assert(lp != NULL);
            if (lp == NULL) return false;
            delete lp;
            lp = NULL;
            profiles.remove(type);
            return true;
        }

        ILanguageProfile* LanguageControl::loadProfile(QString fileName)
        {
            ILanguageProfile *lProfile = NULL;
            QPluginLoader loader(fileName);
            //qDebug() << "trying to load: " << fileName;
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
                    } else {
                      profiles[type] = lProfile;
                    }
                }
            } else qDebug() << "Failed to load languageControl ("
                            << fileName << ")";
            return lProfile;
        }

        void LanguageControl::loadProfiles(QString profilesDirPath)
        {
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
                loadProfile(profileDir.absoluteFilePath(fileName));
            }
        }

        ILanguageProfile* LanguageControl::getProfileFromType
                (LanguageType type)
        {
                if (profiles.contains(type))
                    return profiles[type];
                else return NULL;
        }

        QList<ILanguageProfile*> LanguageControl::getLoadedProfiles()
        {
            QMap<LanguageType, ILanguageProfile*>::iterator it;
            QList<ILanguageProfile*> list;

            for (it = profiles.begin(); it != profiles.end(); it++)
            {
                list.append(it.value());
            }
            return list;
        }

        }
    }
}


