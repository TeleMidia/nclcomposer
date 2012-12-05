/*INF2102 Projeto Final de Programação*****************
 *Período: 2012/2
 *Coordenador: Prof. Carlos José Pereira Lucena       *
 *Projeto: Media-Preview Plugin para o Composer 3.0   *
 *Nome do aluno: Amparito Alexandra Morales Figueroa. *                                 *
 *Matrícula: 1121838                                  *
 *Orientador: Prof. Luiz Fernando Gomes Soares        *
 *Descrição: Plugin que facilita a visualização       *
 *e exibição dos objetos de mídia que estão sendo     *
 *utilizados num projeto desenvolvido utilizando a    *
 *ferramenta Composer 3.0******************************/

#include <mediapreviewplugin.h>
#include<audioplayer.h>
#include<gifview.h>
#include<hypertextview.h>
#include<nclview.h>
#include "videoplayer.h"
#include<QMovie>
#include<QIcon>
#include<QPalette>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFont>
#include<QString>
#include<QTextStream>
#include<QIODevice>
#include<QMainWindow>

using namespace std;

MediaPreviewPlugin::MediaPreviewPlugin()
{
    /*Main Widget*/
    windowg =  new QMainWindow(0);
    windowg->setStyleSheet("* { background-color: rgb(220,220,220) }");
    comp = "";

}

MediaPreviewPlugin::~MediaPreviewPlugin()
{

}


QWidget *MediaPreviewPlugin::getWidget()
{
    return windowg;
}
/* Identifica se a entidade selecionada é do tipo mídia, então armazenada
em variaveis do tipo String os atributos que vão permitir a clasificação do
do tipo de mídia, estes são: source("src") e type("type")*/

void MediaPreviewPlugin::changeSelectedEntity(QString plugID, void *param)
{
    QString  *entityUID = (QString*) param;

    if(entityUID != NULL)
    {
        Entity* entity = getProject()->getEntityById(*entityUID);
        if(entity != NULL)
        {
            if(entity->getType() == "media")
            {
                QString attrSrc = entity->getAttribute("src");
                QString attrType = entity->getAttribute("type");
                qDebug() <<"enderço do arquivo";
                qDebug()<< attrSrc;
                if (comp != entity->getUniqueId())
                {
                    comp=entity->getUniqueId();
                    QString type;


                    /* Manejo das extensões dos objetos de mídia. Compara
                      se o valor contido na variavél attrSrc corresponde a um objeto
                      de mída do tipo vídeo, audio, imagem, hypertexto, ncl or lua.*/


                    /*Objeto de mídia do tipo VIDEO*/
                    if(attrSrc.endsWith(".mp4") ||
                       attrSrc.endsWith(".mpg")|| attrSrc.endsWith(".mp2") ||
                       attrSrc.endsWith(".mpe") || attrSrc.endsWith(".mpg4") ||
                       attrSrc.endsWith(".mng") || attrSrc.endsWith(".qt") ||
                       attrSrc.endsWith(".mov") || attrSrc.endsWith(".avi") )
                    {
                      type = "video";
                    }

                    /*Objeto de mídia do tipo AUDIO*/
                    else if(attrSrc.endsWith(".mp3") || attrSrc.endsWith(".flv") ||
                            attrSrc.endsWith(".wav") || attrSrc.endsWith(".ua") ||
                            attrSrc.endsWith(".mp2") || attrSrc.endsWith(".mp4")||
                            attrSrc.endsWith(".mpg4"))
                    {
                      type = "audio";

                    }

                    /*Objeto de mídia do tipo IMAGEM*/
                   else if(attrSrc.endsWith(".jpeg") || attrSrc.endsWith(".png") ||
                            attrSrc.endsWith(".bmp") || attrSrc.endsWith(".jpg") ||
                            attrSrc.endsWith(".jpe"))
                    {
                      type = "image";
                    }

                    /*Objeto de mídia do tipo GIF*/
                   else if (attrSrc.endsWith((".gif")))
                    {
                        type ="gif";
                    }

                    /*Objeto de mídia do tipo HYPERTEXT*/
                   else if (attrSrc.startsWith ("http") ||attrSrc.endsWith(".html") ||
                             attrSrc.endsWith(".htm") || attrSrc.endsWith(".xhtml"))
                    {
                        type = "hypertext";
                    }

                    /*Objeto de mídia do tipo TEXTO*/
                    else if (attrSrc.endsWith(".txt"))
                    {
                        type ="text";
                    }

                     /*Objeto de mídia do tipo NCL*/
                    else if (attrSrc.endsWith(".ncl"))
                    {
                        type="ncl";
                    }

                     /*Objeto de mídia do tipo LUA*/
                    else if (attrSrc.endsWith(".lua") )
                    {
                        type="lua";
                    }

                    /*Objeto de mídia do tipo XML*/

                   else if(attrSrc.endsWith(".xml"))
                    {
                        type ="xml";
                    }

                    else if(attrSrc.endsWith(".css"))
                    {
                        type="css";
                    }

                     /*Objeto de mídia do tipo XLT*/

                    else if (attrSrc.endsWith(".xlt")|| attrSrc.endsWith(".xlet")|| attrSrc.endsWith(".class"))
                    {
                        type="let";
                    }

                    /* Manejo do atributo type,attrType. (overwrite extension)*/

                 if(attrType.contains("video/"))
                    {
                      type = "video";
                    }
                   else  if(attrType.contains("audio/"))
                    {
                      type = "audio";

                    }

                    else if(attrType.contains("image/gif") )
                    {
                        type="gif";

                    }

                    else if (attrType.contains("image/bmp")||attrType.contains("image/png")|| attrType.contains("image/jpg")
                             ||attrType.contains("image/jpeg")||attrType.contains("image/jpe"))
                    {
                        type="image";
                    }

                    else if(attrType.contains("text/xml"))
                    {
                        type="Markup language";
                    }
                    else if (attrType.contains("text/html"))
                    {
                        type ="hypertext";
                    }

                    else if(attrType.contains("text/css"))
                    {
                        type="css";
                    }

                    else if(attrType.contains("text/plain"))
                    {
                        type="text";
                    }

                   else if(attrType.contains("application/x-ginga-ncl")||attrType.contains("application/x-ncl-NCL"))
                    {
                        type="ncl";
                    }

                   else if (attrType.contains("application/x-ginga-NCLua")|| attrType.contains("application/x-ncl-NCLua"))
                    {
                        type="lua";

                     }

                    else if(attrType.contains("application/x-ginga-NCLet"))
                    {
                        type="let";
                    }

                 /*Interface para exibir o objeto de mídia, dependendo da extensão y/ou tipo
                   da mída. Uma clase diferente para cada tipo de mídia foi criada. */

                    /*audio*/
                  if (type =="audio")
                  {

                             musicplayer = new audioplayer(attrSrc);
                             musicplayer->setParent(windowg);
                             windowg->setCentralWidget(musicplayer);

                  }


                  /*video*/
                  if(type=="video")
                  {
                      videoplay = new videoplayer(attrSrc);
                      videoplay->setParent(windowg);
                      windowg->setCentralWidget(videoplay);

                  }


                /*image*/
                  if(type =="image")
                  {
                      imageview= new imagview(attrSrc);
                      imageview->setParent(windowg);
                      windowg->setCentralWidget(imageview);
                  }

                  /*gif*/

                  if(type=="gif")
                  {
                      picgifview = new gifview(attrSrc);
                      picgifview->setParent(windowg);
                      windowg->setCentralWidget(picgifview);
                  }

                  /*text*/
                  if(type=="text")
                  {
                      txtview =new textview(attrSrc);
                      txtview->setParent(windowg);
                      windowg->setCentralWidget(txtview);
                  }

                  /*hypertext*/
                  if(type=="hypertext")
                  {
                      hyperview = new hypertextview(attrSrc);
                      hyperview->setParent(windowg);
                      windowg->setCentralWidget(hyperview);

                  }

                  /*XML*/

                  if(type=="xml")
                  {

                  }

                 /*NCL DOCUMENTS */
                  if(type=="ncl")
                  {
                      ncldocview =new nclview(attrSrc);
                      ncldocview->setParent(windowg);
                      windowg->setCentralWidget(ncldocview);
                  }

                    /*LUA OBJECTS*/

                  if(type=="lua")
                  {
                      luaobjview = new luaview(attrSrc);
                      luaobjview->setParent(windowg);
                      windowg->setCentralWidget(luaobjview);

                  }

            }
        }
            }
        }
    }






