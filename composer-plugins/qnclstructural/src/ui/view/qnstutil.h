#ifndef QNSTUTIL_H
#define QNSTUTIL_H

#include <algorithm>
#include <QMap>

#include "qnstgraphicsentity.h"

class QnstGraphicsEntity;

class QnstUtil {
public:
  /*!
   * \brief Creates an entity based on the type passed as parameter.
   *
   * \arg type the type of entity to be created.
   * \arg parent the parent of the entity to be created.
   *
   * \return a new QnstGraphicsEntity
   */
  static QnstGraphicsEntity *makeGraphicsEntity(Qnst::EntityType type,
                                                QnstGraphicsEntity *parent = 0,
                                                const QMap<QString, QString> &props = (QMap<QString,QString>()));

  /*!
   * \brief Returns the Qnst::EntityType based on the media mimetype.
   *
   * \arg mimeType the mime-type.
   *
   * \return the qnst media type.
   */
  static Qnst::EntityType getnstTypeFromMime(const QString &mimeType);

  /*!
   * \brief Returns the icon that represents the Qnst::EntityType.
   *
   * \arg type the Qnst::EntityType.
   * \return the path to the icon of that type.
   */
  static QString iconFromMediaType(Qnst::EntityType type);

  /*!
   * \brief Returns the media type based on the extension of the media source.
   *
   * \arg ext the extension of the path.
   * \return the media type associated with that extension.
   */
  static Qnst::EntityType getnstTypeFromExtension(const QString &ext);

  /*!
   * \brief Returns the media type based on the string the represent that type.
   *
   * \arg strType the type specified in a string.
   * \return the media type associated with that string.
   */
  static Qnst::EntityType getnstTypeFromStr(const QString &strType);

  /*!
   * \brief Returns the string that represents the type.
   *
   * One of the use of this function is to serialize the Qnst::EntityType.
   *
   * \arg type the entity type
   * \return the string that represent that type.
   */
  static QString getStrFromNstType(Qnst::EntityType type);

  /*!
   * \brief Returns a media identifier prefix for an specific qnst entity type.
   *
   * \arg type the qnst entity type.
   * \return the prefix for the entity type.
   */
  static QString getPrefixIdFromType(Qnst::EntityType type);

  /*!
   * \brief Returns the Qnst::BindType related to the string role passed as
   *    parameter.
   *
   * \arg role the string of the role (onBegin, onEnd, start, stop, ...)
   * \return the Qnst::BindType related to that string.
   */
  static Qnst::BindType getBindTypeFromStr(const QString &role);

  /*!
   * \brief Returns a string that represents the QnstBindType passed as
   *    parameter.
   *
   * \arg type the Qnst::BindType
   * \return a string that represents that type.
   */
  static QString getStrFromBindType(Qnst::BindType type);

private:
  static std::map <Qnst::EntityType, QString>   iconFromTypeMap;
  static std::map <QString, Qnst::EntityType>   typeFromExtMap;
  static std::map <QString, Qnst::EntityType>   typeFromStr;
  static std::map <Qnst::EntityType, QString>   strFromType;
  static std::map  <Qnst::EntityType, QString>  prefixIdFromType;
  static std::map <QString, Qnst::BindType>     bindTypeFromRoleStr;
  static std::map <Qnst::BindType, QString>     strFromBindType;
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
