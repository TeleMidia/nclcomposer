#ifndef QNSTGRAPHICSMEDIA_H
#define QNSTGRAPHICSMEDIA_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QKeyEvent>

#include "qnstgraphicscontent.h"
#include "qnstgraphicsproperty.h"
#include "qnstgraphicsarea.h"

class QnstGraphicsMedia : public QnstGraphicsContent
{
  Q_OBJECT

public:
  QnstGraphicsMedia(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsMedia();

  QString getIcon() const;

  void setSource(QString source);

  void setnstId(QString id);

  virtual void setnstType(QnstType type);

  QString getSource() const;

  QString getRefer() const;

  void setRefer(QString refer);

  QString getReferUID() const;

  void setReferUID(QString refetUID);

  QString getInstance() const;

  void setInstance(QString instance);

  void updateToolTip();

  virtual void setProperties(const QMap <QString, QString> &properties);

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
  void createObjects();

  void createConnections();

  QString refer;

  QString referUID;

  QString instance;

  QString icon;

  QString source;

  bool enableDrag;
};

#endif // QNSTGRAPHICSMEDIA_H
