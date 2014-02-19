#ifndef QNSTGRAPHICSMAPPING_H
#define QNSTGRAPHICSMAPPING_H

#include "StructuralGraphicsReference.h"

class StructuralGraphicsMapping : public StructuralGraphicsReference
{
public:
  StructuralGraphicsMapping(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsMapping();

  QString getComponent() const;

  void setComponent(QString component);

  QString getComponentUid() const;

  void setComponentUid(QString componentUid);

  QString getSwitchPortUid() const;

  void setSwitchPortUid(QString switchportUid);

  QString getInterface() const;

  void setInterface(QString interface);

  QString getInterfaceUid() const;

  void setInterfaceUid(QString interfaceUid);

  virtual void setProperties(const QMap<QString, QString> &properties);

  virtual void getProperties(QMap<QString, QString> &properties);

private:
  QString component;

  QString componentUid;

  QString switchportUid;

  QString interface;

  QString interfaceUid;
};

#endif // QNSTGRAPHICSMAPPING_H
