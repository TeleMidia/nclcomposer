#ifndef NCLPROBLEMSVIEW_H
#define NCLPROBLEMSVIEW_H

#include <QAction>
#include <QToolBar>

#include <QDockWidget>
#include <QTreeWidget>
#include <QMainWindow>

class NCLProblemsView : public QDockWidget {
    Q_OBJECT

public:
    NCLProblemsView(QWidget *parent = 0);

private:
    void createViewActions();
    void createToolBar();
    void createTreeWidget();
    void createProblemsList();

    QMainWindow *mainWindow;
    QTreeWidget *problemsList;
    QAction *clearProblemsAct;

private slots:
    void clearProblems();

public slots:
    void addProblem (   QString message,
                        QString file,
                        int line,
                        int column = 0,
                        int severity = 0 /*ERROR OR WARNING?*/);
};

#endif
