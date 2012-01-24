#ifndef QNSTGRAPHICSCONTEXT_H
#define QNSTGRAPHICSCONTEXT_H

#include "qnstgraphicscomposition.h"

#include "qnstgraphicsswitch.h"
#include "qnstgraphicsimage.h"
#include "qnstgraphicstext.h"
#include "qnstgraphicsaudio.h"
#include "qnstgraphicsvideo.h"
#include "qnstgraphicssettings.h"
#include "qnstgraphicsscript.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicsport.h"
#include "qnstgraphicsaggregator.h"

class QnstGraphicsContext : public QnstGraphicsComposition
{
    Q_OBJECT

public:
    QnstGraphicsContext(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsContext();

public slots:
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

protected:
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

private:
    void createObjects();

    void createConnections();

        QString dropsrc;
};

#endif // QNSTGRAPHICSCONTEXT_H
