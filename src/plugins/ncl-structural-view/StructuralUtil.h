#ifndef STRUCTURALUTIL_H
#define STRUCTURALUTIL_H

#include <QMap>
#include <QObject>

#include <QLoggingCategory>
#include "StructuralEntity.h"

class StructuralEntity;

class StructuralUtil
{
public:
  static QString createUid ();

  static QMap<QString, QString> createProperties (qreal top, qreal left);
  static QMap<QString, QString> createProperties (qreal top, qreal left,
                                                  qreal width, qreal height);

  static QMap<QString, QString> createSettings ();
  static QMap<QString, QString> createSettings (bool undo, bool notify);
  static QMap<QString, QString> createSettings (const QString &undo,
                                                const QString &notify,
                                                const QString &code);

  static QMap<QString, QString> createCoreTranslations (StructuralType type);
  static QMap<QString, QString> createPluginTranslations (StructuralType type);

  static QString categoryToStr (StructuralCategory category);
  static StructuralCategory strToCategory (const QString &category);

  static QString typeToStr (StructuralType type);
  static StructuralType strToType (const QString &type);

  static QString roleToString (StructuralRole role);
  static StructuralRole strToRole (const QString &role);

  static QString mimeToStr (StructuralMimeType mimetype);
  static StructuralMimeType strToMime (const QString &mimetype);

  static QString getIcon (StructuralType type);
  static QString getColor (StructuralType type);
  static QString getPrefix (StructuralType type);
  static QString getTooltip (StructuralType type, const QString &title = "",
                             const QString &info = "",
                             const QString &warning = "",
                             const QString &error = "",
                             const QString &extra = "");

  static QString getMimeTypeIcon (StructuralMimeType mimetype);
  static QString
  getMimeTooltip (StructuralMimeType mimetype, const QString &title = "",
                  const QString &info = "", const QString &warning = "",
                  const QString &error = "", const QString &extra = "");
  static StructuralMimeType getMimeByExtension (const QString &ext);

  static QString getIcon (StructuralRole role);

  static QVector<StructuralEntity *> getNeighbors (StructuralEntity *entity);
  static QVector<StructuralEntity *> getUpNeighbors (StructuralEntity *entity);

  static bool isCondition (StructuralRole role);
  static bool isCondition (const QString &role);
  static bool isAction (StructuralRole role);
  static bool isAction (const QString &role);

  static void adjustEdges (StructuralEntity *entity);

  static QString formatId (const QString &id);

  static bool validateKinship (StructuralType type, StructuralType parent);

private:
  static const std::map<StructuralCategory, QString> _categoryToStr;
  static const std::map<QString, StructuralCategory> _strToCategory;

  static const std::map<QString, StructuralType> _strToType;
  static const std::map<StructuralType, QString> _typeToStr;

  static const std::map<StructuralRole, QString> _roleToStr;
  static const std::map<QString, StructuralRole> _strToRole;

  static const std::map<StructuralMimeType, QString> _mimeToStr;
  static const std::map<QString, StructuralMimeType> _strToMime;

  static const std::map<StructuralType, QString> _entitiesIcon;
  static const std::map<StructuralType, QString> _entitiesColor;
  static const std::map<StructuralType, QString> _entitiesPrefix;

  static const std::map<StructuralMimeType, QString> _mimeIcon;
  static const std::map<QString, StructuralMimeType> _mimeExtension;

  static const std::map<StructuralRole, QString> _rolesIcon;
};

template <typename K, typename V>
std::pair<V, K>
flip (const std::pair<K, V> &pair)
{
  return std::make_pair (pair.second, pair.first);
}

template <typename K, typename V>
std::map<K, V>
invert (const std::map<V, K> &map)
{
  std::map<K, V> inverted;
  std::transform (map.begin (), map.end (),
                  std::inserter (inverted, inverted.begin ()), flip<V, K>);

  return inverted;
}

Q_DECLARE_LOGGING_CATEGORY(CPR_PLUGIN_STRUCT)

#endif // STRUCTURALUTIL_H
