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

  static QString translateCategoryToString(StructualCategory category);
  static StructualCategory translateStringToCategory(const QString &category);

  static QString translateTypeToString(StructuralType type);
  static StructuralType translateStringToType(const QString &type);

  static StructuralRole translateStringToRole(const QString &role);
  static QString translateRoleToString(StructuralRole role);

  static QString translateTypeToIcon(StructuralType type);
  static QString translateMediaToIcon(StructuralMedia type);



  /*!
   * \brief Returns the Qnst::EntitySubtype based on the media mimetype.
   *
   * \arg mimeType the mime-type.
   *
   * \return the qnst media type.
   */
  static Structural::StructuralMedia getnstTypeFromMime(const QString &mimeType);



  /*!
   * \brief Returns the media type based on the extension of the media source.
   *
   * \arg ext the extension of the path.
   * \return the media type associated with that extension.
   */
  static Structural::StructuralMedia getnstTypeFromExtension(const QString &ext);

  static QString normalizeXMLID(const QString &id);

  /*!
   * \brief Returns the media type based on the string the represent that type.
   *
   * \arg strType the type specified in a string.
   * \return the media type associated with that string.
   */
  static Structural::StructuralType getnstTypeFromStr(const QString &strType);

  /*!
   * \brief Returns the string that represents the type.
   *
   * One of the use of this function is to serialize the Qnst::EntitySubtype.
   *
   * \arg type the entity type
   * \return the string that represent that type.
   */
  static QString getStrFromNstType(Structural::StructuralType type);

  /*!
   * \brief Returns a media identifier prefix for an specific qnst entity type.
   *
   * \arg type the qnst entity type.
   * \return the prefix for the entity type.
   */
  static QString getPrefixIdFromType(Structural::StructuralType type);





  static bool isConditionRole(StructuralRole role);
  static bool isActionRole(StructuralRole role);
  static bool isConditionRole(QString role);
  static bool isActionRole(QString role);
  static bool hasValidKinshp(StructuralType subtype, StructuralType parent);

  static QMap<QString,QString> createCoreTranslationMap(StructuralType type);
  static QMap<QString,QString> createViewTranslationMap(StructuralType type);



private:
  static std::map <StructualCategory, QString> _mapCategoryToString;
  static std::map <QString, StructualCategory> _mapStringToCategory;
  static std::map <StructuralType, QString> _mapTypeToString;
  static std::map <QString, StructuralType> _mapStringToType;
  static std::map <StructuralRole, QString> _mapRoleToString;
  static std::map <QString, StructuralRole> _mapStringToRole;
  static std::map <StructuralType, QString> _mapTypeToIcon;
  static std::map <StructuralMedia, QString>  _mapMediaToIcon;
  static std::map  <StructuralType, QString>  _mapTypeToPrefix;
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
