#ifndef QNSTGRAPHICSSWITCH_H
#define QNSTGRAPHICSSWITCH_H

#include "qnstgraphicscomposition.h"

#include "qnstgraphicscontext.h"
#include "qnstgraphicsimage.h"
#include "qnstgraphicstext.h"
#include "qnstgraphicsaudio.h"
#include "qnstgraphicsvideo.h"
#include "qnstgraphicssettings.h"
#include "qnstgraphicsscript.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicsport.h"

class QnstGraphicsSwitch : public QnstGraphicsComposition
{
    Q_OBJECT

public:
    QnstGraphicsSwitch(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsSwitch();

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


#endif // QNSTGRAPHICSSWITCH_H
