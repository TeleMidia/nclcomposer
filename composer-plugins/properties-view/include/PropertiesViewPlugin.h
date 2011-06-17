#ifndef PROPERTIESVIEWPLUGIN_H
#define PROPERTIESVIEWPLUGIN_H

#include <QObject>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

#include "PropertyEditor.h"

namespace composer {
    namespace plugin {
        namespace property {

/*!
 \brief
*/
class PropertiesViewPlugin : public IPlugin
{
        Q_OBJECT
    private:
        QListWidget *list; /*!< TODO */
        PropertyEditor *window; /*!< TODO */
        Entity *currentEntity;

    public:
        /*!
         \brief Constructor.

        */
        explicit PropertiesViewPlugin();
        /*!
         \brief Destructor.

        */
        ~PropertiesViewPlugin();

        /*!
         \brief

         \return QWidget *
        */
        QWidget* getWidget();
        /*!
         \brief

         \return bool
        */
        bool saveSubsession();
        /*!
         \brief

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

         \param ID
         \param
        */
        void onEntityChanged(QString ID, Entity *);
        /*!
         \brief

         \param
        */
        void onEntityRemoved(QString ID, QString entityID);

        /*!
         \brief

         \param error
        */
        void errorMessage(QString error);

        /*!
         \brief

         \param
        */
        void changeSelectedEntity (void*);

    private slots:
        void updateCurrentEntityAttr(QString attr, QString value);
};

} } }//end namespace
#endif // PROPERTIESVIEWPLUGIN_H
