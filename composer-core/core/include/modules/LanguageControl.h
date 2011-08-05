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

#include "../extensions/ILanguageProfile.h"
using namespace composer::extension;

#include "../util/Singleton.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

namespace composer {
    namespace core {

class LanguageControl : public QObject
{
    Q_OBJECT
    SINGLETON(LanguageControl)
private:
    LanguageControl();
    ~LanguageControl();
    QMap<LanguageType, ILanguageProfile*> profiles;
public:
    void loadProfiles(QString profilesDirPath);

    ILanguageProfile* loadProfile(QString fileName);

    ILanguageProfile* getProfileFromType(LanguageType type);

    bool removeProfile(LanguageType type);

    QList<ILanguageProfile*> getLoadedProfiles();
signals:
    void notifyError(QString);
            public slots:

};

} } //end namespace

#endif // LANGUAGECONTROL_H
