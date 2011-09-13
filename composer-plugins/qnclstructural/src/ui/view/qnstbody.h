#ifndef QNSTBODY_H
#define QNSTBODY_H

#include "qnstcomposite.h"

#include "qnstswitch.h"
#include "qnstcontext.h"
#include "qnstport.h"
#include "qnstproperty.h"
#include "qnstmedia.h"

class QnstBody : public QnstComposite
{
    Q_OBJECT

public:
    QnstBody(QnstNode* parent = 0);

    ~QnstBody();

public slots:
    void addContext();

    void addSwitch();

    void addPort();

    void addProperty();

    void addMedia();

private:
    void createConnection();
};

#endif // QNSTBODY_H
