#ifndef QNSTMEDIA_H
#define QNSTMEDIA_H

#include "qnstcontent.h"

#include "qnstport.h"
#include "qnstarea.h"
#include "qnstproperty.h"

class QnstMedia : public QnstContent
{
    Q_OBJECT

public:
    QnstMedia(QnstNode* parent = 0);

    ~QnstMedia();

public slots:
    void addArea();

private:
    void createConnection();
};

#endif // QNSTMEDIA_H
