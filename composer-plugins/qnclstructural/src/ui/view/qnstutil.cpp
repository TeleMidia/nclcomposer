#include "qnstutil.h"

#include "qnstgraphicsmedia.h"
#include "qnstgraphicsbody.h"
#include "qnstgraphicscontext.h"
#include "qnstgraphicsswitch.h"
#include "qnstgraphicsport.h"
#include "qnstgraphicsswitchport.h"
#include "qnstgraphicsarea.h"
#include "qnstgraphicsproperty.h"
#include "qnstgraphicsaggregator.h"

QnstGraphicsEntity *QnstUtil::makeGraphicsEntity(Qnst::EntityType type,
                                                 QnstGraphicsEntity *parent)
{
  QnstGraphicsEntity *entity = NULL;

  switch (type)
  {
    case Qnst::Image:
    case Qnst::Audio:
    case Qnst::Video:
    case Qnst::Text:
    case Qnst::Script:
    case Qnst::Settings:
    case Qnst::Html:
    case Qnst::NCL:
    case Qnst::Media:
      entity = new QnstGraphicsMedia(parent);
      entity->setnstType(entity->getnstType());
      break;

    case Qnst::Body:
      entity = new QnstGraphicsBody(parent);
      break;

    case Qnst::Context:
      entity = new QnstGraphicsContext(parent);
      break;

    case Qnst::Switch:
      entity = new QnstGraphicsSwitch(parent);
      break;

    case Qnst::Port:
      entity = new QnstGraphicsPort(parent);
      break;

    case Qnst::SwitchPort:
      entity = new QnstGraphicsSwitchPort(parent);
      break;

    case Qnst::Area:
      entity = new QnstGraphicsArea(parent);
      break;

    case Qnst::Property:
      entity = new QnstGraphicsProperty(parent);
       break;

    case Qnst::Aggregator:
      entity = new QnstGraphicsAggregator(parent);
      break;

    default:
      // do nothing
      // We will return NULL;
      break;
  }

  return entity;
}
