#ifndef STRUCTURALUTIL_H
#define STRUCTURALUTIL_H

#include <QMap>

#include "StructuralEntity.h"

class StructuralEntity;

class StructuralUtil {
public:

  static QString createUid();

  static QMap<QString,QString> createSettings(bool undo, bool notify);
  static QMap<QString,QString> createSettings(bool undo, bool notify, const QString &code);
  static QMap<QString,QString> createSettings(const QString &undo, const QString &notify, const QString &code);

  static QMap<QString,QString> createComposerTranslations(StructuralType type);
  static QMap<QString,QString> createStructuralTranslations(StructuralType type);

  static QString translateCategoryToString(StructualCategory category);
  static StructualCategory translateStringToCategory(const QString &category);

  static QString translateTypeToString(StructuralType type);
  static StructuralType translateStringToType(const QString &type);

  static StructuralRole translateStringToRole(const QString &role);
  static QString translateRoleToString(StructuralRole role);

  static QString translateTypeToIcon(StructuralType type);
  static QString translateMediaToIcon(StructuralMedia type);

  static QString translateTypeToPrefix(StructuralType type);

  static StructuralMedia translateExtensionToMedia(const QString &extension);

  static bool isConditionRole(StructuralRole role);
  static bool isConditionRole(const QString &role);

  static bool isActionRole(StructuralRole role);
  static bool isActionRole(const QString &role);

  static QString formatId(const QString &id);

  static bool validateKinship(StructuralType type, StructuralType parent);

private:
  static std::map <StructualCategory, QString> _mapCategoryToString;
  static std::map <QString, StructualCategory> _mapStringToCategory;

  static std::map <StructuralType, QString> _mapTypeToString;
  static std::map <QString, StructuralType> _mapStringToType;

  static std::map <StructuralRole, QString> _mapRoleToString;
  static std::map <QString, StructuralRole> _mapStringToRole;

  static std::map  <StructuralType, QString>  _mapTypeToPrefix;

  static std::map <StructuralType, QString> _mapTypeToIcon;
  static std::map <StructuralMedia, QString>  _mapMediaToIcon;

  static std::map <QString, StructuralMedia>  _mapExtensionToMedia;
};

/*!
 * \brief A template class that simplify the initialization of std::map.
 */
template <typename T, typename U>
class create_map
{
private:
  std::map<T, U> m_map;

public:
  create_map(const T& key, const U& val)
  {
    m_map[key] = val;
  }

  create_map<T, U>& operator()(const T& key, const U& val)
  {
    m_map[key] = val;
    return *this;
  }

  operator std::map<T, U>()
  {
    return m_map;
  }
};

/*!
 * \brief A function that invert a pair <K, V> to <V, K>.
 */
template<typename K, typename V>
std::pair<V, K> flip_pair(const std::pair<K, V>& p)
{
  return std::make_pair(p.second, p.first);
}

/*!
 * \brief A function that invert one complete std::map from <key,value> to
 *    <value,key>.
 */
template <typename K, typename V>
std::map <K, V> invert_map (const std::map <V, K> &in)
{
  std::map<K, V> out;
  std::transform(in.begin(), in.end(),
                 std::inserter(out, out.begin()),
                 flip_pair<V, K>);
       return out;
}

#endif // STRUCTURALUTIL_H
