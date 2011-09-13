#ifndef QNSTINTERFACE_H
#define QNSTINTERFACE_H

#include "qncginterface.h"

#include "qnstnode.h"
#include "qnstentity.h"

class QnstNode;

class QnstInterface : public QncgInterface, public QnstEntity
{
    Q_OBJECT

public:
    QnstInterface(QnstNode* parent = 0);

    ~QnstInterface();

    void setName(QString name);

signals:
    void entitySelected(QnstEntity* e);

    void entityAdded(QnstEntity* e);
protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    void createConnections();

private slots:
    void internalselection();
};

#endif // QNSTINTERFACE_H
