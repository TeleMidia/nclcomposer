#ifndef DOCUMENTTREEITEM_H
#define DOCUMENTTREEITEM_H

#include <QTreeWidgetItem>

class DocumentTreeItem : public QTreeWidgetItem
{
public:
    DocumentTreeItem();
    enum { DocumentType = QTreeWidgetItem::UserType+2};
};

#endif // DOCUMENTTREEITEM_H
