#ifndef QNSTCHANGECOMMAND_H
#define QNSTCHANGECOMMAND_H

#include <QUndoCommand>
#include <QMap>

#include "qnstgraphicsentity.h"
#include "qnstview.h"

class QnstView;

class QnstChangeCommand : public QUndoCommand
{
public:
    QnstChangeCommand(QnstView* view, QString uid,
                      const QMap<QString,QString> &properties);

    ~QnstChangeCommand();

    virtual void undo();

    virtual void redo();

private:
    bool ignore;

    QString uid;

    QnstView* view;

    QMap<QString, QString> previous;

    QMap<QString, QString> next;
};
#endif // QNSTCHANGECOMMAND_H
