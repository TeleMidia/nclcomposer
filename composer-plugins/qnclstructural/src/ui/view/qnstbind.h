#ifndef QNSTBIND_H
#define QNSTBIND_H

#include "qnstentity.h"

#include <QMap>

class QnstBind : public QnstEntity
{
public:
  QnstBind();

  ~QnstBind();

  QString getRole() const;

  void setRole(QString role);

  QString getComponent() const;

  void setComponent(QString component);

  QString getComponentUid() const;

  void setComponentUid(QString componentUid);

  QString getInterface() const;

  void setInterface(QString interface);

  QString getInterfaceUid() const;

  void setInterfaceUid(QString interfaceUid);

  QMap<QString, QString> getParams();

  void setParams(QMap<QString, QString> params);

  QMap<QString, QString> getNameUIDs();

  void setNameUIDs(QMap<QString, QString> name_uids);

private:
  QString role;

  QString component;

  QString componentUID;

  QString interface;

  QString interfaceUID;

  QMap<QString, QString> params;

  QMap<QString, QString> name_uids;
};

#endif // QNSTBIND_H
