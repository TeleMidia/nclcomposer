#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include <QResizeEvent>
#include <QGraphicsView>


#include "layoutitem.h"
#include "layoutscene.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutView : public QGraphicsView
{
    Q_OBJECT

public:
    LayoutView(QWidget* parent = 0);
    virtual ~LayoutView();

public slots:
    LayoutScene* createScene();
    void addScene(QGraphicsScene* scene);
    void removeScene(QGraphicsScene* scene);

signals:
    void itemAdded(LayoutItem* item);

protected:
    virtual void resizeEvent(QResizeEvent* event);
};

}
}
}
}
}

#endif // LAYOUTVIEW_H
