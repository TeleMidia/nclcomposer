#ifndef STRUCTURALMENU_H
#define STRUCTURALMENU_H

#include <QMenu>

#include "StructuralEntity.h"
#include "StructuralUtil.h"

class StructuralEntity;

class StructuralMenu : public QMenu
{
  Q_OBJECT

public:
  StructuralMenu (QWidget *parent = 0);
  virtual ~StructuralMenu ();

  qreal getInsertTop () const;
  void setInsertTop (qreal insertTop);

  qreal getInsertLeft () const;
  void setInsertLeft (qreal insertLeft);

  void adjust (StructuralType type = Structural::NoType);

public slots:
  void switchAutostart (bool state);
  void switchAutostartProperty (bool state);

private:
  qreal _insertTop;
  qreal _insertLeft;
};

#endif // STRUCTURALMENU_H
