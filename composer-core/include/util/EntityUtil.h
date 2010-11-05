#ifndef ENTITYUTIL_H
#define ENTITYUTIL_H

#include <QMap>
#include <QString>
#include <QDomNamedNodeMap>

#include <string>
using namespace std;

#define TOTALENTITIES 2
typedef enum {
      NONE,
      REGION,
      REGIONBASE,
      HEAD,
      BODY,
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
    static map<string,string> toStdMap(QDomNamedNodeMap);


};

#endif // ENTITYUTIL_H
