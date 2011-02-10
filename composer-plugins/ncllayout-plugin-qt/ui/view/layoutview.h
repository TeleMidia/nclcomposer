#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include <QResizeEvent>
#include <QGraphicsView>

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
    void createScene();
    void addScene(QGraphicsScene* scene);
    void removeScene(QGraphicsScene* scene);

protected:
    virtual void resizeEvent(QResizeEvent* event);
};

}
}
}
}
}

#endif // LAYOUTVIEW_H
