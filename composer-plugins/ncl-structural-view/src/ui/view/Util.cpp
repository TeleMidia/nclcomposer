#include "Util.h"

#include "Media.h"
#include "Link.h"

/* config */
static std::map <QString, QString> config  =
  create_map<QString, QString >
  (":nst:entity:type", "0")
  (":nst:entity:subtype", "0")

  (":nst:entity:moveable", "1")
  (":nst:entity:resizeable", "1")
  (":nst:entity:selectable", "1")
  (":nst:entity:resizable", "1")
  (":nst:entity:highlightable", "1")


  (":nst:entity:moving", "0")
  (":nst:entity:resizing", "0")

  (":nst:entity:selected", "0")
  (":nst:entity:highlighted", "0")
  (":nst:entity:validated", "0")

  (":nst:entity:top", "0.0")
  (":nst:entity:left", "0.0")
  (":nst:entity:width", "0.0")
  (":nst:entity:height", "0.0")

  (":nst:entity:moveTop", "0.0")
  (":nst:entity:moveLeft", "0.0")

  (":nst:entity:resizeTop", "0.0")
  (":nst:entity:resizeLeft", "0.0")
  (":nst:entity:resizeWidth", "0.0")
  (":nst:entity:resizeHeight", "0.0")

  (":nst:entity:pressTop", "0.0")
  (":nst:entity:pressLeft", "0.0")
  (":nst:entity:pressWidth", "0.0")
  (":nst:entity:pressHeight", "0.0")

  (":nst:entity:anchorWidth", "0.0")
  (":nst:entity:anchorHeight", "0.0")

  (":nst:entity:composition:collapsible", "0")
  (":nst:entity:composition:collapsed", "0");

/* Initialize icon from type Map */
std::map <Qnst::EntitySubtype, QString> QnstUtil::iconFromTypeMap =
  create_map<Qnst::EntitySubtype, QString >
/*
    (Qnst::Text, ":/icon/text")
    (Qnst::Image, ":/icon/image")
    (Qnst::Audio, ":/icon/audio")
    (Qnst::Video, ":/icon/video")
    (Qnst::Html, ":/icon/html")
    (Qnst::NCL, ":/icon/ncl")
    (Qnst::Settings, ":/icon/settings")
    (Qnst::NCLua, ":/icon/script")
*/
    (Qnst::Media, ":/images/icon/media")

    (Qnst::Context, ":/images/icon/context")
    (Qnst::Switch, ":/images/icon/switch")
    (Qnst::Body, ":/images/icon/body")

    (Qnst::Port, ":icon/port")
    (Qnst::SwitchPort, ":icon/switchport")
    (Qnst::Area, ":icon/area");
    //(Qnst::Aggregator, ":icon/aggregator") // \fixme This is here only for
                                           // compatibility with versions prior
                                           // to 0.1.3
    //(Qnst::Link, ":icon/aggregator")

/* Initialize type from extension Map */
std::map <QString, Qnst::EntitySubtype> QnstUtil::typeFromExtMap =
  create_map <QString, Qnst::EntitySubtype>
    ("txt", Qnst::Media)
    ("png", Qnst::Media)
    ("jpg", Qnst::Media)
    ("jpeg", Qnst::Media)
    ("gif", Qnst::Media)

    ("mp3", Qnst::Media)
    ("wav", Qnst::Media)

    ("mp4", Qnst::Media)
    ("mpeg4", Qnst::Media)
    ("mpeg", Qnst::Media)
    ("mpg", Qnst::Media)
    ("mov", Qnst::Media)
    ("avi", Qnst::Media)

    ("htm", Qnst::Media)
    ("html", Qnst::Media)

    ("ncl", Qnst::Media)

    ("lua", Qnst::Media);

/* Initialize map from str type to qnsttype */
std::map <QString, Qnst::EntitySubtype> QnstUtil::typeFromStr =
  create_map <QString, Qnst::EntitySubtype>
    ("body", Qnst::Body)
    ("context", Qnst::Context)
    ("port", Qnst::Port)

    ("switch", Qnst::Switch)
    ("switchPort", Qnst::SwitchPort)
    //("mapping", Qnst::Mapping)

    ("media", Qnst::Media)
    ("area", Qnst::Area)
    ("property", Qnst::Property)

    ("link", Qnst::Link)
    ("bind", Qnst::Bind)
    ("reference", Qnst::Reference);
   // ("bindParam", Qnst::BindParam)

    /*("causalConnector", Qnst::)
    ("simpleCondition", Qnst::Condition)
    ("simpleAction", Qnst::Action)*/

  //  ("aggregator",  Qnst::Aggregator);

std::map <Qnst::EntitySubtype, QString> QnstUtil::strFromType =
    invert_map<Qnst::EntitySubtype, QString>(QnstUtil::typeFromStr);

/* Initialize prefix id from type Map */
std::map <Qnst::EntitySubtype, QString> QnstUtil::prefixIdFromType =
  create_map<Qnst::EntitySubtype, QString >
    (Qnst::Media, "m")
/*
    (Qnst::Image, "m")
    (Qnst::Audio, "m")
    (Qnst::Video, "m")
    (Qnst::Html, "m")
    (Qnst::NCL, "m")
    (Qnst::Settings, "m")
    (Qnst::NCLua, "m")
*/

    (Qnst::Media, "m")

    (Qnst::Context, "ctx")
    (Qnst::Switch, "swt")
    (Qnst::Body, "body")

    (Qnst::Port, "p")
    (Qnst::SwitchPort, "swp")
    (Qnst::Area, "area")

   // (Qnst::Aggregator, "link") // \fixme This is here only for compatibility
                               // with versions prior to 0.1.3.
   (Qnst::Link, "link");

/* Initialize BindType from Role string Map */
std::map <QString, Qnst::BindType> QnstUtil::bindTypeFromRoleStr =
  create_map<QString, Qnst::BindType>
    ("onBegin", Qnst::onBegin)
    ("onEnd", Qnst::onEnd)
    ("onSelection", Qnst::onSelection)
    ("onResume", Qnst::onResume)
    ("onPause", Qnst::onPause)
    ("onBeginAttribution", Qnst::onBeginAttribution)
    ("onEndAttribution", Qnst::onEndAttribution)
    ("onPauseAttribution", Qnst::onPauseAttribution)
    ("onResumeAttribution", Qnst::onResumeAttribution)

    ("start", Qnst::Start)
    ("stop", Qnst::Stop)
    ("resume", Qnst::Resume)
    ("pause", Qnst::Pause)
    ("set", Qnst::Set);

/* Initialize String from Qnst::BindType Map */
std::map <Qnst::BindType, QString> QnstUtil::strFromBindType =
  invert_map<Qnst::BindType, QString> (QnstUtil::bindTypeFromRoleStr);


QnstEntity *QnstUtil::makeGraphicsEntity(Qnst::EntitySubtype type,
                                                 QnstEntity *parent,
                                           const QMap <QString, QString> &props)
{
  QnstEntity *entity = NULL;

  switch (type)
  {
    /*
    case Qnst::Image:
    case Qnst::Audio:
    case Qnst::Video:
    case Qnst::Text:
    case Qnst::NCLua:
    case Qnst::Settings:
    case Qnst::Html:
    case Qnst::NCL:
    */
    case Qnst::Media:
      entity = new QnstMedia(parent);
      entity->setnstSubtype(type);
      break;

    case Qnst::Body:
//      entity = new QnstBody(parent);
      break;

    case Qnst::Context:
//      entity = new QnstContext(parent);
      break;

    case Qnst::Switch:
//      entity = new QnstSwitch(parent);
      break;

    case Qnst::Port:
//      entity = new QnstPort(parent);
      break;

    case Qnst::SwitchPort:
//      entity = new QnstSwitchPort(parent);
      break;

    case Qnst::Area:
//      entity = new QnstArea(parent);
      break;

    case Qnst::Property:
//      entity = new QnstProperty(parent);
      break;

    //case Qnst::Aggregator: // \fixme Aggregator is here only for compatibility
                           // with versions prior to 0.1.3.
   // case Qnst::Link:
     // entity = new QnstLink(parent);
      //break;

    default:
      // do nothing
      // We will return NULL;
      break;
  }

  return entity;
}

Qnst::EntitySubtype QnstUtil::getnstTypeFromMime(const QString &mimeType)
{
  Qnst::EntitySubtype type;
/*
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
  else if (mimeType == "application/x-ncl-settings" ||
           mimeType == "application/x-ginga-settings")
    type = Qnst::Settings;
  else if (mimeType == "application/x-ginga-NCLua")
    type = Qnst::NCLua;
  else
*/
    type = Qnst::Media;

  return type;
}

QString QnstUtil::iconFromMediaType(Qnst::EntitySubtype type)
{
  QString path;
  if(iconFromTypeMap.count(type))
    path = iconFromTypeMap[type];
  else
    path = ":/icon/media";

  return path;
}

Qnst::EntitySubtype QnstUtil::getnstTypeFromExtension(const QString &ext)
{
  if(typeFromExtMap.count(ext))
    return typeFromExtMap[ext];

  return Qnst::Media;
}

Qnst::EntitySubtype QnstUtil::getnstTypeFromStr(const QString &strType)
{
  if(typeFromStr.count(strType))
    return typeFromStr[strType];

  return Qnst::NoSubtype;
}

QString QnstUtil::getStrFromNstType(Qnst::EntitySubtype type)
{
  if(strFromType.count(type))
    return strFromType[type];

  else return "";
}

QString QnstUtil::getPrefixIdFromType(Qnst::EntitySubtype type)
{
  if(prefixIdFromType.count(type))
    return prefixIdFromType[type];

  return "unknown";
}

Qnst::BindType QnstUtil::getBindTypeFromStr(const QString &role)
{
  if(bindTypeFromRoleStr.count(role))
    return bindTypeFromRoleStr[role];

  return Qnst::NoBindType;
}

QString QnstUtil::getStrFromBindType(Qnst::BindType type)
{
  if(strFromBindType.count(type))
    return strFromBindType[type];

  return QString("NoBindType");
}
