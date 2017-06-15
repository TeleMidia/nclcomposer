/********************************************************************************
** Form generated from reading UI file 'toolwindowmanager.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLWINDOWMANAGER_H
#define UI_TOOLWINDOWMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qtoolwindowmanager.h"

QT_BEGIN_NAMESPACE

class Ui_ToolWindowManager
{
public:
    QAction *actionSaveState;
    QAction *actionRestoreState;
    QAction *actionClearState;
    QAction *actionClosableTabs;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QToolWindowManager *toolWindowManager;
    QMenuBar *menuBar;
    QMenu *menuToolWindows;
    QMenu *menuOptions;
    QMenu *menuConfig;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ToolWindowManager)
    {
        if (ToolWindowManager->objectName().isEmpty())
            ToolWindowManager->setObjectName(QString::fromUtf8("ToolWindowManager"));
        ToolWindowManager->resize(400, 300);
        actionSaveState = new QAction(ToolWindowManager);
        actionSaveState->setObjectName(QString::fromUtf8("actionSaveState"));
        actionRestoreState = new QAction(ToolWindowManager);
        actionRestoreState->setObjectName(QString::fromUtf8("actionRestoreState"));
        actionClearState = new QAction(ToolWindowManager);
        actionClearState->setObjectName(QString::fromUtf8("actionClearState"));
        actionClosableTabs = new QAction(ToolWindowManager);
        actionClosableTabs->setObjectName(QString::fromUtf8("actionClosableTabs"));
        actionClosableTabs->setCheckable(true);
        actionClosableTabs->setChecked(true);
        centralWidget = new QWidget(ToolWindowManager);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        toolWindowManager = new QToolWindowManager(centralWidget);
        toolWindowManager->setObjectName(QString::fromUtf8("toolWindowManager"));

        verticalLayout->addWidget(toolWindowManager);

        ToolWindowManager->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ToolWindowManager);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        menuToolWindows = new QMenu(menuBar);
        menuToolWindows->setObjectName(QString::fromUtf8("menuToolWindows"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuConfig = new QMenu(menuBar);
        menuConfig->setObjectName(QString::fromUtf8("menuConfig"));
        ToolWindowManager->setMenuBar(menuBar);
        statusBar = new QStatusBar(ToolWindowManager);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ToolWindowManager->setStatusBar(statusBar);

        menuBar->addAction(menuToolWindows->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuConfig->menuAction());
        menuOptions->addAction(actionSaveState);
        menuOptions->addAction(actionRestoreState);
        menuOptions->addAction(actionClearState);
        menuConfig->addAction(actionClosableTabs);

        retranslateUi(ToolWindowManager);

        QMetaObject::connectSlotsByName(ToolWindowManager);
    } // setupUi

    void retranslateUi(QMainWindow *ToolWindowManager)
    {
        ToolWindowManager->setWindowTitle(QApplication::translate("ToolWindowManager", "ToolWindowManager", 0, QApplication::UnicodeUTF8));
        actionSaveState->setText(QApplication::translate("ToolWindowManager", "Save state", 0, QApplication::UnicodeUTF8));
        actionRestoreState->setText(QApplication::translate("ToolWindowManager", "Restore state", 0, QApplication::UnicodeUTF8));
        actionClearState->setText(QApplication::translate("ToolWindowManager", "Clear state and exit", 0, QApplication::UnicodeUTF8));
        actionClosableTabs->setText(QApplication::translate("ToolWindowManager", "Closable tabs", 0, QApplication::UnicodeUTF8));
        menuToolWindows->setTitle(QApplication::translate("ToolWindowManager", "Tool windows", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("ToolWindowManager", "Storage", 0, QApplication::UnicodeUTF8));
        menuConfig->setTitle(QApplication::translate("ToolWindowManager", "Config", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ToolWindowManager: public Ui_ToolWindowManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLWINDOWMANAGER_H
