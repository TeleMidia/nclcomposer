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

  void setMediaType(Structural::StructuralMedia type);

  void setSource(const QString &source);

  virtual void setStructuralType(StructuralType _subtype);

  virtual void setStructuralProperty(const QString &name, const QString &value);

  QString getSource() const;

  QString getRefer() const;

  void setRefer(QString refer);

  QString getReferUID() const;

  void setReferUID(QString refetUID);

  QString getInstance() const;

  void setInstance(QString instance);

  virtual void refresh();
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

  Structural::StructuralMedia mediatype;
};

#endif // STRUCTURALCONTENT_H
