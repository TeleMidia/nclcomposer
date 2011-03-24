#include "NCLTreeWidget.h"

#include "NCLStructure.h"
#include <QInputDialog>

NCLTreeWidget::NCLTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    setContextMenuPolicy(Qt::ActionsContextMenu);

    createActions();
    createMenus();

    QStringList labels;
    labels << QObject::tr("Element") << QObject::tr("Attributes") << QObject::tr("Element Id");
    setHeaderLabels(labels);
}

NCLTreeWidget::~NCLTreeWidget()
{

}

void NCLTreeWidget::createActions ()
{
    insertNodeAct = new QAction( QIcon(":/images/save.png"),
                                 tr("&Add child"),
                                 this);

    connect( insertNodeAct, SIGNAL(triggered()),
             this, SLOT(userAddNewElement()));
    addAction(insertNodeAct);

    removeNodeAct = new QAction( QIcon(":/images/delete.png"),
                                 tr("&Remove Selected element"),
                                 this);

    connect( removeNodeAct, SIGNAL(triggered()),
             this, SLOT(userRemoveElement()));
    addAction(removeNodeAct);

    expandAllAct = new QAction(tr("Expand All"), this);
    connect(expandAllAct, SIGNAL(triggered()), this, SLOT(expandAll()));
    addAction(expandAllAct);

}

void NCLTreeWidget::createMenus ()
{
    elementMenu = new QMenu(this);
    elementMenu->addAction(insertNodeAct);
    elementMenu->addAction(removeNodeAct);
    elementMenu->addAction(expandAllAct);
}

bool NCLTreeWidget::updateFromText(QString text)
{
    // qDebug() << "MainWindow::updateTreeWidget()";
    QStringList labels;
    labels << QObject::tr("Element") << QObject::tr("Attributes")
            << QObject::tr("id");

    QXmlInputSource inputSource;
    inputSource.setData( text );
    QXmlSimpleReader reader;

    this->setStyleSheet("/*background-color: #ffffff;*/ font-size: 11px;");
    clear();
    setHeaderLabels(labels);
    // header()->setStretchLastSection(false);

    // FIXME: Uncommenting this option brings some performance problems.
    // header()->setResizeMode(QHeaderView::ResizeToContents);

    //TODO: Transform this parser in a puglin
    NCLParser *handler = new NCLParser(this);
    connect (  handler,
               SIGNAL(fatalErrorFound(QString, QString, int, int, int)),
               this,
               SLOT(errorNotification(QString, QString, int, int, int)));

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
                                             QMap <QString, QString> &attrs,
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
    child->setText(2, id);

    QString strAttrList = "";
    QString key;
    foreach (key, attrs.keys())
    {
        strAttrList += " ";
        strAttrList += key + "=\"" + attrs[key] + "\"";
    }
    // qDebug() << strAttrList;
    child->setText(1, strAttrList);

    //child->setText(2, QString::number(line_in_text));
    // child->setText(3, QString::number(column_in_text));

    return child;
}

void NCLTreeWidget::userAddNewElement()
{
    bool ok;
    QList<QTreeWidgetItem*> selecteds = this->selectedItems ();
    QTreeWidgetItem *item = selecteds.at (0);
    QString id = item->text(2);
    // int line = item->text(2).toInt ( &ok, 10 );
    QString tagname = item->text(0);

    QStringList strlist;
    map <QString, char> *
            children = NCLStructure::getInstance()->getChildren(tagname);

    if(children != NULL) {
        map <QString, char>::iterator it;
        for(it = children->begin(); it != children->end(); ++it){
            strlist << it->first;
        }
    }

    QString element = QInputDialog::getItem( this,
                                             tr("Add child"),
                                             tr("Element name:"),
                                             strlist,
                                             0,
                                             true,
                                             &ok );

    if(ok) {
        //Add new Element to OutlineWidget
        QMap<QString,QString> attr;
        emit elementAddedByUser (element, id, attr, false);
    }
}


QTreeWidgetItem *NCLTreeWidget::getItemById(QString itemId)
{
    QList <QTreeWidgetItem*> items = findItems( itemId,
                                               Qt::MatchExactly |
                                               Qt::MatchRecursive, 2);

    if(items.size() > 1)
    {
        qDebug() << "NCLTreeWidget::getItemById Warning - You have more than "
                 << "one item with id='" << itemId
                 << "' - All them will be deleted!";
    }

    for (uint i = 0; i < items.size(); i++)
    {
        if(items.at(i)->text(2) == itemId){
            return items.at(i);
        }
    }
    return NULL;
}

void NCLTreeWidget::removeItem(QString itemId)
{
    QRegExp exp("*");
    QList <QTreeWidgetItem*> items = this->findItems(itemId, Qt::MatchExactly | Qt::MatchRecursive, 2);
    QTreeWidgetItem *item;

    if(items.size() > 1)
    {
        qDebug() << "NCLTreeWidget::removeItem Warning - You have more than "
                 << "one item with id='"<< itemId
                 << "' - All them will be deleted!";
    }
    else if(items.size() == 0)
    {
        qDebug() << "NCLTreeWidget::removeItem Warning! Item with id ='"
                 << itemId << "' was not found!";
    }

    for (int i = 0; i < items.size(); i++)
    {
        item  = items.at(i);

        if (item->parent() != NULL)
            item->parent()->removeChild(item);
        else
            this->removeItemWidget(item, 0);
    }

}

void NCLTreeWidget::userRemoveElement()
{
    QList<QTreeWidgetItem*> selecteds = this->selectedItems ();
    QTreeWidgetItem *item = selecteds.at (0);
    QString id = item->text(2);

    if (item != NULL) {
        int resp = QMessageBox::question(
                      this,
                      tr("Deleting Element"),
                      tr("Do you really want delete the %1 element ?").arg(id),
                      QMessageBox::Yes,
                      QMessageBox::No );

        if(resp) {
            emit elementRemovedByUser(id);
        }
    }
}

void NCLTreeWidget::errorNotification( QString message,
                                       QString filename,
                                       int line,
                                       int column,
                                       int severity)
{
    //this->setStyleSheet("background-color: #aa0000; font-size: 11px;");
    emit parserErrorNotify(message, filename, line, column, severity);
}

