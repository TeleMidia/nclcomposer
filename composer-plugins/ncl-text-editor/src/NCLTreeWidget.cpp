#include "NCLTreeWidget.h"

NCLTreeWidget::NCLTreeWidget(QWidget *parent) : QTreeWidget(parent)
{

}

NCLTreeWidget::~NCLTreeWidget()
{

}

bool NCLTreeWidget::updateFromText(QString text)
{
    qDebug() << "MainWindow::updateTreeWidget()";
    QStringList labels;
    labels << QObject::tr("Element") << QObject::tr("id") << QObject::tr("Line") << QObject::tr("Column");
    QXmlInputSource inputSource;
    inputSource.setData( text );
    QXmlSimpleReader reader;

    this->setStyleSheet(/*"background-color:*/ "font-size: 11px;");
    clear();
    setHeaderLabels(labels);
    header()->setStretchLastSection(false);
    header()->setResizeMode(QHeaderView::ResizeToContents);

    //TODO: Transform this parser in a puglin
    NCLParser *handler = new NCLParser(this);

    connect (  handler,
               SIGNAL(fatalErrorFound(QString,QString,int,int,int)),
               this,
               SLOT(errorNotification(QString,QString,int,int,int)));

    reader.setContentHandler(handler);
    reader.setErrorHandler(handler);
    bool ret = reader.parse(inputSource);
    delete handler;
    this->expandAll();

    return ret;
}

//TODO: Add Element with the correct icon
QTreeWidgetItem* NCLTreeWidget::addElement ( QTreeWidgetItem *father,
                                 int pos,
                                 QString tagname,
                                 QString id,
                                 int line_in_text,
                                 int column_in_text)
{
    QTreeWidgetItem *child = new QTreeWidgetItem(0);

    father->insertChild(0, child);
    child->setText(0, tagname);
    child->setText(1, id);
    child->setText(2, QString::number(line_in_text));
    child->setText(3, QString::number(column_in_text));

    return child;
}

void NCLTreeWidget::errorNotification(QString message, QString filename, int line, int column, int severity)
{
    emit parserErrorNotify(message, filename, line, column, severity);
}

/*
void NCLTreeWidget::showContextMenu(const QPoint& pnt)
{
    QList<QAction *> actions;
    if (m_ui.projectTreeView->indexAt(pnt).isValid()) {
        actions.append(m_addAction);
    }
    if (actions.count() > 0)
        QMenu::exec(actions, m_ui.projectTreeView->mapToGlobal(pnt));
}
*/

