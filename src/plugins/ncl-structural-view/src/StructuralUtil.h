#ifndef STRUCTURALUTIL_H
#define STRUCTURALUTIL_H

#include <QMap>
#include <QObject>

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

  static QString translateCategoryToString (StructuralCategory category);
  static StructuralCategory
  translateStringToCategory (const QString &category);

  static QString typeToString (StructuralType type);
  static StructuralType stringToType (const QString &type);

  static QString roleToString (StructuralRole role);
  static StructuralRole stringToRole (const QString &role);

  static QString mimeTypeToString (StructuralMimeType mimetype);
  static StructuralMimeType stringToMimeType (const QString &mimetype);

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
  getMimeTypeTooltip (StructuralMimeType mimetype, const QString &title = "",
                      const QString &info = "", const QString &warning = "",
                      const QString &error = "", const QString &extra = "");
  static StructuralMimeType getMimeTypeByExtension (const QString &extension);

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
  static const std::map<StructuralCategory, QString> _categoryToString;
  static const std::map<QString, StructuralCategory> _stringToCategory;

  static const std::map<QString, StructuralType> _stringToType;
  static const std::map<StructuralType, QString> _typeToString;

  static const std::map<StructuralRole, QString> _roleToString;
  static const std::map<QString, StructuralRole> _stringToRole;

  static const std::map<StructuralMimeType, QString> _mimeTypeToString;
  static const std::map<QString, StructuralMimeType> _stringToMimeType;

  static const std::map<StructuralType, QString> _entitiesIcon;
  static const std::map<StructuralType, QString> _entitiesColor;
  static const std::map<StructuralType, QString> _entitiesPrefix;

  static const std::map<StructuralMimeType, QString> _mimetypesIcon;
  static const std::map<QString, StructuralMimeType> _mimetypesExtension;

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

#endif // STRUCTURALUTIL_H
