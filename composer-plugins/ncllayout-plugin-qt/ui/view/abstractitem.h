#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QGraphicsItem>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class AbstractItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    AbstractItem(QGraphicsItem* parent = 0);
    virtual ~AbstractItem();

    QVector<AbstractItem*>* getItems() const;
    AbstractItem* getItem(int index) const;

public slots:
    virtual void addItem(AbstractItem* item) = 0;
    virtual void removeItem(AbstractItem* item) = 0;

signals:
    void itemChanged(AbstractItem*);
    void itemMoved(AbstractItem*);
    void itemSelected(AbstractItem*);
    void itemResized(AbstractItem*);
    void itemAdded(AbstractItem*);
    void itemRemoved(AbstractItem*);
    void itemValidated(AbstractItem*);
    void itemInvalidated(AbstractItem*);

private:
//    QVector<AbstractItem*>* items;
};

}
}
}
}
}

#endif // ABSTRACTITEM_H
