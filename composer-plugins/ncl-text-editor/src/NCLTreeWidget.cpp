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

    this->setStyleSheet("/*background-color: #ffffff;*/ font-size: 11px;");
    clear();
    setHeaderLabels(labels);
    header()->setStretchLastSection(false);

    // FIXME:This commented option has
    // header()->setResizeMode(QHeaderView::ResizeToContents);

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

    QTreeWidgetItem *child;

    if(father != 0) {
        child = new QTreeWidgetItem(0);
        int p = pos;
        if(pos == -1)
            p = father->childCount();

        father->insertChild(p, child);
    }
    else {
        child = new QTreeWidgetItem(this);
        if(pos != -1) {
            this->insertTopLevelItem(pos, child);
        }
    }
    QIcon icon;
    if(tagname == "media")
            icon = QIcon (":/images/media.png");
    else if(tagname == "descriptor")
        icon = QIcon (":/images/descriptor.png");
    else if(tagname == "link")
        icon = QIcon (":/images/link-icon.png");
    else
        icon = QIcon (":/images/new.png");

    child->setIcon(0, icon);
    child->setText(0, tagname);
    child->setText(1, id);
    child->setText(2, QString::number(line_in_text));
    child->setText(3, QString::number(column_in_text));

    return child;
}

void NCLTreeWidget::errorNotification(QString message, QString filename, int line, int column, int severity)
{
    //this->setStyleSheet("background-color: #aa0000; font-size: 11px;");
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

