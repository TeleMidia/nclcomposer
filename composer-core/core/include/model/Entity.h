#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QMap>
#include <QUuid>
#include <QDebug>

namespace composer{
    namespace core {
        namespace module {
            class MessageControl;
        }
    }
}

namespace composer {
    namespace core {
        namespace model {
            class Document;
            class Entity : public QObject
            {
                Q_OBJECT

                // The following classes are "reliable" and can acess the
                // private and protected members of Entity.
                friend class composer::core::model::Document;
                friend class composer::core::module::MessageControl;

            private:
                QMutex lockAtts;
                QMutex lockID;
                QMutex lockType;
                QMutex lockChildren;
                QMutex lockParent;
                QString _id;
                QString type;
                Entity* parent;
                bool deleteChildren; /* initial value is true */
                QMap<QString, Entity*> children;
                /** This <map> represents the attributes of the element
                *   the Key is the name of the attribute and the Value is
                *   the value that this attribute has.
                */
                QMap<QString, QString> atts;

            protected:
                //! This method is used to set an specific attribute of the
                //! element
                /*!
                  \param name - The name of the attribute to be set
                  \param value - The value this attribute is going to be set
                */
                inline void setAttribute(QString name, QString value) {
                    QMutexLocker locker(&lockAtts);
                    atts[name] = value;
                }

                inline void setAtrributes(QMap<QString,QString> &atts) {
                    QMutexLocker locker(&lockAtts);
                    for (QMap<QString,QString>::iterator it = atts.begin() ;
                    it != atts.end() ; it++)
                        this->setAttribute(it.key(), it.value());
                }

                inline void setType(QString type) {
                    QMutexLocker locker(&lockType);
                    this->type = type;
                }

                inline void setUniqueID(QString _id) {
                    QMutexLocker locker(&lockID);
                    this->_id = _id;
                }

                inline void setParent(Entity *entity) {
                    this->parent = entity;
                }

                //OBS: This addChild updates the parent referency
                inline bool addChild(Entity *entity) {
                    QMutexLocker locker(&lockChildren);
                    QString _id = entity->getUniqueId();
                    if(children.contains(_id))
                        return false;
                    children[_id] = entity;
                    entity->setParent(this);
                    return true;
                }

                //! This call deletes the child and his children in a recursive
                //!     way
                bool deleteChild(Entity *entity);
                //! This call removes the child and append his children to his
                //!     parent
                bool removeChildAppendChildren(Entity *entity);

                explicit Entity(QObject *parent = 0);
                Entity(QMap<QString,QString> &atts, QObject *parent = 0);

                ~Entity();

                void print();

            public:

                //! This method is used to get an specific attribute of the
                //!     element
                /*!
                  \param name - The name of the attribute been requested
                  \return A string with the requested attribute.
                */
                inline QString getAttribute(QString name) {
                    QMutexLocker locker(&lockAtts);
                    return atts.contains(name) ? atts[name] : "";
                }
                //! This method is used to get the interetator in the <map> of
                //!     attributes
                /*!
                  \param begin - a reference to be filled with the begin of the
                            map.
                  \param end - a reference to be filled with the end of the map.
                */
                inline void getAttributeIterator (
                        QMap<QString,QString>::iterator &begin,
                        QMap<QString,QString>::iterator &end) {
                    QMutexLocker locker(&lockAtts);
                    begin = this->atts.begin();
                    end = this->atts.end();
                }

                //! This method is used to verify if this element has certain
                //!     attribute
                /*!
                  \param name - The name of the attribute to be verified
                  \return an boolean depending of the existence of the attribute
                */
                inline bool hasAttribute(const QString &name) {
                    QMutexLocker locker(&lockAtts);
                    return this->atts.contains(name);
                }

                inline QString getUniqueId() {
                    QMutexLocker locker(&lockID);
                    return this->_id;
                }

                inline QString getType() {
                    QMutexLocker locker(&lockType);
                    return this->type;
                }

                inline void getChildrenIterator(
                        QMap<QString,Entity*>::iterator &begin,
                        QMap<QString,Entity*>::iterator &end) {

                    QMutexLocker locker(&lockChildren);
                    begin = this->children.begin();
                    end = this->children.end();
                }

                inline Entity* getParent() {
                    QMutexLocker loecker(&lockParent);
                    return parent;
                }

                inline QString getParentUniqueId() {
                    QMutexLocker loecker(&lockParent);
                    return parent->getUniqueId();
                }

                inline void setDeleteChildren(bool _delete) {
                    this->deleteChildren = _delete;
                }

            signals:

            public slots:

            };
        }
    }
}

#endif // ENTITY_H
