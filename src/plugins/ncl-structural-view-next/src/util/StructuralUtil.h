#ifndef QNSTUTIL_H
#define QNSTUTIL_H

#include <algorithm>
#include <QMap>

#include "StructuralEntity.h"
//#include "StructuralMedia.h"

class StructuralEntity;
//class StructuralMedia;


class StructuralUtil {
public:
  /*!
   * \brief Creates an entity based on the type passed as parameter.
   *
   * \arg type the type of entity to be created.
   * \arg parent the parent of the entity to be created.
   *
   * \return a new QnstGraphicsEntity
   */
  static StructuralEntity *makeGraphicsEntity(Structural::StructuralType type,
                                                StructuralEntity *parent = 0,
                                                const QMap<QString, QString> &props = (QMap<QString,QString>()));

  /*!
   * \brief Returns the Qnst::EntitySubtype based on the media mimetype.
   *
   * \arg mimeType the mime-type.
   *
   * \return the qnst media type.
   */
  static Structural::StructuralMedia getnstTypeFromMime(const QString &mimeType);

  /*!
   * \brief Returns the icon that represents the Qnst::EntitySubtype.
   *
   * \arg type the Qnst::EntitySubtype.
   * \return the path to the icon of that type.
   */
  static QString iconFromEntityType(Structural::StructuralType type);

  static QString iconFromMediaType(Structural::StructuralMedia type);

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

  /*!
   * \brief Returns the Qnst::StructuralRole related to the string role passed as
   *    parameter.
   *
   * \arg role the string of the role (onBegin, onEnd, start, stop, ...)
   * \return the Qnst::StructuralRole related to that string.
   */
  static Structural::StructuralRole getStructuralRoleFromStr(const QString &role);

  /*!
   * \brief Returns a string that represents the QnstStructuralRole passed as
   *    parameter.
   *
   * \arg type the Qnst::StructuralRole
   * \return a string that represents that type.
   */
  static QString getStrFromStructuralRole(Structural::StructuralRole type);

  static QString CreateUid();

  static bool isConditionRole(StructuralRole role);
  static bool isActionRole(StructuralRole role);
  static bool isConditionRole(QString role);
  static bool isActionRole(QString role);
  static bool hasValidKinshp(StructuralType subtype, StructuralType parent);
  static QString convertType(StructualCategory type);
  static QString convertSubtype(StructuralType subtype);
  static void dbg(QObject* object, QString message);

  static QMap<QString,QString> createCoreTranslationMap(StructuralType type);
  static QMap<QString,QString> createViewTranslationMap(StructuralType type);

  static QMap<QString,QString> createSettings(bool undo, bool notify);
  static QMap<QString,QString> createSettings(QString undo, QString notify, QString code);

private:
  static std::map <Structural::StructuralType, QString>   iconFromTypeMap;
  static std::map <Structural::StructuralMedia, QString>   iconFromMediaTypeMap;
  static std::map <QString, Structural::StructuralMedia>   typeFromExtMap;
  static std::map <QString, Structural::StructuralType>   typeFromStr;
  static std::map <Structural::StructuralType, QString>   strFromType;
  static std::map  <Structural::StructuralType, QString>  prefixIdFromType;
  static std::map <QString, Structural::StructuralRole>     roleTypeFromRoleStr;
  static std::map <Structural::StructuralRole, QString>     strFromRoleType;

  static std::map <StructuralResize, QRectF> region;
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

#endif
