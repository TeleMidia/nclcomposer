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
#include "qnstgraphicshtml.h"
#include "qnstgraphicsncl.h"

class QnstGraphicsContext : public QnstGraphicsComposition
{
    Q_OBJECT

public:
    QnstGraphicsContext(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsContext();

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

#endif // QNSTGRAPHICSCONTEXT_H
