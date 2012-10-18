#ifndef QNSTGRAPHICSPORT_H
#define QNSTGRAPHICSPORT_H

#include "qnstgraphicsinterface.h"

class QnstGraphicsPort : public QnstGraphicsInterface
{
public:
  QnstGraphicsPort(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsPort();

  QString getComponent() const;

  void setComponent(QString component);

  QString getComponentUid() const;

  void setComponentUid(QString componentUid);

  QString getInterface() const;

  void setInterface(QString interface);

  QString getInterfaceUid() const;

  void setInterfaceUid(QString interfaceUid);

  virtual void setProperties(const QMap<QString, QString> &properties);

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

private:
  QString component;

  QString componentUid;

  QString interface;

  QString interfaceUid;
};

#endif // QNSTGRAPHICSPORT_H
