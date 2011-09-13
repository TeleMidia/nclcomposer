#ifndef QNSTCONTEXT_H
#define QNSTCONTEXT_H

#include "qnstcomposite.h"

#include "qnstswitch.h"
#include "qnstport.h"
#include "qnstproperty.h"
#include "qnstmedia.h"

class QnstContext : public QnstComposite
{
    Q_OBJECT

public:
    QnstContext(QnstNode* parent = 0);

    ~QnstContext();

public slots:
    void addContext();

    void addSwitch();

    void addPort();

    void addProperty();

    void addMedia();

private:
    void createConnection();
};

#endif // QNSTCONTEXT_H
