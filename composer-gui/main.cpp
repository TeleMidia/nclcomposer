/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include <QtGui/QApplication>
#include <QResource>
#include <QObject>
#include "ComposerMainWindow.h"

using namespace composer::gui;

/*!
 \mainpage
This is the online reference for developing with the NCL Composer.
NCL Composer is a multiplatform and flexible multimedia authoring tool
for Interactive Digital TV applications.

\section sec_features Features
List of features

\section sec_tutorials Tutorials
List of tutorials

\section sec_copyright Copyright Policy
NCL Composer is developed by Telemidia/PUC-Rio since 2011 and available over
<a target='_blank' href='http://eclipse.org/legal/epl-v10.html'>
Eclipse Public License</a>.
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    QResource::registerResource("images.qrc");
    QCoreApplication::setOrganizationName("Telemidia Lab");
    QCoreApplication::setOrganizationDomain("telemidia.pucrio.br");
    QCoreApplication::setApplicationName("composer");

    // \todo Remove initModules and readExtensions from ComposerMainWindow
    //    and change splash message in each of this steps.

    //make the library search path include the application dir on windows
    //this is so the plugins can find the dlls they are linked to at run time
    QApplication::addLibraryPath(QApplication::applicationDirPath());

    ComposerMainWindow w(a);
    w.show();

    return a.exec();
}
