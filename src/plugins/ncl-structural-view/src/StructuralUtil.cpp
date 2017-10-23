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
  QMap<QString, QString> props;

  props[ST_ATTR_ENT_TOP] = QString::number (top);
  props[ST_ATTR_ENT_LEFT] = QString::number (left);

  return props;
}

QMap<QString, QString>
StructuralUtil::createProperties (qreal top, qreal left, qreal width,
                                  qreal height)
{
  QMap<QString, QString> props;

  props[ST_ATTR_ENT_TOP] = QString::number (top);
  props[ST_ATTR_ENT_LEFT] = QString::number (left);
  props[ST_ATTR_ENT_WIDTH] = QString::number (width);
  props[ST_ATTR_ENT_HEIGHT] = QString::number (height);

  return props;
}

QMap<QString, QString>
StructuralUtil::createSettings ()
{
  return createSettings (true, true);
}

QMap<QString, QString>
StructuralUtil::createSettings (bool undo, bool notify)
{
  return createSettings ((undo ? ST_VALUE_TRUE : ST_VALUE_FALSE),
                         (notify ? ST_VALUE_TRUE : ST_VALUE_FALSE),
                         createUid ());
}

QMap<QString, QString>
StructuralUtil::createSettings (const QString &undo, const QString &notify,
                                const QString &code)
{
  QMap<QString, QString> settings;

  if (undo.isEmpty ())
    settings.insert (ST_SETTINGS_UNDO, ST_VALUE_TRUE);
  else
    settings.insert (ST_SETTINGS_UNDO, undo);

  if (notify.isEmpty ())
    settings.insert (ST_SETTINGS_NOTIFY, ST_VALUE_TRUE);
  else
    settings.insert (ST_SETTINGS_NOTIFY, notify);

  if (code.isEmpty ())
    settings.insert (ST_SETTINGS_CODE, createUid ());
  else
    settings.insert (ST_SETTINGS_CODE, code);

  return settings;
}

QMap<QString, QString>
StructuralUtil::createCoreTranslations (StructuralType type)
{
  QMap<QString, QString> transl;

  switch (type)
  {
    case Structural::Body:
    {
      transl[NCL_ATTR_ID] = ST_ATTR_ENT_ID;

      break;
    }

    case Structural::Context:
    case Structural::Switch:
    {
      transl[NCL_ATTR_ID] = ST_ATTR_ENT_ID;
      transl[NCL_ATTR_REFER] = ST_ATTR_REFERENCE_REFER_ID;

      break;
    }

    case Structural::Media:
    {
      transl[NCL_ATTR_ID] = ST_ATTR_ENT_ID;
      transl[NCL_ATTR_REFER] = ST_ATTR_REFERENCE_REFER_ID;
      transl[NCL_ATTR_INSTANCE] = ST_ATTR_NODE_INSTANCE;
      transl[NCL_ATTR_TYPE] = ST_ATTR_NODE_TYPE;
      transl[NCL_ATTR_SRC] = ST_ATTR_NODE_SRC;
      transl[NCL_ATTR_DESCRIPTOR] = NCL_ATTR_DESCRIPTOR;

      break;
    }

    case Structural::Port:
    {
      transl[NCL_ATTR_ID] = ST_ATTR_ENT_ID;
      transl[NCL_ATTR_COMPONENT] = ST_ATTR_REFERENCE_COMPONENT_ID;
      transl[NCL_ATTR_INTERFACE] = ST_ATTR_REFERENCE_INTERFACE_ID;

      break;
    }

    case Structural::Property:
    {
      transl[NCL_ATTR_NAME] = ST_ATTR_ENT_ID;
      transl[NCL_ATTR_VALUE] = NCL_ATTR_VALUE;
      transl[NCL_ATTR_EXTERNABLE] = NCL_ATTR_EXTERNABLE;
      break;
    }

    case Structural::Area:
    {
      transl[NCL_ATTR_ID] = ST_ATTR_ENT_ID;

      transl[NCL_ATTR_COORDS] = NCL_ATTR_COORDS;
      transl[NCL_ATTR_BEGIN] = NCL_ATTR_BEGIN;
      transl[NCL_ATTR_END] = NCL_ATTR_END;
      transl[NCL_ATTR_BEGINTEXT] = NCL_ATTR_BEGINTEXT;
      transl[NCL_ATTR_ENDTEXT] = NCL_ATTR_ENDTEXT;
      transl[NCL_ATTR_BEGINPOSITION] = NCL_ATTR_BEGINPOSITION;
      transl[NCL_ATTR_ENDPOSITION] = NCL_ATTR_ENDPOSITION;
      transl[NCL_ATTR_FIRST] = NCL_ATTR_FIRST;
      transl[NCL_ATTR_LAST] = NCL_ATTR_LAST;
      transl[NCL_ATTR_LABEL] = NCL_ATTR_LABEL;
      transl[NCL_ATTR_CLIP] = NCL_ATTR_CLIP;

      break;
    }

    case Structural::SwitchPort:
    {
      transl[NCL_ATTR_ID] = ST_ATTR_ENT_ID;

      break;
    }

    case Structural::Link:
    {
      transl[NCL_ATTR_ID] = ST_ATTR_ENT_ID;
      transl[NCL_ATTR_XCONNECTOR] = ST_ATTR_REFERENCE_XCONNECTOR_ID;

      break;
    }

    case Structural::Bind:
    {
      transl[NCL_ATTR_ROLE] = ST_ATTR_ENT_ID;
      transl[NCL_ATTR_XCONNECTOR] = ST_ATTR_REFERENCE_XCONNECTOR_ID;
      transl[NCL_ATTR_COMPONENT] = ST_ATTR_REFERENCE_COMPONENT_ID;
      transl[NCL_ATTR_INTERFACE] = ST_ATTR_REFERENCE_INTERFACE_ID;
      transl[NCL_ATTR_DESCRIPTOR] = NCL_ATTR_DESCRIPTOR;

      break;
    }

    case Structural::Mapping:
    {
      transl[NCL_ATTR_COMPONENT] = ST_ATTR_REFERENCE_COMPONENT_ID;
      transl[NCL_ATTR_INTERFACE] = ST_ATTR_REFERENCE_INTERFACE_ID;

      break;
    }

    default:
      break;
  }

  return transl;
}

QMap<QString, QString>
StructuralUtil::createPluginTranslations (StructuralType type)
{
  QMap<QString, QString> transl;
  QMap<QString, QString> inverted = createCoreTranslations (type);

  foreach (const QString &key, inverted.keys ())
    transl[inverted.value (key)] = key;

  return transl;
}

const std::map<Structural::Category, QString>
    StructuralUtil::_categoryToString
    = { { Structural::Node, "node" },
        { Structural::Edge, "edge" },
        { Structural::Interface, "interface" },
        { Structural::NoCategory, "" } };

const std::map<QString, Structural::Category>
    StructuralUtil::_stringToCategory
    = invert<QString, Structural::Category> (
        StructuralUtil::_categoryToString);

QString
StructuralUtil::categoryToStr (StructuralCategory category)
{
  if (_categoryToString.count (category))
    return _categoryToString.at (category);
  else
    return "";
}

StructuralCategory
StructuralUtil::strToCategory (const QString &category)
{
  if (_stringToCategory.count (category))
    return _stringToCategory.at (category);
  else
    return Structural::NoCategory;
}

const std::map<Structural::Type, QString> StructuralUtil::_typeToString
    = { { Structural::Media, "media" },
        { Structural::Body, "body" },
        { Structural::Context, "context" },
        { Structural::Switch, "switch" },
        { Structural::Port, "port" },
        { Structural::SwitchPort, "switchPort" },
        { Structural::Area, "area" },
        { Structural::Property, "property" },
        { Structural::Link, "link" },
        { Structural::Bind, "bind" },
        { Structural::Reference, "reference" },
        { Structural::Mapping, "mapping" },
        { Structural::NoType, "" } };

const std::map<QString, Structural::Type> StructuralUtil::_stringToType
    = invert<QString, Structural::Type> (StructuralUtil::_typeToString);

QString
StructuralUtil::typeToStr (StructuralType type)
{
  if (_typeToString.count (type))
    return _typeToString.at (type);
  else
    return "";
}

StructuralType
StructuralUtil::strToType (const QString &type)
{
  if (_stringToType.count (type))
    return _stringToType.at (type);
  else
    return Structural::NoType;
}

const std::map<Structural::Role, QString> StructuralUtil::_roleToString
    = { { Structural::onBegin, "onBegin" },
        { Structural::onEnd, "onEnd" },
        { Structural::onSelection, "onSelection" },
        { Structural::onResume, "onResume" },
        { Structural::onPause, "onPause" },
        { Structural::onBeginAttribution, "onBeginAttribution" },
        { Structural::onEndAttribution, "onEndAttribution" },
        { Structural::onPauseAttribution, "onPauseAttribution" },
        { Structural::onResumeAttribution, "onResumeAttribution" },
        { Structural::Start, "start" },
        { Structural::Stop, "stop" },
        { Structural::Resume, "resume" },
        { Structural::Pause, "pause" },
        { Structural::Set, "set" },
        { Structural::NoRole, "" } };

const std::map<QString, Structural::Role> StructuralUtil::_stringToRole
    = invert<QString, Structural::Role> (StructuralUtil::_roleToString);

QString
StructuralUtil::roleToString (StructuralRole role)
{
  if (_roleToString.count (role))
    return _roleToString.at (role);
  else
    return "";
}

StructuralRole
StructuralUtil::stringToRole (const QString &role)
{
  if (_stringToRole.count (role))
    return _stringToRole.at (role);
  else
    return Structural::NoRole;
}

const std::map<Structural::MimeType, QString>
    StructuralUtil::_mimeTypeToString
    = { { Structural::Text, "text" },   { Structural::Image, "image" },
        { Structural::Audio, "audio" }, { Structural::Video, "video" },
        { Structural::HTML, "html" },   { Structural::NCL, "ncl" },
        { Structural::NCLua, "nclua" }, { Structural::Settings, "settings" },
        { Structural::Time, "time" },   { Structural::NoMimeType, "" } };

const std::map<QString, Structural::MimeType>
    StructuralUtil::_stringToMimeType
    = invert<QString, Structural::MimeType> (
        StructuralUtil::_mimeTypeToString);

QString
StructuralUtil::mimeTypeToString (StructuralMimeType mimetype)
{
  if (_mimeTypeToString.count (mimetype))
    return _mimeTypeToString.at (mimetype);
  else
    return "media";
}

StructuralMimeType
StructuralUtil::stringToMimeType (const QString &mimetype)
{
  if (_stringToMimeType.count (mimetype))
    return _stringToMimeType.at (mimetype);
  else
    return Structural::NoMimeType;
}

const std::map<Structural::Type, QString> StructuralUtil::_entitiesIcon
    = { { Structural::Media, ":/icon/media" },
        { Structural::Body, ":/icon/body" },
        { Structural::Context, ":/icon/context" },
        { Structural::Switch, ":/icon/switch" },
        { Structural::Port, ":/icon/port-mini" },
        { Structural::SwitchPort, ":/icon/switchport-mini" },
        { Structural::Area, ":/icon/area-mini" },
        { Structural::Property, ":/icon/property-mini" },
        { Structural::Link, "" },
        { Structural::Bind, "" },
        { Structural::Reference, "" },
        { Structural::Mapping, "" },
        { Structural::NoType, "" } };

QString
StructuralUtil::getIcon (StructuralType type)
{
  if (_entitiesIcon.count (type))
    return _entitiesIcon.at (type);
  else
    return "";
}

const std::map<Structural::Type, QString> StructuralUtil::_entitiesColor
    = { { Structural::Media, "" },
        { Structural::Body, "#EEEEEE" },
        { Structural::Context, "#F4E4CC" },
        { Structural::Switch, "#C6E2FF" },
        { Structural::Port, "" },
        { Structural::SwitchPort, "" },
        { Structural::Area, "" },
        { Structural::Property, "" },
        { Structural::Link, "#000000" },
        { Structural::Bind, "#000000" },
        { Structural::Reference, "#000000" },
        { Structural::Mapping, "#5C0099" },
        { Structural::NoType, "" } };

QString
StructuralUtil::getColor (StructuralType type)
{
  if (_entitiesColor.count (type))
    return _entitiesColor.at (type);
  else
    return "";
}

const std::map<Structural::Type, QString> StructuralUtil::_entitiesPrefix
    = { { Structural::Media, "m" },     { Structural::Body, "b" },
        { Structural::Context, "ctx" }, { Structural::Switch, "swt" },
        { Structural::Port, "p" },      { Structural::SwitchPort, "swtp" },
        { Structural::Area, "a" },      { Structural::Property, "p" },
        { Structural::Link, "l" },      { Structural::Bind, "" },
        { Structural::Reference, "" },  { Structural::Mapping, "" },
        { Structural::NoType, "e" } };

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
  tooltip += typeToStr (type);
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

const std::map<Structural::MimeType, QString> StructuralUtil::_mimetypesIcon
    = { { Structural::Text, ":/icon/media-text" },
        { Structural::Image, ":/icon/media-image" },
        { Structural::Audio, ":/icon/media-audio" },
        { Structural::Video, ":/icon/media-video" },
        { Structural::HTML, ":/icon/media-text-html" },
        { Structural::NCL, ":/icon/media-ncl" },
        { Structural::NCLua, ":/icon/media-nclua" },
        { Structural::Settings, ":/icon/media-settings" },
        { Structural::Time, ":/icon/media-time" },
        { Structural::NoMimeType, ":/icon/media" } };

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
  tooltip += mimeTypeToString (mimetype);
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
    = { { "txt", Structural::Text },    { "png", Structural::Image },
        { "jpg", Structural::Image },   { "jpeg", Structural::Image },
        { "gif", Structural::Image },   { "mp3", Structural::Audio },
        { "wav", Structural::Audio },   { "mp4", Structural::Video },
        { "mpeg4", Structural::Video }, { "mpeg", Structural::Video },
        { "mpg", Structural::Video },   { "mov", Structural::Video },
        { "avi", Structural::Video },   { "mkv", Structural::Video },
        { "htm", Structural::HTML },    { "html", Structural::HTML },
        { "ncl", Structural::NCL },     { "lua", Structural::NCLua } };

Structural::MimeType
StructuralUtil::getMimeTypeByExtension (const QString &extension)
{
  if (_mimetypesExtension.count (extension))
    return _mimetypesExtension.at (extension);
  else
    return Structural::NoMimeType;
}

const std::map<Structural::Role, QString> StructuralUtil::_rolesIcon
    = { { Structural::onBegin, ":/icon/bind-onbegin" },
        { Structural::onEnd, ":/icon/bind-onend" },
        { Structural::onSelection, ":/icon/bind-onselection" },
        { Structural::onResume, ":/icon/bind-onresume" },
        { Structural::onPause, ":/icon/bind-onpause" },
        { Structural::onBeginAttribution, "" },
        { Structural::onEndAttribution, "" },
        { Structural::onPauseAttribution, "" },
        { Structural::onResumeAttribution, "" },
        { Structural::Start, ":/icon/bind-start" },
        { Structural::Stop, ":/icon/bind-stop" },
        { Structural::Resume, ":/icon/bind-resume" },
        { Structural::Pause, ":/icon/bind-pause" },
        { Structural::Set, ":/icon/bind-set" },
        { Structural::NoRole, "" } };

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
  else if (!ST_DEFAULT_WITH_BODY)
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
    else if (!ST_DEFAULT_WITH_BODY)
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
  return (_stringToRole.count (role)
              ? isCondition (_stringToRole.at (role))
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
  return _stringToRole.count (role)
         && isAction (_stringToRole.at (role));
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
