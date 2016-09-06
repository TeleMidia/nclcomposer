#include "StructuralUtil.h"

#include "StructuralContent.h"

QString StructuralUtil::createUid()
{
  return QUuid::createUuid().toString();
}

QMap<QString,QString> StructuralUtil::createSettings(bool undo, bool notify)
{
   return createSettings(undo, notify, createUid());
}

QMap<QString,QString> StructuralUtil::createSettings(bool undo, bool notify, const QString &code)
{
  return createSettings((undo ? STR_VALUE_TRUE : STR_VALUE_FALSE), (notify ? STR_VALUE_TRUE : STR_VALUE_FALSE), code);
}

QMap<QString,QString> StructuralUtil::createSettings(const QString &undo, const QString &notify, const QString &code)
{
  QMap<QString,QString> settings;

  if (undo.isEmpty())
    settings.insert(PLG_SETTING_UNDO, STR_VALUE_TRUE);
  else
    settings.insert(PLG_SETTING_UNDO, undo);

  if (notify.isEmpty())
    settings.insert(PLG_SETTING_NOTIFY,STR_VALUE_TRUE);
  else
    settings.insert(PLG_SETTING_NOTIFY, notify);

  if (code.isEmpty())
    settings.insert(PLG_SETTING_CODE, createUid());
  else
    settings.insert(PLG_SETTING_CODE, code);

  return settings;
}

QMap<QString,QString> StructuralUtil::createComposerTranslations(StructuralType type)
{
  QMap<QString, QString> translations;

  switch (type)
  {
    case Structural::Body:
    {
      translations.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);

      break;
    }

    case Structural::Context:
    case Structural::Switch:
    {
      translations.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);
      translations.insert(NCL_ENTITY_REFER, NCL_ENTITY_REFER);

      break;
    }

    case Structural::Media:
    {
      translations.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);
      translations.insert(NCL_ENTITY_REFER, NCL_ENTITY_REFER);
      translations.insert(NCL_ENTITY_INSTANCE, NCL_ENTITY_INSTANCE);
      translations.insert(NCL_ENTITY_TYPE, NCL_ENTITY_TYPE);
      translations.insert(NCL_ENTITY_SRC, PLG_ENTITY_SRC);
      translations.insert(NCL_ENTITY_DESCRIPTOR, NCL_ENTITY_DESCRIPTOR);

      break;
    }

    case Structural::Port:
    {
      translations.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);
      translations.insert(NCL_ENTITY_COMPONENT, PLG_ENTITY_COMPONENT_ID);
      translations.insert(NCL_ENTITY_INTERFACE, PLG_ENTITY_INTERFACE_ID);

      break;
    }

    case Structural::Property:
    {
      translations.insert(NCL_ENTITY_NAME, PLG_ENTITY_ID);
      translations.insert(NCL_ENTITY_VALUE, NCL_ENTITY_VALUE);
      translations.insert(NCL_ENTITY_EXTERNABLE, NCL_ENTITY_EXTERNABLE);
      break;
    }

    case Structural::Area:
    {
      translations.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);

      translations.insert(NCL_ENTITY_COORDS, NCL_ENTITY_COORDS);
      translations.insert(NCL_ENTITY_BEGIN, NCL_ENTITY_BEGIN);
      translations.insert(NCL_ENTITY_END, NCL_ENTITY_END);
      translations.insert(NCL_ENTITY_BEGINTEXT, NCL_ENTITY_BEGINTEXT);
      translations.insert(NCL_ENTITY_ENDTEXT, NCL_ENTITY_ENDTEXT);
      translations.insert(NCL_ENTITY_BEGINPOSITION, NCL_ENTITY_BEGINPOSITION);
      translations.insert(NCL_ENTITY_ENDPOSITION, NCL_ENTITY_ENDPOSITION);
      translations.insert(NCL_ENTITY_FIRST, NCL_ENTITY_FIRST);
      translations.insert(NCL_ENTITY_LAST, NCL_ENTITY_LAST);
      translations.insert(NCL_ENTITY_LABEL, NCL_ENTITY_LABEL);
      translations.insert(NCL_ENTITY_CLIP, NCL_ENTITY_CLIP);

      break;
    }

    case Structural::SwitchPort:
    {
      translations.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);

      break;
    }

    case Structural::Link:
    {
      translations.insert(NCL_ENTITY_ID, PLG_ENTITY_ID);
      translations.insert(NCL_ENTITY_XCONNECTOR, PLG_ENTITY_XCONNECTOR_ID);

      break;
    }

    case Structural::Bind:
    {
      translations.insert(NCL_ENTITY_ROLE, PLG_ENTITY_ID);
      translations.insert(NCL_ENTITY_XCONNECTOR, PLG_ENTITY_XCONNECTOR_ID);
      translations.insert(NCL_ENTITY_COMPONENT, PLG_ENTITY_COMPONENT_ID);
      translations.insert(NCL_ENTITY_INTERFACE, PLG_ENTITY_INTERFACE_ID);
      translations.insert(NCL_ENTITY_DESCRIPTOR, NCL_ENTITY_DESCRIPTOR);

      break;
    }

    case Structural::Mapping:
    {
      translations.insert(NCL_ENTITY_COMPONENT, PLG_ENTITY_COMPONENT_ID);
      translations.insert(NCL_ENTITY_INTERFACE, PLG_ENTITY_INTERFACE_ID);

      break;
    }

    default:
      break;

  }

  return translations;
}

QMap<QString,QString> StructuralUtil::createStructuralTranslations(StructuralType type)
{
  QMap<QString, QString> translations;

  QMap<QString, QString> reverse = createComposerTranslations(type);
  foreach (QString key, reverse.keys())
    translations.insert(reverse.value(key), key);

  return translations;
}


std::map<Structural::StructuralCategory, QString> StructuralUtil::_mapCategoryToString =
  create_map<Structural::StructuralCategory, QString>
  (Structural::Node, "node")
  (Structural::Edge, "edge")
  (Structural::Interface, "interface")

  (Structural::NoCategory, "");

std::map<QString, Structural::StructuralCategory> StructuralUtil::_mapStringToCategory =
  invert_map<QString, Structural::StructuralCategory> (StructuralUtil::_mapCategoryToString);

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
  create_map<Structural::StructuralType, QString>
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
    invert_map<QString, Structural::StructuralType>(StructuralUtil::_mapTypeToString);

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


std::map <Structural::StructuralRole, QString> StructuralUtil::_mapRoleToString =
  create_map<Structural::StructuralRole, QString>
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
  invert_map<QString, Structural::StructuralRole> (StructuralUtil::_mapRoleToString);

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

std::map <Structural::StructuralType, QString> StructuralUtil::_mapTypeToIcon =
  create_map<Structural::StructuralType, QString >
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

QString StructuralUtil::translateTypeToIcon(Structural::StructuralType type)
{
  if(_mapTypeToIcon.count(type))
    return _mapTypeToIcon[type];
  else
    return "";
}

std::map <Structural::StructuralMedia, QString> StructuralUtil::_mapMediaToIcon =
  create_map<Structural::StructuralMedia, QString >
  (Structural::Text, ":/icon/text")
  (Structural::Image, ":/icon/image")
  (Structural::Audio, ":/icon/audio")
  (Structural::Video, ":/icon/video")
  (Structural::HTML, ":/icon/html")
  (Structural::NCL, ":/icon/ncl")
  (Structural::Settings, ":/icon/settings")
  (Structural::NCLua, ":/icon/script")
  (Structural::NoMedia, ":/images/icon/media");

QString StructuralUtil::translateMediaToIcon(Structural::StructuralMedia type)
{
  if(_mapMediaToIcon.count(type))
    return _mapMediaToIcon[type];
  else
    return ":/images/icon/media";
}

/* Initialize prefix id from type Map */
std::map <Structural::StructuralType, QString> StructuralUtil::_mapTypeToPrefix =
  create_map<Structural::StructuralType, QString >
  (Structural::Media, "m")
  (Structural::Body, ":b")
  (Structural::Context, "c")
  (Structural::Switch, "s")
  (Structural::Port, "p")
  (Structural::SwitchPort, "sp")
  (Structural::Area, "a")
  (Structural::Property, "p")
  (Structural::Link, "")
  (Structural::Bind, "")
  (Structural::Reference, "")
  (Structural::Mapping, "")

  (Structural::NoType, "e");

QString StructuralUtil::translateTypeToPrefix(Structural::StructuralType type)
{
  if(_mapTypeToPrefix.count(type))
    return _mapTypeToPrefix[type];
  else
    return "e";
}

std::map <QString, Structural::StructuralMedia> StructuralUtil::_mapExtensionToMedia =
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

Structural::StructuralMedia StructuralUtil::translateExtensionToMedia(const QString &extension)
{
  if(_mapExtensionToMedia.count(extension))
    return _mapExtensionToMedia[extension];
  else
    return Structural::NoMedia;
}



bool StructuralUtil::validateKinship(StructuralType entityType, StructuralType parentType)
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

bool StructuralUtil::isConditionRole(const QString &role)
{
  return isConditionRole(_mapStringToRole[role]);
}

bool StructuralUtil::isActionRole(const QString &role)
{
  return isActionRole(_mapStringToRole[role]);
}








QString StructuralUtil::formatId(const QString &id)
{
  QString tmp = id.normalized(QString::NormalizationForm_KD);
  tmp.remove(QRegExp("[^a-zA-Z_-\.\\s]"));
  if(tmp.at(0).isDigit())
    tmp = "_" + tmp;

  return tmp;
}






