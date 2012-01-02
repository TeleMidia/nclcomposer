#ifndef VALIDATORTREEWIDGETITEM_H
#define VALIDATORTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <composer/core/model/Entity.h>

using namespace composer::core::model;

namespace composer{
    namespace plugin{
        namespace validator{

class ValidatorTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit ValidatorTreeWidgetItem(QTreeWidget *view, Entity * entity, QString message);
    Entity * entity () const { return _entity; }
    QString message () const { return _message; }

signals:


private:
    Entity * _entity;
    QString _message;

};
            }
    }
}

#endif // VALIDATORTREEWIDGETITEM_H
