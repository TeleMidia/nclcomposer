#ifndef QNSTUTIL_H
#define QNSTUTIL_H

#include <QMap>

#include "qnstgraphicsentity.h"

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
                                                QnstGraphicsEntity *parent = 0);

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

private:
  static std::map<Qnst::EntityType, QString> iconFromTypeMap;
};

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

#endif
