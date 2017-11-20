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
  virtual ~StructuralMenu () {}
  void adjust (StructuralType type = Structural::NoType);

  void exec (const QPoint &, const StructuralEntity *e = nullptr);

public slots:
  void setAutostartEnabled (bool state);
  void setAutoStartChecked (bool state);
};

#endif // STRUCTURALMENU_H
