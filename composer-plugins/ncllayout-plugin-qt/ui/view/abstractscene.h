#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

#include <QGraphicsScene>

#include "abstractitem.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class AbstractScene : public QGraphicsScene
{
    Q_OBJECT

public:
    AbstractScene(QObject* parent = 0);
    virtual ~AbstractScene();

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

#endif // ABSTRACTSCENE_H
