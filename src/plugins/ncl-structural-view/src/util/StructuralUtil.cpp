#include "StructuralUtil.h"

#include <QUuid>

#include "StructuralEdge.h"
#include "StructuralView.h"

QString
StructuralUtil::createUid ()
{
  return QUuid::createUuid ().toString ();
}

QMap<QString, QString>
StructuralUtil::createProperties (qreal top, qreal left)
{
  QMap<QString, QString> properties;

  properties[STR_PROPERTY_ENTITY_TOP] = QString::number (top);
  properties[STR_PROPERTY_ENTITY_LEFT] = QString::number (left);

  return properties;
}

QMap<QString, QString>
StructuralUtil::createProperties (qreal top, qreal left, qreal width,
                                  qreal height)
{
  QMap<QString, QString> properties;

  properties[STR_PROPERTY_ENTITY_TOP] = QString::number (top);
  properties[STR_PROPERTY_ENTITY_LEFT] = QString::number (left);
  properties[STR_PROPERTY_ENTITY_WIDTH] = QString::number (width);
  properties[STR_PROPERTY_ENTITY_HEIGHT] = QString::number (height);

  return properties;
}

QMap<QString, QString>
StructuralUtil::createSettings ()
{
  return createSettings (true, true);
}

QMap<QString, QString>
StructuralUtil::createSettings (bool undo, bool notify)
{
  return createSettings ((undo ? STR_VALUE_TRUE : STR_VALUE_FALSE),
                         (notify ? STR_VALUE_TRUE : STR_VALUE_FALSE),
                         createUid ());
}

QMap<QString, QString>
StructuralUtil::createSettings (const QString &undo, const QString &notify,
                                const QString &code)
{
  QMap<QString, QString> settings;

  if (undo.isEmpty ())
    settings.insert (STR_SETTING_UNDO, STR_VALUE_TRUE);
  else
    settings.insert (STR_SETTING_UNDO, undo);

  if (notify.isEmpty ())
    settings.insert (STR_SETTING_NOTIFY, STR_VALUE_TRUE);
  else
    settings.insert (STR_SETTING_NOTIFY, notify);

  if (code.isEmpty ())
    settings.insert (STR_SETTING_CODE, createUid ());
  else
    settings.insert (STR_SETTING_CODE, code);

  return settings;
}

QMap<QString, QString>
StructuralUtil::createCoreTranslations (StructuralType type)
{
  QMap<QString, QString> translations;

  switch (type)
  {
    case Structural::Body:
    {
      translations[NCL_ATTR_ID] = STR_PROPERTY_ENTITY_ID;

      break;
    }

    case Structural::Context:
    case Structural::Switch:
    {
      translations[NCL_ATTR_ID] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTR_REFER] = STR_PROPERTY_REFERENCE_REFER_ID;

      break;
    }

    case Structural::Media:
    {
      translations[NCL_ATTR_ID] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTR_REFER] = STR_PROPERTY_REFERENCE_REFER_ID;
      translations[NCL_ATTR_INSTANCE] = STR_PROPERTY_CONTENT_INSTANCE;
      translations[NCL_ATTR_TYPE] = STR_PROPERTY_CONTENT_TYPE;
      translations[NCL_ATTR_SRC] = STR_PROPERTY_CONTENT_LOCATION;
      translations[NCL_ATTR_DESCRIPTOR] = NCL_ATTR_DESCRIPTOR;

      break;
    }

    case Structural::Port:
    {
      translations[NCL_ATTR_ID] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTR_COMPONENT]
          = STR_PROPERTY_REFERENCE_COMPONENT_ID;
      translations[NCL_ATTR_INTERFACE]
          = STR_PROPERTY_REFERENCE_INTERFACE_ID;

      break;
    }

    case Structural::Property:
    {
      translations[NCL_ATTR_NAME] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTR_VALUE] = NCL_ATTR_VALUE;
      translations[NCL_ATTR_EXTERNABLE] = NCL_ATTR_EXTERNABLE;
      break;
    }

    case Structural::Area:
    {
      translations[NCL_ATTR_ID] = STR_PROPERTY_ENTITY_ID;

      translations[NCL_ATTR_COORDS] = NCL_ATTR_COORDS;
      translations[NCL_ATTR_BEGIN] = NCL_ATTR_BEGIN;
      translations[NCL_ATTR_END] = NCL_ATTR_END;
      translations[NCL_ATTR_BEGINTEXT] = NCL_ATTR_BEGINTEXT;
      translations[NCL_ATTR_ENDTEXT] = NCL_ATTR_ENDTEXT;
      translations[NCL_ATTR_BEGINPOSITION] = NCL_ATTR_BEGINPOSITION;
      translations[NCL_ATTR_ENDPOSITION] = NCL_ATTR_ENDPOSITION;
      translations[NCL_ATTR_FIRST] = NCL_ATTR_FIRST;
      translations[NCL_ATTR_LAST] = NCL_ATTR_LAST;
      translations[NCL_ATTR_LABEL] = NCL_ATTR_LABEL;
      translations[NCL_ATTR_CLIP] = NCL_ATTR_CLIP;

      break;
    }

    case Structural::SwitchPort:
    {
      translations[NCL_ATTR_ID] = STR_PROPERTY_ENTITY_ID;

      break;
    }

    case Structural::Link:
    {
      translations[NCL_ATTR_ID] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTR_XCONNECTOR]
          = STR_PROPERTY_REFERENCE_XCONNECTOR_ID;

      break;
    }

    case Structural::Bind:
    {
      translations[NCL_ATTR_ROLE] = STR_PROPERTY_ENTITY_ID;
      translations[NCL_ATTR_XCONNECTOR]
          = STR_PROPERTY_REFERENCE_XCONNECTOR_ID;
      translations[NCL_ATTR_COMPONENT]
          = STR_PROPERTY_REFERENCE_COMPONENT_ID;
      translations[NCL_ATTR_INTERFACE]
          = STR_PROPERTY_REFERENCE_INTERFACE_ID;
      translations[NCL_ATTR_DESCRIPTOR] = NCL_ATTR_DESCRIPTOR;

      break;
    }

    case Structural::Mapping:
    {
      translations[NCL_ATTR_COMPONENT]
          = STR_PROPERTY_REFERENCE_COMPONENT_ID;
      translations[NCL_ATTR_INTERFACE]
          = STR_PROPERTY_REFERENCE_INTERFACE_ID;

      break;
    }

    default:
      break;
  }

  return translations;
}

QMap<QString, QString>
StructuralUtil::createPluginTranslations (StructuralType type)
{
  QMap<QString, QString> translations;
  QMap<QString, QString> inverted = createCoreTranslations (type);

  foreach (const QString &key, inverted.keys ())
    translations[inverted.value (key)] = key;

  return translations;
}

const std::map<Structural::Category, QString>
    StructuralUtil::_mapCategoryToString
    = { {Structural::Node, "node"},
        {Structural::Edge, "edge"},
        {Structural::Interface, "interface"},
        {Structural::NoCategory, ""} };

const std::map<QString, Structural::Category>
    StructuralUtil::_mapStringToCategory
    = invert<QString, Structural::Category> (
        StructuralUtil::_mapCategoryToString);

QString
StructuralUtil::translateCategoryToString (StructuralCategory category)
{
  if (_mapCategoryToString.count (category))
    return _mapCategoryToString.at (category);
  else
    return "";
}

StructuralCategory
StructuralUtil::translateStringToCategory (const QString &category)
{
  if (_mapStringToCategory.count (category))
    return _mapStringToCategory.at (category);
  else
    return Structural::NoCategory;
}

const std::map<Structural::Type, QString> StructuralUtil::_mapTypeToString
    = { {Structural::Media, "media"},
        {Structural::Body, "body"},
        {Structural::Context, "context"},
        {Structural::Switch, "switch"},
        {Structural::Port, "port"},
        {Structural::SwitchPort, "switchPort"},
        {Structural::Area, "area"},
        {Structural::Property, "property"},
        {Structural::Link, "link"},
        {Structural::Bind, "bind"},
        {Structural::Reference, "reference"},
        {Structural::Mapping, "mapping"},
        {Structural::NoType, ""} };

const std::map<QString, Structural::Type> StructuralUtil::_mapStringToType
    = invert<QString, Structural::Type> (
        StructuralUtil::_mapTypeToString);

QString
StructuralUtil::translateTypeToString (StructuralType type)
{
  if (_mapTypeToString.count (type))
    return _mapTypeToString.at (type);
  else
    return "";
}

StructuralType
StructuralUtil::translateStringToType (const QString &type)
{
  if (_mapStringToType.count (type))
    return _mapStringToType.at (type);
  else
    return Structural::NoType;
}

const std::map<Structural::Role, QString> StructuralUtil::_mapRoleToString
    = { {Structural::onBegin, "onBegin"},
        {Structural::onEnd, "onEnd"},
        {Structural::onSelection, "onSelection"},
        {Structural::onResume, "onResume"},
        {Structural::onPause, "onPause"},
        {Structural::onBeginAttribution, "onBeginAttribution"},
        {Structural::onEndAttribution, "onEndAttribution"},
        {Structural::onPauseAttribution, "onPauseAttribution"},
        {Structural::onResumeAttribution, "onResumeAttribution"},
        {Structural::Start, "start"},
        {Structural::Stop, "stop"},
        {Structural::Resume, "resume"},
        {Structural::Pause, "pause"},
        {Structural::Set, "set"},
        {Structural::NoRole, ""} };

const std::map<QString, Structural::Role> StructuralUtil::_mapStringToRole
    = invert<QString, Structural::Role> (
        StructuralUtil::_mapRoleToString);

QString
StructuralUtil::translateRoleToString (StructuralRole role)
{
  if (_mapRoleToString.count (role))
    return _mapRoleToString.at (role);
  else
    return "";
}

StructuralRole
StructuralUtil::translateStringToRole (const QString &role)
{
  if (_mapStringToRole.count (role))
    return _mapStringToRole.at (role);
  else
    return Structural::NoRole;
}

const std::map<Structural::MimeType, QString>
    StructuralUtil::_mapMimeTypeToString
    = { {Structural::Text, "text"},
        {Structural::Image, "image"},
        {Structural::Audio, "audio"},
        {Structural::Video, "video"},
        {Structural::HTML, "html"},
        {Structural::NCL, "ncl"},
        {Structural::NCLua, "nclua"},
        {Structural::Settings, "settings"},
        {Structural::Time, "time"},
        {Structural::NoMimeType, ""} };

const std::map<QString, Structural::MimeType>
    StructuralUtil::_mapStringToMimeType
    = invert<QString, Structural::MimeType> (
        StructuralUtil::_mapMimeTypeToString);

QString
StructuralUtil::translateMimeTypeToString (StructuralMimeType mimetype)
{
  if (_mapMimeTypeToString.count (mimetype))
    return _mapMimeTypeToString.at (mimetype);
  else
    return "media";
}

StructuralMimeType
StructuralUtil::translateStringToMimeType (const QString &mimetype)
{
  if (_mapStringToMimeType.count (mimetype))
    return _mapStringToMimeType.at (mimetype);
  else
    return Structural::NoMimeType;
}

const std::map<Structural::Type, QString> StructuralUtil::_entitiesIcon
    = { {Structural::Media, ":/icon/media"},
        {Structural::Body, ":/icon/body"},
        {Structural::Context, ":/icon/context"},
        {Structural::Switch, ":/icon/switch"},
        {Structural::Port, ":/icon/port-mini"},
        {Structural::SwitchPort, ":/icon/switchport-mini"},
        {Structural::Area, ":/icon/area-mini"},
        {Structural::Property, ":/icon/property-mini"},
        {Structural::Link, ""},
        {Structural::Bind, ""},
        {Structural::Reference, ""},
        {Structural::Mapping, ""},
        {Structural::NoType, ""} };

QString
StructuralUtil::getIcon (StructuralType type)
{
  if (_entitiesIcon.count (type))
    return _entitiesIcon.at (type);
  else
    return "";
}

const std::map<Structural::Type, QString> StructuralUtil::_entitiesColor
    = { {Structural::Media, ""},
        {Structural::Body, "#EEEEEE"},
        {Structural::Context, "#F4E4CC"},
        {Structural::Switch, "#C6E2FF"},
        {Structural::Port, ""},
        {Structural::SwitchPort, ""},
        {Structural::Area, ""},
        {Structural::Property, ""},
        {Structural::Link, "#000000"},
        {Structural::Bind, "#000000"},
        {Structural::Reference, "#000000"},
        {Structural::Mapping, "#5C0099"},
        {Structural::NoType, ""} };

QString
StructuralUtil::getColor (StructuralType type)
{
  if (_entitiesColor.count (type))
    return _entitiesColor.at (type);
  else
    return "";
}

const std::map<Structural::Type, QString> StructuralUtil::_entitiesPrefix
    = { {Structural::Media, "m"},
        {Structural::Body, "b"},
        {Structural::Context, "ctx"},
        {Structural::Switch, "swt"},
        {Structural::Port, "p"},
        {Structural::SwitchPort, "swtp"},
        {Structural::Area, "a"},
        {Structural::Property, "p"},
        {Structural::Link, "l"},
        {Structural::Bind, ""},
        {Structural::Reference, ""},
        {Structural::Mapping, ""},
        {Structural::NoType, "e"} };

QString
StructuralUtil::getPrefix (StructuralType type)
{
  if (_entitiesPrefix.count (type))
    return _entitiesPrefix.at (type);
  else
    return "e";
}

QString
StructuralUtil::getTooltip (StructuralType type, const QString &title,
                            const QString &info, const QString &warning,
                            const QString &error, const QString &extra)
{
  QString tooltip;

  // Adding type
  tooltip += translateTypeToString (type);
  tooltip += " ";

  // Adding title
  if (!title.isEmpty ())
    tooltip += "(" + title + ")";
  else
    tooltip += "(?)";

  tooltip += " ";

  // Adding extra
  if (type == Structural::Link)
  {
    if (!extra.isEmpty ())
      tooltip += ": " + extra;
    else
      tooltip += ": Unknow";

    tooltip += " ";
  }

  // Adding messages
  if (!error.isEmpty ())
    tooltip += "- Error: " + error;
  else if (!warning.isEmpty ())
    tooltip += "- Warning: " + warning;
  else if (!info.isEmpty ())
    tooltip += "- Info: " + info;

  // Formating
  tooltip[0] = tooltip[0].toUpper ();

  return tooltip;
}

const std::map<Structural::MimeType, QString>
    StructuralUtil::_mimetypesIcon
    = { {Structural::Text, ":/icon/media-text"},
        {Structural::Image, ":/icon/media-image"},
        {Structural::Audio, ":/icon/media-audio"},
        {Structural::Video, ":/icon/media-video"},
        {Structural::HTML, ":/icon/media-text-html"},
        {Structural::NCL, ":/icon/media-ncl"},
        {Structural::NCLua, ":/icon/media-nclua"},
        {Structural::Settings, ":/icon/media-settings"},
        {Structural::Time, ":/icon/media-time"},
        {Structural::NoMimeType, ":/icon/media"} };

QString
StructuralUtil::getMimeTypeIcon (StructuralMimeType type)
{
  if (_mimetypesIcon.count (type))
    return _mimetypesIcon.at (type);
  else
    return ":/icon/media";
}

QString
StructuralUtil::getMimeTypeTooltip (StructuralMimeType mimetype,
                                    const QString &title, const QString &info,
                                    const QString &warning,
                                    const QString &error, const QString &extra)
{
  Q_UNUSED (extra);

  QString tooltip;

  // Adding type
  tooltip += translateMimeTypeToString (mimetype);
  tooltip += " ";

  // Adding title
  if (!title.isEmpty ())
    tooltip += "(" + title + ")";
  else
    tooltip += "(?)";

  tooltip += " ";

  // Adding extra
  // nothing...

  // Adding messages
  if (!error.isEmpty ())
    tooltip += "- Error: " + error;
  else if (!warning.isEmpty ())
    tooltip += "- Warning: " + warning;
  else if (!info.isEmpty ())
    tooltip += "- Info: " + info;

  // Formating
  tooltip[0] = tooltip[0].toUpper ();

  return tooltip;
}

const std::map<QString, Structural::MimeType>
    StructuralUtil::_mimetypesExtension
    = { {"txt", Structural::Text},
        {"png", Structural::Image},
        {"jpg", Structural::Image},
        {"jpeg", Structural::Image},
        {"gif", Structural::Image},
        {"mp3", Structural::Audio},
        {"wav", Structural::Audio},
        {"mp4", Structural::Video},
        {"mpeg4", Structural::Video},
        {"mpeg", Structural::Video},
        {"mpg", Structural::Video},
        {"mov", Structural::Video},
        {"avi", Structural::Video},
        {"mkv", Structural::Video},
        {"htm", Structural::HTML},
        {"html", Structural::HTML},
        {"ncl", Structural::NCL},
        {"lua", Structural::NCLua} };

Structural::MimeType
StructuralUtil::getMimeTypeByExtension (const QString &extension)
{
  if (_mimetypesExtension.count (extension))
    return _mimetypesExtension.at (extension);
  else
    return Structural::NoMimeType;
}

const std::map<Structural::Role, QString> StructuralUtil::_rolesIcon
    = { {Structural::onBegin, ":/icon/bind-onbegin"},
        {Structural::onEnd, ":/icon/bind-onend"},
        {Structural::onSelection, ":/icon/bind-onselection"},
        {Structural::onResume, ":/icon/bind-onresume"},
        {Structural::onPause, ":/icon/bind-onpause"},
        {Structural::onBeginAttribution, ""},
        {Structural::onEndAttribution, ""},
        {Structural::onPauseAttribution, ""},
        {Structural::onResumeAttribution, ""},
        {Structural::Start, ":/icon/bind-start"},
        {Structural::Stop, ":/icon/bind-stop"},
        {Structural::Resume, ":/icon/bind-resume"},
        {Structural::Pause, ":/icon/bind-pause"},
        {Structural::Set, ":/icon/bind-set"},
        {Structural::NoRole, ""} };

QString
StructuralUtil::getIcon (StructuralRole role)
{
  if (_rolesIcon.count (role))
    return _rolesIcon.at (role);
  else
    return "";
}

QVector<StructuralEntity *>
StructuralUtil::getNeighbors (StructuralEntity *entity)
{
  QVector<StructuralEntity *> neighbors;

  StructuralEntity *parent = entity->getParent ();

  if (parent != NULL)
  {
    neighbors = parent->getChildren ();
  }
  else if (!STR_DEFAULT_WITH_BODY)
  {
    if (entity->scene () != NULL)
    {
      StructuralView *view
          = (StructuralView *)entity->scene ()->views ().at (1);

      foreach (StructuralEntity *current, view->getEntities ().values ())
        if (current->getParent () == NULL && current != entity)
          neighbors += current;
    }
  }

  return neighbors;
}

QVector<StructuralEntity *>
StructuralUtil::getUpNeighbors (StructuralEntity *entity)
{
  QVector<StructuralEntity *> neighbors;

  StructuralEntity *parent = entity->getParent ();

  if (parent != NULL)
  {
    if (parent->getParent () != NULL)
    {
      neighbors += parent->getParent ()->getChildren ();
    }
    else if (!STR_DEFAULT_WITH_BODY)
    {
      if (entity->scene () != NULL)
      {
        StructuralView *view
            = (StructuralView *)entity->scene ()->views ().at (1);

        foreach (StructuralEntity *current, view->getEntities ().values ())
          if (current->getParent () == NULL && current != entity)
            neighbors += current;
      }
    }
  }

  return neighbors;
}

bool
StructuralUtil::isCondition (StructuralRole role)
{

  return (role == Structural::onBegin || role == Structural::onBeginAttribution
          || role == Structural::onEnd || role == Structural::onEndAttribution
          || role == Structural::onPause
          || role == Structural::onPauseAttribution
          || role == Structural::onResume
          || role == Structural::onResumeAttribution
          || role == Structural::onSelection);
}

bool
StructuralUtil::isCondition (const QString &role)
{
  return (_mapStringToRole.count (role) ? isCondition (_mapStringToRole.at (role))
                                        : false);
}

bool
StructuralUtil::isAction (StructuralRole role)
{
  return (role != Structural::NoRole && !isCondition (role));
}

bool
StructuralUtil::isAction (const QString &role)
{
  return isAction (_mapStringToRole.at (role));
}

void
StructuralUtil::adjustEdges (StructuralEntity *entity)
{
  QVector<StructuralEntity *> relatives;
  relatives += getNeighbors (entity);
  relatives += getUpNeighbors (entity);

  foreach (StructuralEntity *relative, relatives)
  {
    if (relative->getCategory () == Structural::Edge)
    {
      StructuralEdge *edge = (StructuralEdge *)relative;

      if (edge->getTail () == entity || edge->getHead () == entity)
        edge->adjust (true);
    }
  }
}

QString
StructuralUtil::formatId (const QString &id)
{
  QString normalized = id.normalized (QString::NormalizationForm_KD);
  normalized.remove (QRegExp ("[^a-zA-Z_-\\.\\s]"));

  if (normalized.at (0).isDigit ())
    normalized = "_" + normalized;

  return normalized;
}

bool
StructuralUtil::validateKinship (StructuralType entityType,
                                 StructuralType parentType)
{
  switch (entityType)
  {
    case Structural::Body:
    {
      return false;
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
      switch (parentType)
      {
        case Structural::Body:
        case Structural::Switch:
        case Structural::Context:
        {
          return true;
        }

        default:
        {
          return false;
        }
      }
      break;
    }

    case Structural::SwitchPort:
    case Structural::Mapping:
    {
      switch (parentType)
      {
        case Structural::Switch:
        {
          return true;
        }

        default:
        {
          return false;
        }
      }
      break;
    }

    case Structural::Area:
    {
      switch (parentType)
      {
        case Structural::Media:
        {
          return true;
        }

        default:
        {
          return false;
        }
      }
      break;
    }

    default:
    {
      return false;
    }
  }
}
