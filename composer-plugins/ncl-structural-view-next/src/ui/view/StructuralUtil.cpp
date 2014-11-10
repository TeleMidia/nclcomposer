#include "StructuralUtil.h"

#include "StructuralMedia.h"
#include "StructuralLink.h"

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
std::map <Structural::EntityName, QString> StructuralUtil::iconFromTypeMap =
  create_map<Structural::EntityName, QString >

    (Structural::Media, ":/images/icon/media")

    (Structural::Context, ":/images/icon/context")
    (Structural::Switch, ":/images/icon/switch")
    (Structural::Body, ":/images/icon/body")

    (Structural::Port, ":icon/port")
    (Structural::SwitchPort, ":icon/switchport")
    (Structural::Area, ":icon/area");
    //(Qnst::Aggregator, ":icon/aggregator") // \fixme This is here only for
                                           // compatibility with versions prior
                                           // to 0.1.3
    //(Qnst::Link, ":icon/aggregator")

std::map <Structural::MediaTypes, QString> StructuralUtil::iconFromMediaTypeMap =
  create_map<Structural::MediaTypes, QString >
    (Structural::Text, ":/icon/text")
    (Structural::Image, ":/icon/image")
    (Structural::Audio, ":/icon/audio")
    (Structural::Video, ":/icon/video")
    (Structural::Html, ":/icon/html")
    (Structural::NCL, ":/icon/ncl")
    (Structural::Settings, ":/icon/settings")
    (Structural::NCLua, ":/icon/script")
    (Structural::NoMediaType, ":/images/icon/media");


/* Initialize type from extension Map */
std::map <QString, Structural::MediaTypes> StructuralUtil::typeFromExtMap =
  create_map <QString, Structural::MediaTypes>
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
    ("mkv", Structural::Video)

    ("htm", Structural::Html)
    ("html", Structural::Html)

    ("ncl", Structural::NCL)

    ("lua", Structural::NCLua);

/* Initialize map from str type to qnsttype */
std::map <QString, Structural::EntityName> StructuralUtil::typeFromStr =
  create_map <QString, Structural::EntityName>
    ("body", Structural::Body)
    ("context", Structural::Context)
    ("port", Structural::Port)

    ("switch", Structural::Switch)
    ("switchPort", Structural::SwitchPort)
    //("mapping", Qnst::Mapping)

    ("media", Structural::Media)
    ("area", Structural::Area)
    ("property", Structural::Property)

    ("link", Structural::Link)
    ("bind", Structural::Bind)
    ("reference", Structural::Reference);
   // ("bindParam", Qnst::BindParam)

    /*("causalConnector", Qnst::)
    ("simpleCondition", Qnst::Condition)
    ("simpleAction", Qnst::Action)*/

  //  ("aggregator",  Qnst::Aggregator);

std::map <Structural::EntityName, QString> StructuralUtil::strFromType =
    invert_map<Structural::EntityName, QString>(StructuralUtil::typeFromStr);

/* Initialize prefix id from type Map */
std::map <Structural::EntityName, QString> StructuralUtil::prefixIdFromType =
  create_map<Structural::EntityName, QString >
    (Structural::Media, "m")
/*
    (Qnst::Image, "m")
    (Qnst::Audio, "m")
    (Qnst::Video, "m")
    (Qnst::Html, "m")
    (Qnst::NCL, "m")
    (Qnst::Settings, "m")
    (Qnst::NCLua, "m")
*/

    (Structural::Media, "m")

    (Structural::Context, "ctx")
    (Structural::Switch, "swt")
    (Structural::Body, "body")

    (Structural::Port, "p")
    (Structural::SwitchPort, "swp")
    (Structural::Area, "area")

   // (Qnst::Aggregator, "link") // \fixme This is here only for compatibility
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

StructuralEntity *StructuralUtil::makeGraphicsEntity(Structural::EntityName type,
                                                 StructuralEntity *parent,
                                           const QMap <QString, QString> &props)
{
  StructuralEntity *entity = NULL;

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
    case Structural::Media:
      entity = new StructuralMedia(parent);
      entity->setLocalName(type);
      break;

    case Structural::Body:
//      entity = new QnstBody(parent);
      break;

    case Structural::Context:
//      entity = new QnstContext(parent);
      break;

    case Structural::Switch:
//      entity = new QnstSwitch(parent);
      break;

    case Structural::Port:
//      entity = new QnstPort(parent);
      break;

    case Structural::SwitchPort:
//      entity = new QnstSwitchPort(parent);
      break;

    case Structural::Area:
//      entity = new QnstArea(parent);
      break;

    case Structural::Property:
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

Structural::MediaTypes StructuralUtil::getnstTypeFromMime(const QString &mimeType)
{
  Structural::MediaTypes type;

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
    type = Structural::NoMediaType;

  return type;
}

QString StructuralUtil::createUid()
{
  return QUuid::createUuid().toString();
}

QString StructuralUtil::iconFromEntityType(Structural::EntityName type)
{
  QString path;
  if(iconFromTypeMap.count(type))
    path = iconFromTypeMap[type];
  else
    path = ":/icon/media";

  return path;
}

QString StructuralUtil::iconFromMediaType(Structural::MediaTypes type)
{
  QString path;
  if(iconFromMediaTypeMap.count(type))
    path = iconFromMediaTypeMap[type];
  else
    path = ":/icon/media";

  return path;
}

QString StructuralUtil::normalizeXMLID(const QString &id)
{
  QString tmp = id.normalized(QString::NormalizationForm_KD);
  tmp.remove(QRegExp("[^a-zA-Z_-\.\\s]"));
  if(tmp.at(0).isDigit())
    tmp = "_" + tmp;

  return tmp;
}

Structural::MediaTypes StructuralUtil::getnstTypeFromExtension(const QString &ext)
{
  if(typeFromExtMap.count(ext))
    return typeFromExtMap[ext];

  return Structural::NoMediaType;
}

Structural::EntityName StructuralUtil::getnstTypeFromStr(const QString &strType)
{
  if(typeFromStr.count(strType))
    return typeFromStr[strType];

  return Structural::NoName;
}

QString StructuralUtil::getStrFromNstType(Structural::EntityName type)
{
  if(strFromType.count(type))
    return strFromType[type];

  else return "";
}

bool StructuralUtil::isCondition(Structural::BindType btype)
{

  switch (btype) {
    case Structural::onBegin:
    case Structural::onEnd:
    case Structural::onSelection:
    case Structural::onResume:
    case Structural::onPause:
    case Structural::onBeginAttribution:
    case Structural::onEndAttribution:
    case Structural::onPauseAttribution:
    case Structural::onResumeAttribution:
      return true;

      break;
    default:

      return false;
      break;
  }

  return false;
}

QString StructuralUtil::getPrefixIdFromType(Structural::EntityName type)
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
