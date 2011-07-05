#ifndef OUTLINEVIEWPLUGIN_H
#define OUTLINEVIEWPLUGIN_H

#include <QObject>

#include "NCLTreeWidget.h"
#include <core/extensions/IPlugin.h>
using namespace composer::extension;


namespace composer {
    namespace plugin {
        namespace outline {

/*!
 \brief OutlineViewPlugin is a Composer plugin that shows the internal model
            as a tree allowing ease navigation.
*/
class OutlineViewPlugin : public IPlugin
{
    Q_OBJECT

private:
    NCLTreeWidget *window; /*!< TODO */
    QString *selectedId; /*!< TODO */

public:
    /*!
     \brief Constructor.
    */
    explicit OutlineViewPlugin();
    /*!
     \brief Destructor.
    */
    ~OutlineViewPlugin();

    /*!
     \brief Returns the widget of that plugin. This widget will be presentated
                to the user.

        This function is part of the IPlugin API.

     \return QWidget* the widget that represents this plugin. If NULL, the
                plugin has not a visual representation
    */
    QWidget* getWidget();

    QMap <QString, QTreeWidgetItem*> idToItem; /*!< TODO */

    /*!
     \brief Save the specific data of this plugin.

        This function is part of the IPlugin API.

     \return bool
    */
    bool saveSubsession();
    /*!
     \brief

        This function is part of the IPlugin API.

    */
    void updateFromModel();


public slots:
    /*!
     \brief

     \param ID
     \param
    */
    void onEntityAdded(QString ID, Entity *);
    /*!
     \brief

        This function is part of the IPlugin API.

     \param ID
     \param
    */
    void onEntityChanged(QString ID, Entity *);
    /*!
     \brief

        This function is part of the IPlugin API.

     \param
    */
    /*void onEntityAboutToRemove(Entity *);*/
    /*!
     \brief

        This function is part of the IPlugin API.

     \param ID
     \param entityID
    */
    void onEntityRemoved(QString ID, QString entityID);

    /*!
     \brief

        This function is part of the IPlugin API.

     \param error
    */
    void errorMessage(QString error);

    /* Comunication from Debug to me */
    /*!
     \brief

     \param obj
    */
    void debugHasSendClearAll(void *obj);

private slots:
    /*!
     \brief

     \param id
    */
    void elementRemovedByUser(QString id);
    /*!
     \brief

     \param id
    */
    void elementAddedByUser( QString type,
                             QString parentId,
                             QMap <QString, QString> & atts,
                             bool force);
    /*!
     \brief

    */
    void itemSelectionChanged();
};

}}} //end namespace

#endif // OUTLINEVIEWPLUGIN_H
