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

    this->setStyleSheet(/*"background-color:*/ "font-size: 11px;");;
    clear();
    setHeaderLabels(labels);
    header()->setStretchLastSection(false);
    header()->setResizeMode(QHeaderView::ResizeToContents);

    NCLSaxHandler handler(this);
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    bool ret = reader.parse(inputSource);
    this->expandAll();

    return ret;
}

QTreeWidgetItem* NCLTreeWidget::addElement ( QTreeWidgetItem *father,
                                 int pos,
                                 QString tagname,
                                 QString id,
                                 int line_in_text,
                                 int column_in_text)
{
    QTreeWidgetItem *child = new QTreeWidgetItem(father);
    father->insertChild(pos, child);
    child->setText(0, tagname);
    child->setText(1, id);
    child->setText(2, QString::number(line_in_text));
    child->setText(3, QString::number(column_in_text));
    return child;
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

