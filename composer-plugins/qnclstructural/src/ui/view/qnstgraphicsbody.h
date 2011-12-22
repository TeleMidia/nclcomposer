#ifndef QNSTGRAPHICSBODY_H
#define QNSTGRAPHICSBODY_H

#include "qnstgraphicscomposition.h"

#include "qnstgraphicscontext.h"
#include "qnstgraphicsswitch.h"
#include "qnstgraphicsimage.h"
#include "qnstgraphicstext.h"
#include "qnstgraphicsaudio.h"
#include "qnstgraphicsvideo.h"
#include "qnstgraphicssettings.h"
#include "qnstgraphicsscript.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicsport.h"

class QnstGraphicsBody : public QnstGraphicsComposition
{
    Q_OBJECT

public:
    QnstGraphicsBody(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsBody();

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

private:
    void createObjects();

    void createConnections();
};

#endif // QNSTGRAPHICSBODY_H
