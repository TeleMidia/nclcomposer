#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QGraphicsView>
#include <QIcon>

#include "abstractscene.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class AbstractView : public QGraphicsView
{
    Q_OBJECT

public:
    AbstractView(QWidget* parent = 0);
    virtual ~AbstractView();

    QVector<AbstractScene*>* getScenes() const;
    AbstractScene* getScene(int index) const;

public slots:
    virtual void createScene() = 0;
    virtual void addScene(AbstractScene* Scene) = 0;
    virtual void removeScene(AbstractScene* Scene) = 0;

private:

    QVector<AbstractScene*>* scenes;
};

}
}
}
}
}

#endif // ABSTRACTVIEW_H
