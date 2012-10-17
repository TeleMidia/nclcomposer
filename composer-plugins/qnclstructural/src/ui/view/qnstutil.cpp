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

std::map <Qnst::EntityType, QString> QnstUtil::iconFromTypeMap =
    create_map<Qnst::EntityType, QString >
      (Qnst::Image, ":/icon/image")
      (Qnst::Audio, ":/icon/audio")
      (Qnst::Video, ":/icon/video")
      (Qnst::Html, ":/icon/html")
      (Qnst::NCL, ":/icon/ncl")
      (Qnst::Text, ":/icon/text")
      (Qnst::Settings, ":/icon/settings")
      (Qnst::Script, ":/icon/script")
      (Qnst::Media, ":/icon/media");

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

Qnst::EntityType QnstUtil::getnstTypeFromMime(const QString &mimeType)
{
  Qnst::EntityType type;

  if (mimeType.startsWith("image"))
   type = Qnst::Image;
  else if (mimeType.startsWith("audio"))
    type = Qnst::Audio;
  else if (mimeType.startsWith("video"))
    type = Qnst::Video;
  else if (mimeType == "text/html")
    type = Qnst::Html;
  else if (mimeType == "application/x-ginga-NCL")
    type = Qnst::NCL;
  else if (mimeType.startsWith("text"))
    type = Qnst::Text;
  else if (mimeType == "application/x-ncl-settings")
    type = Qnst::Settings;
  else if (mimeType == "application/x-ginga-NCLua")
    type = Qnst::Script;
  else
    type = Qnst::Media;

  return type;
}

QString QnstUtil::iconFromMediaType(Qnst::EntityType type)
{
  QString path;
  if(iconFromTypeMap.count(type))
    path = iconFromTypeMap[type];
  else
    path = ":/icon/media";

  return path;
}
