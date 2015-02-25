#include "StructuralUtil.h"

#include "StructuralGraphicsMedia.h"
#include "StructuralGraphicsBody.h"
#include "StructuralGraphicsContext.h"
#include "StructuralGraphicsSwitch.h"
#include "StructuralGraphicsPort.h"
#include "StructuralGraphicsSwitchPort.h"
#include "StructuralGraphicsArea.h"
#include "StructuralGraphicsProperty.h"
#include "StructuralGraphicsLink.h"

/* Initialize icon from type Map */
std::map <Structural::EntityType, QString> StructuralUtil::iconFromTypeMap =
  create_map<Structural::EntityType, QString >
    (Structural::Text, ":/icon/text")
    (Structural::Image, ":/icon/image")
    (Structural::Audio, ":/icon/audio")
    (Structural::Video, ":/icon/video")
    (Structural::Html, ":/icon/html")
    (Structural::NCL, ":/icon/ncl")
    (Structural::Settings, ":/icon/settings")
    (Structural::NCLua, ":/icon/script")
    (Structural::Stream, ":/icon/stream")
    (Structural::Media, ":/icon/media")

    (Structural::Context, ":icon/context")
    (Structural::Switch, ":icon/switch")
    (Structural::Body, ":icon/body")

    (Structural::Port, ":icon/port")
    (Structural::SwitchPort, ":icon/switchport")
    (Structural::Area, ":icon/area")
    (Structural::Aggregator, ":icon/aggregator") // \fixme This is here only for
                                           // compatibility with versions prior
                                           // to 0.1.3
    (Structural::Link, ":icon/aggregator");

/* Initialize type from extension Map */
std::map <QString, Structural::EntityType> StructuralUtil::typeFromExtMap =
  create_map <QString, Structural::EntityType>
    ("txt", Structural::Text)

    ("png", Structural::Image)
    ("jpg", Structural::Image)
    ("jpeg", Structural::Image)
    ("gif", Structural::Image)

    ("mp3", Structural::Audio)
    ("wav", Structural::Audio)

    ("mp4", Structural::Video)
    ("mpeg4", Structural::Video)
    ("mpeg", Structural::Video)
    ("mpg", Structural::Video)
    ("mov", Structural::Video)
    ("avi", Structural::Video)

    ("htm", Structural::Html)
    ("html", Structural::Html)

    ("ncl", Structural::NCL)

    ("lua", Structural::NCLua);

/* Initialize type from URL Schema */
std::map <QString, Structural::EntityType> StructuralUtil::typeFromUrlSchemaMap =
  create_map <QString, Structural::EntityType>
    ("file", Structural::Media)
    ("http", Structural::Media)
    ("https", Structural::Media)

    ("rtp", Structural::Stream)
    ("rtsp", Structural::Stream)

    ("ncl-mirror", Structural::Media)

    ("sbtvd-ts", Structural::Stream)
    ("isdb-ts", Structural::Stream)
    ("ts", Structural::Stream);

/* Initialize map from str type to qnsttype */
std::map <QString, Structural::EntityType> StructuralUtil::typeFromStr =
  create_map <QString, Structural::EntityType>
    ("body", Structural::Body)
    ("context", Structural::Context)
    ("port", Structural::Port)

    ("switch", Structural::Switch)
    ("switchPort", Structural::SwitchPort)
    ("mapping", Structural::Mapping)

    ("media", Structural::Media)
    ("area", Structural::Area)
    ("property", Structural::Property)

    ("link", Structural::Link)
    ("bind", Structural::Bind)
    ("bindParam", Structural::BindParam)

    /*("causalConnector", Qnst::)
    ("simpleCondition", Qnst::Condition)
    ("simpleAction", Qnst::Action)*/

    ("aggregator",  Structural::Aggregator);

std::map <Structural::EntityType, QString> StructuralUtil::strFromType =
    invert_map<Structural::EntityType, QString>(StructuralUtil::typeFromStr);

/* Initialize prefix id from type Map */
std::map <Structural::EntityType, QString> StructuralUtil::prefixIdFromType =
  create_map<Structural::EntityType, QString >
    (Structural::Media, "m")
    (Structural::Image, "m")
    (Structural::Audio, "m")
    (Structural::Video, "m")
    (Structural::Html, "m")
    (Structural::NCL, "m")
    (Structural::Settings, "m")
    (Structural::NCLua, "m")
    (Structural::Stream, "m")
    (Structural::Media, "m")

    (Structural::Context, "ctx")
    (Structural::Switch, "swt")
    (Structural::Body, "body")

    (Structural::Port, "p")
    (Structural::SwitchPort, "swp")
    (Structural::Area, "area")

    (Structural::Aggregator, "link") // \fixme This is here only for compatibility
                               // with versions prior to 0.1.3.
    (Structural::Link, "link");

/* Initialize BindType from Role string Map */
std::map <QString, Structural::BindType> StructuralUtil::bindTypeFromRoleStr =
  create_map<QString, Structural::BindType>
    ("onBegin", Structural::onBegin)
    ("onEnd", Structural::onEnd)
    ("onSelection", Structural::onSelection)
    ("onResume", Structural::onResume)
    ("onPause", Structural::onPause)
    ("onBeginAttribution", Structural::onBeginAttribution)
    ("onEndAttribution", Structural::onEndAttribution)
    ("onPauseAttribution", Structural::onPauseAttribution)
    ("onResumeAttribution", Structural::onResumeAttribution)

    ("start", Structural::Start)
    ("stop", Structural::Stop)
    ("resume", Structural::Resume)
    ("pause", Structural::Pause)
    ("set", Structural::Set);

/* Initialize String from Qnst::BindType Map */
std::map <Structural::BindType, QString> StructuralUtil::strFromBindType =
  invert_map<Structural::BindType, QString> (StructuralUtil::bindTypeFromRoleStr);


StructuralGraphicsEntity *StructuralUtil::makeGraphicsEntity(Structural::EntityType type,
                                                 StructuralGraphicsEntity *parent,
                                           const QMap <QString, QString> &props)
{
  StructuralGraphicsEntity *entity = NULL;

  switch (type)
  {
    case Structural::Image:
    case Structural::Audio:
    case Structural::Video:
    case Structural::Text:
    case Structural::Settings:
    case Structural::NCLua:
    case Structural::Stream:
    case Structural::Html:
    case Structural::NCL:
    case Structural::Media:
      entity = new StructuralGraphicsMedia(parent);
      entity->setnstType(type);
      break;

    case Structural::Body:
      entity = new StructuralGraphicsBody(parent);
      break;

    case Structural::Context:
      entity = new StructuralGraphicsContext(parent);
      break;

    case Structural::Switch:
      entity = new StructuralGraphicsSwitch(parent);
      break;

    case Structural::Port:
      entity = new StructuralGraphicsPort(parent);
      break;

    case Structural::SwitchPort:
      entity = new StructuralGraphicsSwitchPort(parent);
      break;

    case Structural::Area:
      entity = new StructuralGraphicsArea(parent);
      break;

    case Structural::Property:
      entity = new StructuralGraphicsProperty(parent);
      break;

    case Structural::Aggregator: // \fixme Aggregator is here only for compatibility
                           // with versions prior to 0.1.3.
    case Structural::Link:
      entity = new StructuralGraphicsLink(parent);
      break;

    default:
      // do nothing
      // We will return NULL;
      break;
  }

  if(entity)
    entity->setProperties(props);

  return entity;
}

Structural::EntityType StructuralUtil::getnstTypeFromMime(const QString &mimeType)
{
  Structural::EntityType type;

  if (mimeType.startsWith("image"))
   type = Structural::Image;
  else if (mimeType.startsWith("audio"))
    type = Structural::Audio;
  else if (mimeType.startsWith("video"))
    type = Structural::Video;
  else if (mimeType == "text/html")
    type = Structural::Html;
  else if (mimeType == "application/x-ginga-NCL")
    type = Structural::NCL;
  else if (mimeType.startsWith("text"))
    type = Structural::Text;
  else if (mimeType == "application/x-ncl-settings" ||
           mimeType == "application/x-ginga-settings")
    type = Structural::Settings;
  else if (mimeType == "application/x-ginga-NCLua")
    type = Structural::NCLua;
  else
    type = Structural::Media;

  return type;
}

QString StructuralUtil::iconFromMediaType(Structural::EntityType type)
{
  QString path;
  if(iconFromTypeMap.count(type))
    path = iconFromTypeMap[type];
  else
    path = ":/icon/media";

  return path;
}

Structural::EntityType StructuralUtil::getnstTypeFromExtension(const QString &ext)
{
  if(typeFromExtMap.count(ext))
    return typeFromExtMap[ext];

  return Structural::Media;
}

Structural::EntityType StructuralUtil::getnstTypeFromUrlSchema(const QString &urlSchema)
{
  if(typeFromUrlSchemaMap.count(urlSchema))
    return typeFromUrlSchemaMap[urlSchema];

  return Structural::Media;
}

Structural::EntityType StructuralUtil::getnstTypeFromStr(const QString &strType)
{
  if(typeFromStr.count(strType))
    return typeFromStr[strType];

  return Structural::NoType;
}

QString StructuralUtil::getStrFromNstType(Structural::EntityType type)
{
  if(strFromType.count(type))
    return strFromType[type];

  else return "";
}

QString StructuralUtil::getPrefixIdFromType(Structural::EntityType type)
{
  if(prefixIdFromType.count(type))
    return prefixIdFromType[type];

  return "unknown";
}

Structural::BindType StructuralUtil::getBindTypeFromStr(const QString &role)
{
  if(bindTypeFromRoleStr.count(role))
    return bindTypeFromRoleStr[role];

  return Structural::NoBindType;
}

QString StructuralUtil::getStrFromBindType(Structural::BindType type)
{
  if(strFromBindType.count(type))
    return strFromBindType[type];

  return QString("NoBindType");
}
