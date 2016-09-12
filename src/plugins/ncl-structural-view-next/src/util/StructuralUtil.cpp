#include "StructuralUtil.h"

QString StructuralUtil::createUid()
{
  return QUuid::createUuid().toString();
}

QMap<QString,QString> StructuralUtil::createSettings()
{
  return createSettings(true, true);
}

QMap<QString,QString> StructuralUtil::createSettings(const bool undo, const bool notify)
{
  return createSettings((undo ? PLG_VALUE_TRUE : PLG_VALUE_FALSE), (notify ? PLG_VALUE_TRUE : PLG_VALUE_FALSE), createUid());
}

QMap<QString,QString> StructuralUtil::createSettings(const QString &undo, const QString &notify, const QString &code)
{
  QMap<QString,QString> settings;

  if (undo.isEmpty())
    settings.insert(PLG_SETTING_UNDO, PLG_VALUE_TRUE);
  else
    settings.insert(PLG_SETTING_UNDO, undo);

  if (notify.isEmpty())
    settings.insert(PLG_SETTING_NOTIFY,PLG_VALUE_TRUE);
  else
    settings.insert(PLG_SETTING_NOTIFY, notify);

  if (code.isEmpty())
    settings.insert(PLG_SETTING_CODE, createUid());
  else
    settings.insert(PLG_SETTING_CODE, code);

  return settings;
}

QMap<QString,QString> StructuralUtil::createCoreTranslations(const StructuralType type)
{
  QMap<QString, QString> translations;

  switch (type)
  {
    case Structural::Body:
    {
      translations[NCL_ENTITY_ID] = PLG_PROPERTY_ID;

      break;
    }

    case Structural::Context:
    case Structural::Switch:
    {
      translations[NCL_ENTITY_ID] = PLG_PROPERTY_ID;
      translations[NCL_ENTITY_REFER] = NCL_ENTITY_REFER;

      break;
    }

    case Structural::Media:
    {
      translations[NCL_ENTITY_ID] = PLG_PROPERTY_ID;
      translations[NCL_ENTITY_REFER] = NCL_ENTITY_REFER;
      translations[NCL_ENTITY_INSTANCE] = NCL_ENTITY_INSTANCE;
      translations[NCL_ENTITY_TYPE] = NCL_ENTITY_TYPE;
      translations[NCL_ENTITY_SRC] = PLG_ENTITY_SRC;
      translations[NCL_ENTITY_DESCRIPTOR] = NCL_ENTITY_DESCRIPTOR;

      break;
    }

    case Structural::Port:
    {
      translations[NCL_ENTITY_ID] = PLG_PROPERTY_ID;
      translations[NCL_ENTITY_COMPONENT] = PLG_ENTITY_COMPONENT_ID;
      translations[NCL_ENTITY_INTERFACE] = PLG_ENTITY_INTERFACE_ID;

      break;
    }

    case Structural::Property:
    {
      translations[NCL_ENTITY_NAME] = PLG_PROPERTY_ID;
      translations[NCL_ENTITY_VALUE] = NCL_ENTITY_VALUE;
      translations[NCL_ENTITY_EXTERNABLE] = NCL_ENTITY_EXTERNABLE;
      break;
    }

    case Structural::Area:
    {
      translations[NCL_ENTITY_ID] = PLG_PROPERTY_ID;

      translations[NCL_ENTITY_COORDS] = NCL_ENTITY_COORDS;
      translations[NCL_ENTITY_BEGIN] = NCL_ENTITY_BEGIN;
      translations[NCL_ENTITY_END] = NCL_ENTITY_END;
      translations[NCL_ENTITY_BEGINTEXT] = NCL_ENTITY_BEGINTEXT;
      translations[NCL_ENTITY_ENDTEXT] = NCL_ENTITY_ENDTEXT;
      translations[NCL_ENTITY_BEGINPOSITION] = NCL_ENTITY_BEGINPOSITION;
      translations[NCL_ENTITY_ENDPOSITION] = NCL_ENTITY_ENDPOSITION;
      translations[NCL_ENTITY_FIRST] = NCL_ENTITY_FIRST;
      translations[NCL_ENTITY_LAST] = NCL_ENTITY_LAST;
      translations[NCL_ENTITY_LABEL] = NCL_ENTITY_LABEL;
      translations[NCL_ENTITY_CLIP] = NCL_ENTITY_CLIP;

      break;
    }

    case Structural::SwitchPort:
    {
      translations[NCL_ENTITY_ID] = PLG_PROPERTY_ID;

      break;
    }

    case Structural::Link:
    {
      translations[NCL_ENTITY_ID] = PLG_PROPERTY_ID;
      translations[NCL_ENTITY_XCONNECTOR] = PLG_ENTITY_XCONNECTOR_ID;

      break;
    }

    case Structural::Bind:
    {
      translations[NCL_ENTITY_ROLE] = PLG_PROPERTY_ID;
      translations[NCL_ENTITY_XCONNECTOR] = PLG_ENTITY_XCONNECTOR_ID;
      translations[NCL_ENTITY_COMPONENT] = PLG_ENTITY_COMPONENT_ID;
      translations[NCL_ENTITY_INTERFACE] = PLG_ENTITY_INTERFACE_ID;
      translations[NCL_ENTITY_DESCRIPTOR] = NCL_ENTITY_DESCRIPTOR;

      break;
    }

    case Structural::Mapping:
    {
      translations[NCL_ENTITY_COMPONENT] = PLG_ENTITY_COMPONENT_ID;
      translations[NCL_ENTITY_INTERFACE] = PLG_ENTITY_INTERFACE_ID;

      break;
    }

    default:
      break;

  }

  return translations;
}

QMap<QString,QString> StructuralUtil::createPluginTranslations(const StructuralType type)
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

QString StructuralUtil::translateCategoryToString(const StructualCategory category)
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

std::map <QString, Structural::StructuralType> StructuralUtil::_mapStringToType =
    invert<QString, Structural::StructuralType>(StructuralUtil::_mapTypeToString);

QString StructuralUtil::translateTypeToString(const StructuralType type)
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

std::map <Structural::StructuralRole, QString> StructuralUtil::_mapRoleToString =
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

std::map <QString, Structural::StructuralRole> StructuralUtil::_mapStringToRole =
    invert<QString, Structural::StructuralRole> (StructuralUtil::_mapRoleToString);

QString StructuralUtil::translateRoleToString(const StructuralRole role)
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

std::map <Structural::StructuralMimeType, QString> StructuralUtil::_mapMimeTypeToString =
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

std::map <QString, Structural::StructuralMimeType> StructuralUtil::_mapStringToMimeType =
    invert<QString, Structural::StructuralMimeType> (StructuralUtil::_mapMimeTypeToString);

QString StructuralUtil::translateMimeTypeToString(const StructuralMimeType mimetype)
{
  if(_mapMimeTypeToString.count(mimetype))
    return _mapMimeTypeToString[mimetype];
  else
    return "";
}

StructuralMimeType StructuralUtil::translateStringToMimeType(const QString &mimetype)
{
  if(_mapStringToMimeType.count(mimetype))
    return _mapStringToMimeType[mimetype];
  else
    return Structural::NoMimeType;
}

std::map <Structural::StructuralType, QString> StructuralUtil::_entitiesIcon =
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

QString StructuralUtil::getEntityIcon(const StructuralType type)
{
  if(_entitiesIcon.count(type))
    return _entitiesIcon[type];
  else
    return "";
}

std::map <Structural::StructuralMimeType, QString> StructuralUtil::_mimetypesIcon =
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

QString StructuralUtil::getMimeTypeIcon(const StructuralMimeType type)
{
  if(_mimetypesIcon.count(type))
    return _mimetypesIcon[type];
  else
    return ":/images/icon/media";
}

std::map <Structural::StructuralType, QString> StructuralUtil::_entitiesPrefix =
    StructuralMap<Structural::StructuralType, QString >
(Structural::Media, "m")
(Structural::Body, "b")
(Structural::Context, "ctx")
(Structural::Switch, "swt")
(Structural::Port, "p")
(Structural::SwitchPort, "swtp")
(Structural::Area, "a")
(Structural::Property, "p")
(Structural::Link, "")
(Structural::Bind, "")
(Structural::Reference, "")
(Structural::Mapping, "")

(Structural::NoType, "e");

QString StructuralUtil::getEntityPrefix(const StructuralType type)
{
  if(_entitiesPrefix.count(type))
    return _entitiesPrefix[type];
  else
    return "e";
}

std::map <QString, Structural::StructuralMimeType> StructuralUtil::_mimetypesExtension =
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

bool StructuralUtil::isConditionRole(const StructuralRole role)
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

bool StructuralUtil::isConditionRole(const QString &role)
{
  return isConditionRole(_mapStringToRole[role]);
}

bool StructuralUtil::isActionRole(const StructuralRole role)
{
  return (role != Structural::NoRole && !isConditionRole(role));
}

bool StructuralUtil::isActionRole(const QString &role)
{
  return isActionRole(_mapStringToRole[role]);
}

QString StructuralUtil::formatId(const QString &id)
{
  QString normalized = id.normalized(QString::NormalizationForm_KD);
  normalized.remove(QRegExp("[^a-zA-Z_-\.\\s]"));

  if(normalized.at(0).isDigit())
    normalized = "_" + normalized;

  return normalized;
}

bool StructuralUtil::validateKinship(const StructuralType entityType, const StructuralType parentType)
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
