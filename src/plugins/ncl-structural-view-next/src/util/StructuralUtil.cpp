#include "StructuralUtil.h"

#include "StructuralContent.h"

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
std::map <Structural::StructuralType, QString> StructuralUtil::iconFromTypeMap =
  create_map<Structural::StructuralType, QString >

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

std::map <Structural::StructuralMedia, QString> StructuralUtil::iconFromMediaTypeMap =
  create_map<Structural::StructuralMedia, QString >
    (Structural::Text, ":/icon/text")
    (Structural::Image, ":/icon/image")
    (Structural::Audio, ":/icon/audio")
    (Structural::Video, ":/icon/video")
    (Structural::HTML, ":/icon/html")
    (Structural::NCL, ":/icon/ncl")
    (Structural::Settings, ":/icon/settings")
    (Structural::NCLua, ":/icon/script")
    (Structural::NoContent, ":/images/icon/media");


/* Initialize type from extension Map */
std::map <QString, Structural::StructuralMedia> StructuralUtil::typeFromExtMap =
  create_map <QString, Structural::StructuralMedia>
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

    ("htm", Structural::HTML)
    ("html", Structural::HTML)

    ("ncl", Structural::NCL)

    ("lua", Structural::NCLua);


std::map <QString, Structural::StructuralRole> StructuralUtil::roleTypeFromRoleStr =
  create_map<QString, Structural::StructuralRole>
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

/* Initialize String from Qnst::StructuralRole Map */
std::map <Structural::StructuralRole, QString> StructuralUtil::strFromRoleType =
  invert_map<Structural::StructuralRole, QString> (StructuralUtil::roleTypeFromRoleStr);

/* Initialize map from str type to qnsttype */
std::map <QString, Structural::StructuralType> StructuralUtil::typeFromStr =
  create_map <QString, Structural::StructuralType>
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
    ("bind", Structural::Bind);

   // ("bindParam", Qnst::BindParam)

    /*("causalConnector", Qnst::)
    ("simpleCondition", Qnst::Condition)
    ("simpleAction", Qnst::Action)*/

  //  ("aggregator",  Qnst::Aggregator);

std::map <Structural::StructuralType, QString> StructuralUtil::strFromType =
    invert_map<Structural::StructuralType, QString>(StructuralUtil::typeFromStr);

/* Initialize prefix id from type Map */
std::map <Structural::StructuralType, QString> StructuralUtil::prefixIdFromType =
  create_map<Structural::StructuralType, QString >
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
    (Structural::Property, "prop")
    (Structural::SwitchPort, "swp")
    (Structural::Area, "area")
    (Structural::Link, "link")

   // (Qnst::Aggregator, "link") // \fixme This is here only for compatibility
                               // with versions prior to 0.1.3.
   ;

StructuralEntity *StructuralUtil::makeGraphicsEntity(Structural::StructuralType type,
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
      entity = new StructuralContent(parent);
      entity->setStructuralType(type);
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

QMap<QString,QString> StructuralUtil::createSettings(bool undo, bool notify)
{
   return createSettings((undo ? "1" : "0"), (notify ? "1" : "0"), CreateUid());
}

QMap<QString,QString> StructuralUtil::createSettings(const QString &undo, const QString &notify, const QString &code)
{
  QMap<QString,QString> settings;

  if (undo.isEmpty())
    settings.insert(PLG_SETTING_UNDO,"1");
  else
    settings.insert(PLG_SETTING_UNDO,undo);

  if (notify.isEmpty())
    settings.insert(PLG_SETTING_NOTIFY,"1");
  else
    settings.insert(PLG_SETTING_NOTIFY,notify);

  if (code.isEmpty())
    settings.insert(PLG_SETTING_CODE,CreateUid());
  else
    settings.insert(PLG_SETTING_CODE,code);

  return settings;
}

bool StructuralUtil::hasValidKinshp(StructuralType entityType, StructuralType parentType)
{
  switch (entityType) {
    case Structural::Body:
    {
      return false;
      break;
    }

    case Structural::Media:
    case Structural::Switch:
    case Structural::Context:
    {
      switch (parentType) {
        case Structural::Body:
        case Structural::Switch:
        case Structural::Context:
        {
          return true;
          break;
        }
        default:
        {
          return false;
          break;
        }
      }
      break;
    }

    default:
    {
      return false;
      break;
    }
  }
}

QString StructuralUtil::convertType(StructualCategory type)
{
  return QString::number(type);
}

QString StructuralUtil::convertSubtype(StructuralType subtype)
{
  return QString::number(subtype);
}

Structural::StructuralMedia StructuralUtil::getnstTypeFromMime(const QString &mimeType)
{
  Structural::StructuralMedia type;

  if (mimeType.startsWith("image"))
   type = Structural::Image;
  else if (mimeType.startsWith("audio"))
    type = Structural::Audio;
  else if (mimeType.startsWith("video"))
    type = Structural::Video;
  else if (mimeType == "text/html")
    type = Structural::HTML;
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
    type = Structural::NoContent;

  return type;
}

void StructuralUtil::dbg(QObject* object, const QString &message)
{
  qDebug() << "[PLUGIN][STR]" << ":" << object->metaObject()->className() << ":" << message;
}

QMap<QString,QString> StructuralUtil::createCoreTranslationMap(StructuralType type)
{
  QMap<QString, QString> m;

  switch (type)
  {
    case Structural::Body:
    {
      m.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);

      break;
    }

    case Structural::Context:
    case Structural::Switch:
    {
      m.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);
      m.insert(NCL_ENTITY_REFER, NCL_ENTITY_REFER);

      break;
    }

    case Structural::Media:
    {
      m.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);
      m.insert(NCL_ENTITY_REFER, NCL_ENTITY_REFER);
      m.insert(NCL_ENTITY_INSTANCE, NCL_ENTITY_INSTANCE);
      m.insert(NCL_ENTITY_TYPE, NCL_ENTITY_TYPE);
      m.insert(NCL_ENTITY_SRC, PLG_ENTITY_SRC);
      m.insert(NCL_ENTITY_DESCRIPTOR, NCL_ENTITY_DESCRIPTOR);

      break;
    }

    case Structural::Port:
    {
      m.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);
      m.insert(NCL_ENTITY_COMPONENT, PLG_ENTITY_COMPONENT_ID);
      m.insert(NCL_ENTITY_INTERFACE, PLG_ENTITY_INTERFACE_ID);

      break;
    }

    case Structural::Property:
    {
      m.insert(NCL_ENTITY_NAME, PLG_ENTITY_ID);
      m.insert(NCL_ENTITY_VALUE, NCL_ENTITY_VALUE);
      m.insert(NCL_ENTITY_EXTERNABLE, NCL_ENTITY_EXTERNABLE);
      break;
    }

    case Structural::Area:
    {
      m.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);

      m.insert(NCL_ENTITY_COORDS, NCL_ENTITY_COORDS);
      m.insert(NCL_ENTITY_BEGIN, NCL_ENTITY_BEGIN);
      m.insert(NCL_ENTITY_END, NCL_ENTITY_END);
      m.insert(NCL_ENTITY_BEGINTEXT, NCL_ENTITY_BEGINTEXT);
      m.insert(NCL_ENTITY_ENDTEXT, NCL_ENTITY_ENDTEXT);
      m.insert(NCL_ENTITY_BEGINPOSITION, NCL_ENTITY_BEGINPOSITION);
      m.insert(NCL_ENTITY_ENDPOSITION, NCL_ENTITY_ENDPOSITION);
      m.insert(NCL_ENTITY_FIRST, NCL_ENTITY_FIRST);
      m.insert(NCL_ENTITY_LAST, NCL_ENTITY_LAST);
      m.insert(NCL_ENTITY_LABEL, NCL_ENTITY_LABEL);
      m.insert(NCL_ENTITY_CLIP, NCL_ENTITY_CLIP);

      break;
    }

    case Structural::SwitchPort:
    {
      m.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);

      break;
    }

    case Structural::Link:
    {
      m.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);
      m.insert(NCL_ENTITY_XCONNECTOR, PLG_ENTITY_XCONNECTOR_ID);

      break;
    }

    case Structural::Bind:
    {
      m.insert(NCL_ENTITY_ROLE, PLG_ENTITY_ID);
      m.insert(NCL_ENTITY_XCONNECTOR, PLG_ENTITY_XCONNECTOR_ID);
      m.insert(NCL_ENTITY_COMPONENT, PLG_ENTITY_COMPONENT_ID);
      m.insert(NCL_ENTITY_INTERFACE, PLG_ENTITY_INTERFACE_ID);
      m.insert(NCL_ENTITY_DESCRIPTOR, NCL_ENTITY_DESCRIPTOR);

      break;
    }

    case Structural::Mapping:
    {
      m.insert(NCL_ENTITY_COMPONENT, PLG_ENTITY_COMPONENT_ID);
      m.insert(NCL_ENTITY_INTERFACE, PLG_ENTITY_INTERFACE_ID);

      break;
    }

    default:
      break;

  }

  return m;
}

bool StructuralUtil::isConditionRole(StructuralRole role)
{
  return (role == Structural::onBegin ||
          role == Structural::onBeginAttribution ||
          role == Structural::onEnd ||
          role == Structural::onEndAttribution ||
          role == Structural::onPause ||
          role == Structural::onPauseAttribution ||
          role == Structural::onResume ||
          role == Structural::onResumeAttribution ||
          role == Structural::onSelection);
}

bool StructuralUtil::isActionRole(StructuralRole role)
{
  return (role != Structural::NoRole && !isConditionRole(role));
}

bool StructuralUtil::isConditionRole(QString role)
{
  return isConditionRole(roleTypeFromRoleStr[role]);
}

bool StructuralUtil::isActionRole(QString role)
{
  return isActionRole(roleTypeFromRoleStr[role]);
}

QMap<QString,QString> StructuralUtil::createViewTranslationMap(StructuralType type)
{
  QMap<QString, QString> m;

  switch(type)
  {
    case Structural::Body:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_ID);

      break;
    }

    case Structural::Context:
    case Structural::Switch:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_ID);
      m.insert(NCL_ENTITY_REFER,NCL_ENTITY_REFER);

      break;
    }

    case Structural::Media:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_ID);
      m.insert(NCL_ENTITY_REFER,NCL_ENTITY_REFER);
      m.insert(NCL_ENTITY_TYPE,NCL_ENTITY_TYPE);
      m.insert(PLG_ENTITY_SRC, NCL_ENTITY_SRC);
      m.insert(NCL_ENTITY_INSTANCE, NCL_ENTITY_INSTANCE);
      m.insert(NCL_ENTITY_DESCRIPTOR, NCL_ENTITY_DESCRIPTOR);

      break;
    }

    case Structural::Port:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_ID);
      m.insert(PLG_ENTITY_COMPONENT_ID, NCL_ENTITY_COMPONENT);
      m.insert(PLG_ENTITY_INTERFACE_ID, NCL_ENTITY_INTERFACE);

      break;
    }

    case Structural::Property:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_NAME);
      m.insert(NCL_ENTITY_VALUE, NCL_ENTITY_VALUE);
      m.insert(NCL_ENTITY_EXTERNABLE, NCL_ENTITY_EXTERNABLE);
      break;
    }

    case Structural::Area:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_ID);

      m.insert(NCL_ENTITY_COORDS, NCL_ENTITY_COORDS);
      m.insert(NCL_ENTITY_BEGIN, NCL_ENTITY_BEGIN);
      m.insert(NCL_ENTITY_END, NCL_ENTITY_END);
      m.insert(NCL_ENTITY_BEGINTEXT, NCL_ENTITY_BEGINTEXT);
      m.insert(NCL_ENTITY_ENDTEXT, NCL_ENTITY_ENDTEXT);
      m.insert(NCL_ENTITY_BEGINPOSITION, NCL_ENTITY_BEGINPOSITION);
      m.insert(NCL_ENTITY_ENDPOSITION, NCL_ENTITY_ENDPOSITION);
      m.insert(NCL_ENTITY_FIRST, NCL_ENTITY_FIRST);
      m.insert(NCL_ENTITY_LAST, NCL_ENTITY_LAST);
      m.insert(NCL_ENTITY_LABEL, NCL_ENTITY_LABEL);
      m.insert(NCL_ENTITY_CLIP, NCL_ENTITY_CLIP);

      break;
    }

    case Structural::SwitchPort:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_ID);

      break;
    }

    case Structural::Link:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_ID);
      m.insert(PLG_ENTITY_XCONNECTOR_ID, NCL_ENTITY_XCONNECTOR);

      break;
    }

    case Structural::Bind:
    {
      m.insert(PLG_ENTITY_ID, NCL_ENTITY_ROLE);
      m.insert(PLG_ENTITY_XCONNECTOR_ID, NCL_ENTITY_XCONNECTOR);
      m.insert(PLG_ENTITY_COMPONENT_ID, NCL_ENTITY_COMPONENT);
      m.insert(PLG_ENTITY_INTERFACE_ID, NCL_ENTITY_INTERFACE);
      m.insert(NCL_ENTITY_DESCRIPTOR, NCL_ENTITY_DESCRIPTOR);

      break;
    }

    case Structural::Mapping:
    {
      m.insert(PLG_ENTITY_COMPONENT_ID, NCL_ENTITY_COMPONENT);
      m.insert(PLG_ENTITY_INTERFACE_ID, NCL_ENTITY_INTERFACE);

      break;
    }

    default:
      // do nothing
      break;
  }

  return m;
}


QString StructuralUtil::CreateUid()
{
  return QUuid::createUuid().toString();
}

QString StructuralUtil::iconFromEntityType(Structural::StructuralType type)
{
  QString path;
  if(iconFromTypeMap.count(type))
    path = iconFromTypeMap[type];
  else
    path = ":/icon/media";

  return path;
}

QString StructuralUtil::iconFromMediaType(Structural::StructuralMedia type)
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

Structural::StructuralMedia StructuralUtil::getnstTypeFromExtension(const QString &ext)
{
  if(typeFromExtMap.count(ext))
    return typeFromExtMap[ext];

  return Structural::NoContent;
}

Structural::StructuralType StructuralUtil::getnstTypeFromStr(const QString &strType)
{
  if(typeFromStr.count(strType))
    return typeFromStr[strType];

  return Structural::NoType;
}



QString StructuralUtil::getStrFromNstType(Structural::StructuralType type)
{
  if(strFromType.count(type))
    return strFromType[type];

  else return "";
}
QString StructuralUtil::getPrefixIdFromType(Structural::StructuralType type)
{
  if(prefixIdFromType.count(type))
    return prefixIdFromType[type];

  return "unknown";
}

Structural::StructuralRole StructuralUtil::getStructuralRoleFromStr(const QString &role)
{
  if(roleTypeFromRoleStr.count(role))
    return roleTypeFromRoleStr[role];

  return Structural::NoRole;
}

QString StructuralUtil::getStrFromStructuralRole(Structural::StructuralRole type)
{
  if(strFromRoleType.count(type))
    return strFromRoleType[type];

  return QString("NoRole");
}

