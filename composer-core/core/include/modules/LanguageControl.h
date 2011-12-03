/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef LANGUAGECONTROL_H
#define LANGUAGECONTROL_H

#include <QObject>
#include <QMap>
#include <QPluginLoader>
#include <QDir>

#include "../util/ComposerCoreControl_global.h"

#include "../extensions/ILanguageProfile.h"
using namespace composer::extension;

#include "../util/Singleton.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

namespace composer {
    namespace core {


/*!
 \brief Manages the installed language profiles (though ILanguageProfile
    interface).
*/
class COMPOSERCORESHARED_EXPORT LanguageControl : public QObject
{
    Q_OBJECT
    SINGLETON(LanguageControl)

private:
    /*!
     \brief Constructor (it is private because this class is a Singleton).
    */
    LanguageControl();
    /*!
     \brief Destructor (it is private because this class is a Singleton).
    */
    ~LanguageControl();

    QMap<LanguageType, ILanguageProfile*> profiles; /*!< TODO */
public:
    /*!
     \brief Load all the profiles from a specific directory.

     \param profilesDirPath the directory path from where the profiles must be
        loaded.
    */
    void loadProfiles(QString profilesDirPath);

    /*!
     \brief Load a language profile from a specific fileName.

     \param fileName th
    */
    ILanguageProfile* loadProfile(QString fileName);

    /*!
     \brief

     \param type
    */
    ILanguageProfile* getProfileFromType(LanguageType type);

    /*!
     \brief

     \param type
    */
    bool removeProfile(LanguageType type);

    /*!
     \brief

    */
    QList<ILanguageProfile*> getLoadedProfiles();
signals:
    /*!
     \brief

     \param QString
    */
    void notifyError(QString);
            public slots:

};

} } //end namespace

#endif // LANGUAGECONTROL_H
