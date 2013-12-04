#ifndef QNSTGRAPHICSMEDIA_H
#define QNSTGRAPHICSMEDIA_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QKeyEvent>

#include "qnstnode.h"

class QnstMedia : public QnstNode
{
  Q_OBJECT

public:
  QnstMedia(QnstEntity* parent = 0);

  ~QnstMedia();

  QString getIcon() const;

  void setSource(QString source);

  virtual void setnstSubtype(QnstSubtype subtype);

  QString getSource() const;

  QString getRefer() const;

  void setRefer(QString refer);

  QString getReferUID() const;

  void setReferUID(QString refetUID);

  QString getInstance() const;

  void setInstance(QString instance);

  virtual void updateToolTip();
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
