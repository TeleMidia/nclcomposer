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


/* Initialize icon from type Map */
std::map <Qnst::EntityType, QString> QnstUtil::iconFromTypeMap =
  create_map<Qnst::EntityType, QString >
    (Qnst::Text, ":/icon/text")
    (Qnst::Image, ":/icon/image")
    (Qnst::Audio, ":/icon/audio")
    (Qnst::Video, ":/icon/video")
    (Qnst::Html, ":/icon/html")
    (Qnst::NCL, ":/icon/ncl")
    (Qnst::Settings, ":/icon/settings")
    (Qnst::NCLua, ":/icon/script")
    (Qnst::Media, ":/icon/media")

    (Qnst::Context, ":icon/context")
    (Qnst::Switch, ":icon/switch")
    (Qnst::Body, ":icon/body")

    (Qnst::Port, ":icon/port")
    (Qnst::SwitchPort, ":icon/switchport")
    (Qnst::Area, ":icon/area")
    (Qnst::Aggregator, ":icon/aggregator");

/* Initialize type from extension Map */
std::map <QString, Qnst::EntityType> QnstUtil::typeFromExtMap =
  create_map <QString, Qnst::EntityType>
    ("txt", Qnst::Text)

    ("png", Qnst::Image)
    ("jpg", Qnst::Image)
    ("jpeg", Qnst::Image)
    ("gif", Qnst::Image)

    ("mp3", Qnst::Audio)
    ("wav", Qnst::Audio)

    ("mp4", Qnst::Video)
    ("mpeg4", Qnst::Video)
    ("mpeg", Qnst::Video)
    ("mpg", Qnst::Video)
    ("mov", Qnst::Video)
    ("avi", Qnst::Video)

    ("htm", Qnst::Html)
    ("html", Qnst::Html)

    ("ncl", Qnst::NCL)

    ("lua", Qnst::NCLua);

/* Initialize map from str type to qnsttype */
std::map <QString, Qnst::EntityType> QnstUtil::typeFromStr =
  create_map <QString, Qnst::EntityType>
    ("body", Qnst::Body)
    ("context", Qnst::Context)
    ("port", Qnst::Port)

    ("switch", Qnst::Switch)
    ("switchPort", Qnst::SwitchPort)
    ("mapping", Qnst::Mapping)

    ("media", Qnst::Media)
    ("area", Qnst::Area)
    ("property", Qnst::Property)

    ("link", Qnst::Link)
    ("bind", Qnst::Bind)
    ("bindParam", Qnst::BindParam)

    /*("causalConnector", Qnst::)*/
    ("simpleCondition", Qnst::Condition)
    ("simpleAction", Qnst::Action);

/* Initialize prefix id from type Map */
std::map <Qnst::EntityType, QString> QnstUtil::prefixIdFromType =
  create_map<Qnst::EntityType, QString >
    (Qnst::Media, "m")
    (Qnst::Image, "m")
    (Qnst::Audio, "m")
    (Qnst::Video, "m")
    (Qnst::Html, "m")
    (Qnst::NCL, "m")
    (Qnst::Settings, "m")
    (Qnst::NCLua, "m")
    (Qnst::Media, "m")

    (Qnst::Context, "ctx")
    (Qnst::Switch, "swt")
    (Qnst::Body, "body")

    (Qnst::Port, "p")
    (Qnst::SwitchPort, "swp")
    (Qnst::Area, "area")
    (Qnst::Aggregator, "prop");

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
    case Qnst::NCLua:
    case Qnst::Settings:
    case Qnst::Html:
    case Qnst::NCL:
    case Qnst::Media:
      entity = new QnstGraphicsMedia(parent);
      entity->setnstType(type);
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
    type = Qnst::NCLua;
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

Qnst::EntityType QnstUtil::getnstTypeFromExtension(const QString &ext)
{
  if(typeFromExtMap.count(ext))
    return typeFromExtMap[ext];

  return Qnst::Media;
}

Qnst::EntityType QnstUtil::getnstTypeFromStr(const QString &strType)
{
  if(typeFromStr.count(strType))
    return typeFromStr[strType];

  return Qnst::NoType;
}

QString QnstUtil::getPrefixIdFromType(Qnst::EntityType type)
{
  if(prefixIdFromType.count(type))
    return prefixIdFromType[type];

  return "unknown";
}
