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
    labels << QObject::tr("Element") << QObject::tr("id") << QObject::tr("Line");
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

