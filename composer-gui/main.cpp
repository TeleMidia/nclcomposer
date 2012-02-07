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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    QTranslator composerTranslator;
    QString filename = "composer_br";
    filename = filename.toLower();
    composerTranslator.load(filename);
    a.installTranslator(&composerTranslator);

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
    w.setWindowIcon(QIcon(":/mainwindow/icon"));
    w.show();

    return a.exec();
}
