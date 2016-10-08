#include "StructuralUtil.h"

#include <QUuid>

#include "StructuralEdge.h"

QString StructuralUtil::createUid()
{
  return QUuid::createUuid().toString();
}

QMap<QString,QString> StructuralUtil::createProperties(qreal top, qreal left)
{
  QMap<QString,QString> properties;

  properties[STR_PROPERTY_ENTITY_TOP] = QString::number(top);
  properties[STR_PROPERTY_ENTITY_LEFT] = QString::number(left);

  return properties;
}

QMap<QString,QString> StructuralUtil::createProperties(qreal top, qreal left, qreal width, qreal height)
{
  QMap<QString,QString> properties;

  properties[STR_PROPERTY_ENTITY_TOP] = QString::number(top);
  properties[STR_PROPERTY_ENTITY_LEFT] = QString::number(left);
  properties[STR_PROPERTY_ENTITY_WIDTH] = QString::number(width);
  properties[STR_PROPERTY_ENTITY_HEIGHT] = QString::number(height);

  return properties;
}

QMap<QString,QString> StructuralUtil::createSettings()
{
  return createSettings(true, true);
}

QMap<QString,QString> StructuralUtil::createSettings(bool undo, bool notify)
{
  return createSettings((undo ? STR_VALUE_TRUE : STR_VALUE_FALSE), (notify ? STR_VALUE_TRUE : STR_VALUE_FALSE), createUid());
}

QMap<QString,QString> StructuralUtil::createSettings(const QString &undo, const QString &notify, const QString &code)
{
  QMap<QString,QString> settings;

  if (undo.isEmpty())
    settings.insert(STR_SETTING_UNDO, STR_VALUE_TRUE);
  else
    settings.insert(STR_SETTING_UNDO, undo);

  if (notify.isEmpty())
    settings.insert(STR_SETTING_NOTIFY,STR_VALUE_TRUE);
  else
    settings.insert(STR_SETTING_NOTIFY, notify);

  if (code.isEmpty())
    settings.insert(STR_SETTING_CODE, createUid());
  else
    settings.insert(STR_SETTING_CODE, code);

  return settings;
}

QMap<QString,QString> StructuralUtil::createCoreTranslations(StructuralType type)
{
  QMap<QString, QString> translations;

  switch (type)
  {
    case Structural::Body:
    {
      translations[NCL_ATTRIBUTE_ID] = STR_PROPERTY_ENTITY_ID;

      break;
    }

    case Structural::Context:
    case Structural::Switch:
    {
      translations[NCL_ATTRIBUTE_ID] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTRIBUTE_REFER] = STR_PROPERTY_REFERENCE_REFER_ID;

      break;
    }

    case Structural::Media:
    {
      translations[NCL_ATTRIBUTE_ID] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTRIBUTE_REFER] = STR_PROPERTY_REFERENCE_REFER_ID;
      translations[NCL_ATTRIBUTE_INSTANCE] = STR_PROPERTY_CONTENT_INSTANCE;
      translations[NCL_ATTRIBUTE_TYPE] = STR_PROPERTY_CONTENT_TYPE;
      translations[NCL_ATTRIBUTE_SRC] = STR_PROPERTY_CONTENT_LOCATION;
      translations[NCL_ATTRIBUTE_DESCRIPTOR] = NCL_ATTRIBUTE_DESCRIPTOR;

      break;
    }

    case Structural::Port:
    {
      translations[NCL_ATTRIBUTE_ID] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTRIBUTE_COMPONENT] = STR_PROPERTY_REFERENCE_COMPONENT_ID;
      translations[NCL_ATTRIBUTE_INTERFACE] = STR_PROPERTY_REFERENCE_INTERFACE_ID;

      break;
    }

    case Structural::Property:
    {
      translations[NCL_ATTRIBUTE_NAME] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTRIBUTE_VALUE] = NCL_ATTRIBUTE_VALUE;
      translations[NCL_ATTRIBUTE_EXTERNABLE] = NCL_ATTRIBUTE_EXTERNABLE;
      break;
    }

    case Structural::Area:
    {
      translations[NCL_ATTRIBUTE_ID] = STR_PROPERTY_ENTITY_ID;

      translations[NCL_ATTRIBUTE_COORDS] = NCL_ATTRIBUTE_COORDS;
      translations[NCL_ATTRIBUTE_BEGIN] = NCL_ATTRIBUTE_BEGIN;
      translations[NCL_ATTRIBUTE_END] = NCL_ATTRIBUTE_END;
      translations[NCL_ATTRIBUTE_BEGINTEXT] = NCL_ATTRIBUTE_BEGINTEXT;
      translations[NCL_ATTRIBUTE_ENDTEXT] = NCL_ATTRIBUTE_ENDTEXT;
      translations[NCL_ATTRIBUTE_BEGINPOSITION] = NCL_ATTRIBUTE_BEGINPOSITION;
      translations[NCL_ATTRIBUTE_ENDPOSITION] = NCL_ATTRIBUTE_ENDPOSITION;
      translations[NCL_ATTRIBUTE_FIRST] = NCL_ATTRIBUTE_FIRST;
      translations[NCL_ATTRIBUTE_LAST] = NCL_ATTRIBUTE_LAST;
      translations[NCL_ATTRIBUTE_LABEL] = NCL_ATTRIBUTE_LABEL;
      translations[NCL_ATTRIBUTE_CLIP] = NCL_ATTRIBUTE_CLIP;

      break;
    }

    case Structural::SwitchPort:
    {
      translations[NCL_ATTRIBUTE_ID] = STR_PROPERTY_ENTITY_ID;

      break;
    }

    case Structural::Link:
    {
      translations[NCL_ATTRIBUTE_ID] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTRIBUTE_XCONNECTOR] = STR_PROPERTY_REFERENCE_XCONNECTOR_ID;

      break;
    }

    case Structural::Bind:
    {
      translations[NCL_ATTRIBUTE_ROLE] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTRIBUTE_XCONNECTOR] = STR_PROPERTY_REFERENCE_XCONNECTOR_ID;
      translations[NCL_ATTRIBUTE_COMPONENT] = STR_PROPERTY_REFERENCE_COMPONENT_ID;
      translations[NCL_ATTRIBUTE_INTERFACE] = STR_PROPERTY_REFERENCE_INTERFACE_ID;
      translations[NCL_ATTRIBUTE_DESCRIPTOR] = NCL_ATTRIBUTE_DESCRIPTOR;

      break;
    }

    case Structural::Mapping:
    {
      translations[NCL_ATTRIBUTE_COMPONENT] = STR_PROPERTY_REFERENCE_COMPONENT_ID;
      translations[NCL_ATTRIBUTE_INTERFACE] = STR_PROPERTY_REFERENCE_INTERFACE_ID;

      break;
    }

    default:
      break;

  }

  return translations;
}

QMap<QString,QString> StructuralUtil::createPluginTranslations(StructuralType type)
{
  QMap<QString, QString> translations;

  QMap<QString, QString> inverted = createCoreTranslations(type);
  foreach (QString key, inverted.keys())
    translations[inverted.value(key)] = key;

  return translations;
}

std::map<Structural::StructuralCategory, QString> StructuralUtil::_mapCategoryToString =
    StructuralMap<Structural::StructuralCategory, QString>
(Structural::Node, "node")
(Structural::Edge, "edge")
(Structural::Interface, "interface")

(Structural::NoCategory, "");

std::map<QString, Structural::StructuralCategory> StructuralUtil::_mapStringToCategory =
    invert<QString, Structural::StructuralCategory> (StructuralUtil::_mapCategoryToString);

QString StructuralUtil::translateCategoryToString(StructualCategory category)
{
  if (_mapCategoryToString.count(category))
    return _mapCategoryToString[category];
  else
    return "";
}

StructualCategory StructuralUtil::translateStringToCategory(const QString &category)
{
  if (_mapStringToCategory.count(category))
    return _mapStringToCategory[category];
  else
    return Structural::NoCategory;
}

std::map<Structural::StructuralType, QString> StructuralUtil::_mapTypeToString =
    StructuralMap<Structural::StructuralType, QString>
(Structural::Media, "media")
(Structural::Body, "body")
(Structural::Context, "context")
(Structural::Switch, "switch")
(Structural::Port, "port")
(Structural::SwitchPort, "switchPort")
(Structural::Area, "area")
(Structural::Property, "property")
(Structural::Link, "link")
(Structural::Bind, "bind")
(Structural::Reference, "reference")
(Structural::Mapping, "mapping")

(Structural::NoType, "");

std::map<QString, Structural::StructuralType> StructuralUtil::_mapStringToType =
    invert<QString, Structural::StructuralType>(StructuralUtil::_mapTypeToString);

QString StructuralUtil::translateTypeToString(StructuralType type)
{
  if (_mapTypeToString.count(type))
    return _mapTypeToString[type];
  else
    return "";
}

StructuralType StructuralUtil::translateStringToType(const QString &type)
{
  if (_mapStringToType.count(type))
    return _mapStringToType[type];
  else
    return Structural::NoType;
}

std::map<Structural::StructuralRole, QString> StructuralUtil::_mapRoleToString =
    StructuralMap<Structural::StructuralRole, QString>
(Structural::onBegin, "onBegin")
(Structural::onEnd, "onEnd")
(Structural::onSelection, "onSelection")
(Structural::onResume, "onResume")
(Structural::onPause, "onPause")
(Structural::onBeginAttribution, "onBeginAttribution")
(Structural::onEndAttribution, "onEndAttribution")
(Structural::onPauseAttribution, "onPauseAttribution")
(Structural::onResumeAttribution, "onResumeAttribution")

(Structural::Start, "start")
(Structural::Stop, "stop")
(Structural::Resume, "resume")
(Structural::Pause, "pause")
(Structural::Set, "set")

(Structural::NoRole, "");

std::map<QString, Structural::StructuralRole> StructuralUtil::_mapStringToRole =
    invert<QString, Structural::StructuralRole> (StructuralUtil::_mapRoleToString);

QString StructuralUtil::translateRoleToString(StructuralRole role)
{
  if(_mapRoleToString.count(role))
    return _mapRoleToString[role];
  else
    return "";
}

StructuralRole StructuralUtil::translateStringToRole(const QString &role)
{
  if(_mapStringToRole.count(role))
    return _mapStringToRole[role];
  else
    return Structural::NoRole;
}

std::map<Structural::StructuralMimeType, QString> StructuralUtil::_mapMimeTypeToString =
    StructuralMap<Structural::StructuralMimeType, QString >
(Structural::Text, "text")
(Structural::Image, "image")
(Structural::Audio, "audio")
(Structural::Video, "video")
(Structural::HTML, "html")
(Structural::NCL, "ncl")
(Structural::Settings, "settings")
(Structural::NCLua, "nclua")

(Structural::NoMimeType, "");

std::map<QString, Structural::StructuralMimeType> StructuralUtil::_mapStringToMimeType =
    invert<QString, Structural::StructuralMimeType> (StructuralUtil::_mapMimeTypeToString);

QString StructuralUtil::translateMimeTypeToString(StructuralMimeType mimetype)
{
  if(_mapMimeTypeToString.count(mimetype))
    return _mapMimeTypeToString[mimetype];
  else
    return "Media";
}

StructuralMimeType StructuralUtil::translateStringToMimeType(const QString &mimetype)
{
  if(_mapStringToMimeType.count(mimetype))
    return _mapStringToMimeType[mimetype];
  else
    return Structural::NoMimeType;
}

std::map<Structural::StructuralType, QString> StructuralUtil::_entitiesIcon =
    StructuralMap<Structural::StructuralType, QString >
(Structural::Media, ":/images/icon/media")
(Structural::Body, ":/images/icon/body")
(Structural::Context, ":/images/icon/context")
(Structural::Switch, ":/images/icon/switch")
(Structural::Port, ":/images/icon/port")
(Structural::SwitchPort, ":/images/icon/switchport")
(Structural::Area, ":/images/icon/area")
(Structural::Property, ":/images/icon/property")
(Structural::Link, "")
(Structural::Bind, "")
(Structural::Reference, "")
(Structural::Mapping, "")

(Structural::NoType, "");

QString StructuralUtil::getIcon(StructuralType type)
{
  if(_entitiesIcon.count(type))
    return _entitiesIcon[type];
  else
    return "";
}

std::map<Structural::StructuralType, QString> StructuralUtil::_entitiesColor =
    StructuralMap<Structural::StructuralType, QString >
(Structural::Media, "")
(Structural::Body, "#EEEEEE")
(Structural::Context, "#F4E4CC")
(Structural::Switch, "#C6E2FF")
(Structural::Port, "")
(Structural::SwitchPort, "")
(Structural::Area, "")
(Structural::Property, "")
(Structural::Link, "#000000")
(Structural::Bind, "#000000")
(Structural::Reference, "#000000")
(Structural::Mapping, "#5C0099")

(Structural::NoType, "");

QString StructuralUtil::getColor(StructuralType type)
{
  if(_entitiesColor.count(type))
    return _entitiesColor[type];
  else
    return "";
}

std::map<Structural::StructuralType, QString> StructuralUtil::_entitiesPrefix =
    StructuralMap<Structural::StructuralType, QString >
(Structural::Media, "m")
(Structural::Body, "b")
(Structural::Context, "ctx")
(Structural::Switch, "swt")
(Structural::Port, "p")
(Structural::SwitchPort, "swtp")
(Structural::Area, "a")
(Structural::Property, "p")
(Structural::Link, "l")
(Structural::Bind, "")
(Structural::Reference, "")
(Structural::Mapping, "")

(Structural::NoType, "e");

QString StructuralUtil::getPrefix(StructuralType type)
{
  if(_entitiesPrefix.count(type))
    return _entitiesPrefix[type];
  else
    return "e";
}

QString StructuralUtil::getTooltip(StructuralType type, const QString &title, const QString &info, const QString &warning, const QString &error, const QString &extra)
{
  QString tooltip;

  // Adding type
  tooltip += translateTypeToString(type);
  tooltip += " ";

  // Adding title
  if (!title.isEmpty())
    tooltip += "("+title+")";
  else
    tooltip += "(?)";

  tooltip += " ";

  // Adding extra
  if (type == Structural::Link){
    if (!extra.isEmpty())
      tooltip += ": "+extra;
    else
      tooltip += ": Unknow";

    tooltip += " ";
  }

  // Adding messages
  if (!error.isEmpty())
    tooltip += "- Error: "+error;
  else if (!warning.isEmpty())
    tooltip += "- Warning: "+warning;
  else if (!info.isEmpty())
    tooltip += "- Info: "+info;

  // Formating
  tooltip[0] = tooltip[0].toUpper();

  return tooltip;
}

std::map<Structural::StructuralMimeType, QString> StructuralUtil::_mimetypesIcon =
    StructuralMap<Structural::StructuralMimeType, QString >
(Structural::Text, ":/icon/text")
(Structural::Image, ":/icon/image")
(Structural::Audio, ":/icon/audio")
(Structural::Video, ":/icon/video")
(Structural::HTML, ":/icon/html")
(Structural::NCL, ":/icon/ncl")
(Structural::Settings, ":/icon/settings")
(Structural::NCLua, ":/icon/script")

(Structural::NoMimeType, ":/images/icon/media");

QString StructuralUtil::getMimeTypeIcon(StructuralMimeType type)
{
  if(_mimetypesIcon.count(type))
    return _mimetypesIcon[type];
  else
    return ":/images/icon/media";
}

QString StructuralUtil::getMimeTypeTooltip(StructuralMimeType mimetype, const QString &title, const QString &info, const QString &warning, const QString &error, const QString &extra)
{
  QString tooltip;

  // Adding type
  tooltip += translateMimeTypeToString(mimetype);
  tooltip += " ";

  // Adding title
  if (!title.isEmpty())
    tooltip += "("+title+")";
  else
    tooltip += "(?)";

  tooltip += " ";

  // Adding extra
  // nothing...

  // Adding messages
  if (!error.isEmpty())
    tooltip += "- Error: "+error;
  else if (!warning.isEmpty())
    tooltip += "- Warning: "+warning;
  else if (!info.isEmpty())
    tooltip += "- Info: "+info;

  // Formating
  tooltip[0] = tooltip[0].toUpper();

  return tooltip;
}

std::map<QString, Structural::StructuralMimeType> StructuralUtil::_mimetypesExtension =
    StructuralMap <QString, Structural::StructuralMimeType>
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

Structural::StructuralMimeType StructuralUtil::getMimeTypeByExtension(const QString &extension)
{
  if(_mimetypesExtension.count(extension))
    return _mimetypesExtension[extension];
  else
    return Structural::NoMimeType;
}

std::map<Structural::StructuralRole, QString> StructuralUtil::_rolesIcon =
    StructuralMap<Structural::StructuralRole, QString>
(Structural::onBegin, ":/icon/onbegin")
(Structural::onEnd, ":/icon/onend")
(Structural::onSelection, ":/icon/onselection")
(Structural::onResume, ":/icon/onresume")
(Structural::onPause, ":/icon/onpause")
(Structural::onBeginAttribution, "")
(Structural::onEndAttribution, "")
(Structural::onPauseAttribution, "")
(Structural::onResumeAttribution, "")

(Structural::Start, ":/icon/start")
(Structural::Stop, ":/icon/stop")
(Structural::Resume, ":/icon/resume")
(Structural::Pause, ":/icon/pause")
(Structural::Set, ":/icon/set")

(Structural::NoRole, "");

QString StructuralUtil::getIcon(StructuralRole role)
{
  if(_rolesIcon.count(role))
    return _rolesIcon[role];
  else
    return "";
}

bool StructuralUtil::isCondition(StructuralRole role)
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

bool StructuralUtil::isCondition(const QString &role)
{
  return isCondition(_mapStringToRole[role]);
}

bool StructuralUtil::isAction(StructuralRole role)
{
  return (role != Structural::NoRole && !isCondition(role));
}

bool StructuralUtil::isAction(const QString &role)
{
  return isAction(_mapStringToRole[role]);
}

void StructuralUtil::adjustEdges(StructuralEntity* entity)
{
  StructuralEntity* parent = entity->getStructuralParent();

  if (parent != NULL)
  {
    QVector<StructuralEntity*> relatives;
    relatives = parent->getStructuralEntities();

    if (parent->getStructuralParent() != NULL){
      relatives += parent->getStructuralParent()->getStructuralEntities();
    }

    foreach (StructuralEntity* relative, relatives) {
      if (relative->getStructuralCategory() == Structural::Edge){
        StructuralEdge *edge = (StructuralEdge*) relative;

        if (edge->getTail() == entity || edge->getHead() == entity)
          edge->adjust(true);
      }
    }
  }
}

QString StructuralUtil::formatId(const QString &id)
{
  QString normalized = id.normalized(QString::NormalizationForm_KD);
  normalized.remove(QRegExp("[^a-zA-Z_-\.\\s]"));

  if(normalized.at(0).isDigit())
    normalized = "_" + normalized;

  return normalized;
}

bool StructuralUtil::validateKinship(StructuralType entityType, StructuralType parentType)
{
  switch (entityType) {
    case Structural::Body:
    {
      return false;
      break;
    }

    case Structural::Switch:
    case Structural::Context:
    case Structural::Media:
    case Structural::Port:
    case Structural::Property:
    case Structural::Link:
    case Structural::Bind:
    case Structural::Reference:
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

    case Structural::SwitchPort:
    case Structural::Mapping:
    {
      switch (parentType) {
        case Structural::Switch:
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

    case Structural::Area:
    {
      switch (parentType) {
        case Structural::Media:
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
