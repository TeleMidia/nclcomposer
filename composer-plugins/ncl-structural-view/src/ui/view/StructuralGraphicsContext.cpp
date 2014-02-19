#include "StructuralGraphicsContext.h"

StructuralGraphicsContext::StructuralGraphicsContext(StructuralGraphicsEntity* parent)
    : StructuralGraphicsComposition(parent)
{
  setnstType(Structural::Context);

  setColor("#EEEEEE");

  createObjects();
  createConnections();

  dropsrc = "";
}

StructuralGraphicsContext::~StructuralGraphicsContext()
{

}

void StructuralGraphicsContext::createObjects()
{
  menu = new StructuralMenu();
  menu->actionCut->setEnabled(true);
  menu->actionCopy->setEnabled(true);

  menu->actionDelete->setEnabled(true);

  menu->actionExport->setEnabled(true);

  menu->menuInsert->setEnabled(true);

  menu->actionAddMedia->setEnabled(true);
  menu->actionAddContext->setEnabled(true);
  menu->actionAddSwitch->setEnabled(true);
  menu->actionAddPort->setEnabled(true);
  menu->actionAddProperty->setEnabled(true);
  menu->actionAddAggregator->setEnabled(true);

  menu->menuAdjust->setEnabled(true);
  menu->actionAuto->setEnabled(true);
}

void StructuralGraphicsContext::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

  connect(menu, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
  connect(menu, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
  connect(menu, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

  connect(menu, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

  connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

  connect(menu, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
  connect(menu, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
  connect(menu, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
  connect(menu, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));

  connect(menu, SIGNAL(menuAddEntityTriggered(Structural::EntityType)),
                SLOT(createEntity(Structural::EntityType)));

  connect(menu, SIGNAL(autoRequested()), SLOT(adjustWithSpring()));
}
