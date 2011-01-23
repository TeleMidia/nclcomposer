#include "NCLProblemsView.h"

NCLProblemsView::NCLProblemsView(QWidget *parent):
        QDockWidget("Problems View", parent)
{
    mainWindow = new QMainWindow(parent);

    createViewActions();
    createToolBar();
    createTreeWidget();
}

void NCLProblemsView::createViewActions()
{
    clearProblemsAct = new QAction(QIcon(":/images/clear-icon.png"), tr("&Clear Problems"), this);
    clearProblemsAct->setStatusTip(tr("Clear All"));
    connect(clearProblemsAct, SIGNAL(triggered()), this, SLOT(clearProblems()));
}

void NCLProblemsView::createToolBar()
{
    QToolBar *file = mainWindow->addToolBar(tr("File"));
    file->addAction(clearProblemsAct);
    file->setIconSize(QSize(16, 16));
}

void NCLProblemsView::createTreeWidget()
{
    QStringList labels;
    labels << QObject::tr("Description") << QObject::tr("file") << QObject::tr("line");

    problemsList = new QTreeWidget(this);
    problemsList->setSortingEnabled(1);
    problemsList->setStyleSheet(/*"background-color:*/ "font-size: 11px;");
    problemsList->setHeaderLabels(labels);

    QTreeWidgetItem *lst1 = new QTreeWidgetItem(problemsList);
    lst1->setIcon(0, QIcon(":/images/error-icon-16.png"));
    lst1->setText(0, "Error!");
    lst1->setText(1, "0");

    setFeatures(QDockWidget::DockWidgetMovable |
                             QDockWidget::DockWidgetFloatable);

    mainWindow->setCentralWidget(problemsList);

    setWidget(mainWindow);
}

void NCLProblemsView::clearProblems()
{
    problemsList->clear();
}

//TODO: ERROR OR WARNING ??
void NCLProblemsView::addProblem(   QString message,
                                    QString file,
                                    int line,
                                    int column,
                                    int severity /*ERROR OR WARNING?*/)
{
    QTreeWidgetItem *error = new QTreeWidgetItem(problemsList);
    if(severity == 0)
        error->setIcon(0, QIcon(":/images/error-icon-16.png"));
    else
        error->setIcon(0, QIcon(":/images/warning-big.png"));
    error->setText(0, message);
    error->setText(1, file);
    error->setText(2, QString::number(line));
}

