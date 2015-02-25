#ifndef QNSTENTITY_H
#define QNSTENTITY_H

#include <QUuid>
#include <QString>
#include <QSet>
#include <QMap>

#include "Structural.h"

typedef Structural::EntityType QnstType;

class StructuralEntity
{
public:
  StructuralEntity(StructuralEntity* parent = 0);

  ~StructuralEntity();

  QString getnstId() const;

  virtual void setnstId(const QString &id);

  QString getnstUid() const;

  void setnstUid(const QString &uid);

  QnstType getnstType() const;

  virtual void setnstType(QnstType type);

  QMap <QString, QString> getUsrData();

  void setUsrData(QMap <QString, QString> userData);

  StructuralEntity* getnstParent() const;

  void setnstParent(StructuralEntity* parent);

  QSet<StructuralEntity*> getnstEntities();

  void addnstEntity(StructuralEntity* entity);

  void removenstEntity(StructuralEntity* entity);

  bool isMedia();

  virtual void getProperties(QMap <QString, QString> &props) { (void) props;}


private:
  QString id;
  QString uid;
  QnstType type;
  StructuralEntity* parent;
  QSet<StructuralEntity*> entities;

  // FIXME: Maybe, this is not needed!! We could use the
  QMap <QString, QString> userData;
};

#endif // QNSTENTITY_H
