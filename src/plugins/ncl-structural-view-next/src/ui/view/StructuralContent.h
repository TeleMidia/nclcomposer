#ifndef STRUCTURALCONTENT_H
#define STRUCTURALCONTENT_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QKeyEvent>

#include "StructuralNode.h"

class StructuralContent : public StructuralNode
{
  Q_OBJECT

public:
  StructuralContent(StructuralEntity* parent = 0);
  ~StructuralContent();

  QString getIcon() const;

  void setMediaType(Structural::StructuralMimeType type);

  void setSource(const QString &source);

  QString getSource() const;

  QString getRefer() const;

  void setRefer(const QString &refer);

  QString getReferUID() const;

  void setReferUID(const QString &refetUID);

  QString getInstance() const;

  void setInstance(const QString &instance);

  virtual void adjust(bool avoidCollision = false,  bool rec = true);

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

  Structural::StructuralMimeType mediatype;
};

#endif // STRUCTURALCONTENT_H
