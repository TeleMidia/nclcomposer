#ifndef QNSTGRAPHICSBODY_H
#define QNSTGRAPHICSBODY_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>

#include "qnstgraphicscomposition.h"

#include "qnstgraphicscontext.h"
#include "qnstgraphicsswitch.h"
#include "qnstgraphicsport.h"
#include "qnstgraphicsaggregator.h"

class QnstGraphicsBody : public QnstGraphicsComposition
{
    Q_OBJECT

public:
    QnstGraphicsBody(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsBody();

public slots:
    void performHtml();

    void performNCL();

    void performImage();

    void performAudio();

    void performText();

    void performVideo();

    void performScript();

    void performSettings();

    void performMedia();

    void performContext();

    void performSwitch();

    void performPort();

    void performArea();

    void performProperty();

    void performAggregator();

private:
    void createObjects();

    void createConnections();

    QString dropsrc;
};

#endif // QNSTGRAPHICSBODY_H
