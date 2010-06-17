#ifndef ENTITYUTIL_H
#define ENTITYUTIL_H

#include <QMap>
#include <QString>


#define TOTALENTITIES 2
typedef enum {
      NONE,
      REGION,
      REGIONBASE,
}EntityType;

class EntityUtil
{
private:
    static EntityUtil *instance;
    EntityUtil() {}
    ~EntityUtil() {}
    void init();
    static QMap<EntityType,QString> entitiesTypes;

public:
    static QString getEntityName(EntityType entity);
    static EntityType getEntityType(QString entityName);
    static EntityUtil *getInstance();
    static void releaseInstance();


};

#endif // ENTITYUTIL_H
