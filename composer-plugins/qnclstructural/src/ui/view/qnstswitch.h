#ifndef QNSTSWITCH_H
#define QNSTSWITCH_H

#include "qnstcomposite.h"

#include "qnstcontext.h"
#include "qnstport.h"
#include "qnstproperty.h"
#include "qnstmedia.h"

class QnstSwitch : public QnstComposite
{
    Q_OBJECT

public:
    QnstSwitch(QnstNode* parent = 0);

    ~QnstSwitch();

public slots:
    void addContext();

    void addSwitch();

    void addPort();

    void addProperty();

    void addMedia();

private:
    void createConnection();
};

#endif // QNSTSWITCH_H
