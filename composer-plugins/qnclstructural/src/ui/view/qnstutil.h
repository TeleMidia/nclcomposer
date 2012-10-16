#ifndef QNSTUTIL_H
#define QNSTUTIL_H

#include "qnstgraphicsentity.h"

class QnstUtil {
public:
  /*!
   * \brief Create an entity based on the type passed as parameter.
   *
   * \arg type the type of entity to be created.
   * \arg parent the parent of the entity to be created.
   */
  static QnstGraphicsEntity *makeGraphicsEntity(Qnst::EntityType type,
                                                QnstGraphicsEntity *parent = 0);
};

#endif
