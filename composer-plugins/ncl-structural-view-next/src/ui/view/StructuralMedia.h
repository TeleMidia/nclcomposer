#ifndef QNSTGRAPHICSMEDIA_H
#define QNSTGRAPHICSMEDIA_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QKeyEvent>

#include "StructuralNode.h"

class StructuralMedia : public StructuralNode
{
  Q_OBJECT

public:
  StructuralMedia(StructuralEntity* parent = 0);

  ~StructuralMedia();

  QString getIcon() const;

  void setSource(QString source);

  virtual void setLocalName(LocalName _name);

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
