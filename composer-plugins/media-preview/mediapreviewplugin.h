#ifndef MEDIAPREVIEWPLUGIN_H
#define MEDIAPREVIEWPLUGIN_H

#include <audioplayer.h>
#include <imagview.h>
#include <gifview.h>
#include <textview.h>
#include <hypertextview.h>
#include <nclview.h>
#include <luaview.h>
#include "videoplayer.h"
#include <composer/core/extensions/IPlugin.h>
#include<VideoPlayer>
#include<MediaObject>
#include<QPushButton>
#include<QLabel>
#include<QWidget>
#include<QMovie>
#include<QtWebKit/QWebView>
#include<QTextEdit>
#include<QMainWindow>


using namespace composer::core;

/*!
 * \brief MediaPreviewPlugin is a Composer plugin that display a different
    media player, which depends of the extension and the media type object.
    When a media object is selected (clicked) by the user in the estrutural view,
    a media player is displayed and a media pre-view of the media object is avaliable.
 */

class MediaPreviewPlugin : public IPlugin
{
    Q_OBJECT

public:

    /*!
     \brief Handles the creation of a principal Window where the media player
      is displayed.Constructor

    */
    explicit MediaPreviewPlugin();

    /*!
      \brief Destructor.
     */
    ~MediaPreviewPlugin();

    /*!
     * \brief Returns the widget of that plugin. This widget will be presentated
     *      to the user.
     *
     *  This function is part of the IPlugin API.
     *
     * \return QWidget* the widget that represents this plugin. If NULL, the
     *      plugin has not a visual representation.
     */


   QWidget *getWidget();

private:

   QString comp;
   QMainWindow *windowg;
   QWidget *windowv;
   Phonon::MediaObject *mediaobject;
   QWebView *view;
   audioplayer *musicplayer;
   imagview *imageview;
   gifview *picgifview;
   textview *txtview;
   hypertextview *hyperview;
   nclview *ncldocview;
   luaview *luaobjview;
   videoplayer *videoplay;
   QMovie *movie;



public slots:

   /*!
    * \brief Identify when a media object was selected and though entity parameter
    *  the media object attributes can be accessed. In this case the source and media
    *  type attributes was used.
    *
    * \param PluginID is the plugin instance id that first called the changes to
    *  the entity
    * \param *entity the entity that was selected by the user.
    */
    void changeSelectedEntity(QString pluginID, void *entity);

};

#endif // MEDIAPREVIEWPLUGIN_H














