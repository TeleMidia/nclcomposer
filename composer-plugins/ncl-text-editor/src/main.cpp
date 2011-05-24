/****************************************************************************
**
** Copyright (C) 2004-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QApplication>
#include <QTreeView>

#include "NCLTextEditorMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(ncl_textual_plugin);

    NCLTextEditorMainWindow mainWin;
    mainWin.show();

    app.setStyleSheet(" QMainWindow{ } \
                        QDockWidget {\
                            border: 0.5px solid black;\
                        }\
                        QDockWidget::title {\
                            background: #B9D3EE;\
                            padding-left: 2px;\
                            padding-top: 2px;\
                        }");
    return app.exec();
}
