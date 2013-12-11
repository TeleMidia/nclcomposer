#include "qnstscene.h"

QnstScene::QnstScene(QObject* parent)
    : QGraphicsScene(parent)
{
  createObjects();
  createConnections();
}

QnstScene::~QnstScene()
{

}

QVector<QnstEntity*> QnstScene::getRoots() const
{
  return roots;
}

void QnstScene::addRoot(QnstEntity* entity)
{
  if (entity != NULL)
  {
    roots.append(entity);

    // connect(entity, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
    // connect(entity, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

    connect(entity, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
    connect(entity, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
    connect(entity, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

    connect(entity, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

    connect(entity, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

    connect(entity, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
    connect(entity, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
    connect(entity, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
    connect(entity, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));

//    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
//    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
//    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
//    connect(entity, SIGNAL(entityAboutToChange(QnstEntity*,QMap<QString,QString>)), SIGNAL(entityAboutToChange(QnstEntity*,QMap<QString,QString>)));
//    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addItem(entity);

//    menu->menuInsert->setEnabled(false);
//    menu->actionAddBody->setEnabled(false);
  }
}

void QnstScene::removeRoot(QnstEntity* entity)
{
  if (entity != NULL)
  {
    int index = roots.indexOf(entity);

    if (index >= 0)
    {
      roots.remove(index);

      removeItem(entity);

//      menu->menuInsert->setEnabled(true);
//      menu->actionAddBody->setEnabled(true);
    }
  }
}

void QnstScene::createObjects()
{
  menu = new QnstMenu();
  menu->actionExport->setEnabled(true);

  menu->menuInsert->setEnabled(true);
  menu->actionAddBody->setEnabled(true);
  menu->actionAddContext->setEnabled(true);
  menu->actionAddMedia->setEnabled(true);
  menu->actionAddSwitch->setEnabled(true);
  menu->actionAddArea->setEnabled(true);
  menu->actionAddPort->setEnabled(true);
  menu->actionAddProperty->setEnabled(true);
  menu->actionAddSwitchPort->setEnabled(true);
}

void QnstScene::createConnections()
{
  connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

  // \fixme Even working this is not the correct way to do that
  connect(menu, SIGNAL(menuAddEntityTriggered(QnstSubtype)),
                SLOT(performAdd(QnstSubtype)));
}

void QnstScene::performAdd(QnstSubtype subtype)
{
  QString uid = QUuid::createUuid().toString();
  QString parent = "";
  QMap<QString, QString> properties;
  properties[":nst:subtype"] = QnstUtil::getStrFromNstType(subtype);
//  properties[":nst:top"] = QString::number(height()/2 - DEFAULT_BODY_HEIGHT/2);
//  properties[":nst:left"] = QString::number(width()/2 - DEFAULT_BODY_WIDTH/2);
//  properties[":nst:width"] = QString::number(DEFAULT_BODY_WIDTH);
//  properties[":nst:height"] = QString::number(DEFAULT_BODY_HEIGHT);

  QMap<QString, QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = QUuid::createUuid().toString();

  emit insertEntityResquested(uid, parent, properties, settings);
}

void QnstScene::performBody()
{
  /*********************************************
  QnstGraphicsBody* entity = new QnstGraphicsBody();
  entity->setTop(height()/2 - DEFAULT_BODY_HEIGHT/2);
  entity->setLeft(width()/2 - DEFAULT_BODY_WIDTH/2);
  entity->setWidth(DEFAULT_BODY_WIDTH);
  entity->setHeight(DEFAULT_BODY_HEIGHT);

  addRoot(entity);

  emit entityAdded(entity);
  **************************************************/

  performAdd(Qnst::Body);
}

void QnstScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsScene::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    menu->exec(event->screenPos());

    event->accept();
  }
}
