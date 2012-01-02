#include "validatortreewidgetitem.h"

namespace composer{
    namespace plugin{
        namespace validator{


ValidatorTreeWidgetItem::ValidatorTreeWidgetItem(QTreeWidget *view, Entity *entity, QString message) :
    QTreeWidgetItem(view)
{
    if (entity){
        _entity = entity;
        _message = message;

        setText(1, entity->getType());
        setText(2, message);

        setToolTip(1, entity->getType());
        setToolTip(2, message);
    }
}



        }
    }
}
