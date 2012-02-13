#include <mediapreviewplugin.h>
#include<player.h>
#include<QPushButton>
#include<VideoPlayer>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsPathItem>
#include<QtGui>
#include<MediaObject>
#include<VideoWidget>
#include<QMovie>
#include<QIcon>
#include<QPalette>
#include<QHBoxLayout>
#include<QFont>
#include<QString>
#include<wchar.h>
#include<QLine>
#include<QTextStream>
#include<QByteArray>
#include<QMessageBox>
#include <QHBoxLayout>
#include<QtWebKit/QWebView>
#include<QUrl>
#include<QtCore/QFile>
//#include<QtCore/QTextStream>
#include<QTextEdit>
#include<QIODevice>
#include<QTextCursor>



using namespace std;


MediaPreviewPlugin::MediaPreviewPlugin()
{
    /*main widget*/
    windowg =  new QWidget(0);
    windowg->setStyleSheet("* { background-color: rgb(220,220,220) }");
    windowg->adjustSize();


    /*audio*/
    window= new QWidget(0);
    window->setParent(windowg);

    window->hide();

    /*picture*/
    picture=new QLabel(0);
    picture->setParent(windowg);
    picture->setAlignment(Qt::AlignHCenter);
    picture->hide();

    /*picture1*/
    picture1=new QLabel(0);
    picture1->setParent(windowg);
    picture1->hide();
    picture1->setAlignment(Qt::AlignHCenter);

    /*video*/
    vwidget=new Phonon::VideoWidget (windowg);
    vwidget->adjustSize();
    //vwidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    vwidget->hide();





    /*webview*/
    view= new QWebView(windowg);
    view->hide();

    /*text*/

    textedit=new QTextEdit(windowg);
    textedit->setFixedSize(630,400);
    textedit->hide();

    //textedit->setFixedSize(630,500);

    /*ncl*/
    texteditncl =new QTextEdit(windowg);
    texteditncl->setFixedSize(630,400);
    texteditncl->hide();
    /*lua*/
    texteditlua =new QTextEdit(windowg);
    texteditlua->hide();
    texteditlua->setFixedSize(630,400);

    comp = "";



    videoPlayer= new Phonon::VideoPlayer(Phonon::VideoCategory,vwidget);
    button1 = new QPushButton("Play");
    button2 = new QPushButton("Pause");
    button3 = new QPushButton("Stop");
}

MediaPreviewPlugin::~MediaPreviewPlugin()
{

}


QWidget *MediaPreviewPlugin::getWidget()
{
    return windowg;
}

void MediaPreviewPlugin::changeSelectedEntity(QString plugID, void *param)
{
    QString  *entityUID = (QString*) param;

    qDebug() << "MediaPreviewPlugin::changeSelectedEntity";

    if(entityUID != NULL)
    {
        Entity* entity = getProject()->getEntityById(*entityUID);
        if(entity != NULL)
        {
            if(entity->getType() == "media")
            {
                QString filename = entity->getAttribute("src");
                QString typemidia = entity->getAttribute("type");
                if (comp != entity->getUniqueId())
                {
                    comp=entity->getUniqueId();
                    if (filename.endsWith(".webm") || filename.endsWith(".mp4") ||
                            filename.endsWith(".mpg")|| filename.endsWith(".mp2") ||
                            filename.endsWith(".mpe") || filename.endsWith(".mpg4") ||
                            filename.endsWith(".mng") || filename.endsWith(".qt") ||
                            filename.endsWith(".mov") || filename.endsWith(".avi"))
                    {

                        videoPlayer->play(Phonon::MediaSource(filename));
                        videoPlayer->setVisible(true);
                        videoPlayer->adjustSize();



                        // videoPlayer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

                        if(vwidget->isVisible())

                        {
                            videoPlayer->stop();
                            picture->hide();
                            picture1->hide();
                            window->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            videoPlayer->play(Phonon::MediaSource(filename));
                            videoPlayer->show();

                            vwidget->show();

                        }
                        else
                        {
                            vwidget->show();
                            videoPlayer->show();
                            videoPlayer->play();

                            picture->hide();
                            picture1->hide();
                            window->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();


                        }
                    }
                    else if (filename.endsWith(".mp3") || filename.endsWith(".flv") ||
                             filename.endsWith(".wav") || filename.endsWith(".ua") ||
                             filename.endsWith(".mp2") || filename.endsWith(".mp4")||
                             filename.endsWith(".mpg4"))
                    {

                        button1->setFont(QFont("Comic Sans MS",10,QFont::Bold));
                        button2->setFont(QFont("Comic Sans MS",10,QFont::Bold));
                        button3->setFont(QFont("Comic Sans MS",10,QFont::Bold));

                        //QPalette pal = window->palette();

                        //pal.setColor(window->backgroundRole(), Qt::blue);
                        //window->setPalette(pal);


                        button1->setIcon(QIcon("play.jpeg"));
                        button1->setIconSize(QSize (30,30));
                        button1->setStyleSheet("* { background-color: rgb(173,255,47) }");

                        button2->setIcon(QIcon("pause.jpeg"));
                        button2->setIconSize(QSize (30,30));
                        button2->setStyleSheet("* { background-color: rgb(173,255,47) }");

                        button3->setIcon(QIcon("stop.jpeg"));
                        button3->setIconSize(QSize (30,30));
                        button3->setStyleSheet("* { background-color: rgb(173,255,47) }");

                        QHBoxLayout *layout = new QHBoxLayout;
                        layout->addWidget(button1);
                        layout->addWidget(button2);
                        layout->addWidget(button3);

                        window->setLayout(layout);

                        player *p = new player(filename);
                        mediaobject = Phonon::createPlayer(Phonon::MusicCategory,
                                                           Phonon::MediaSource(filename));
                        QObject::connect(button1,SIGNAL(clicked()),p,SLOT(play()));
                        QObject::connect(button2,SIGNAL(clicked()),p,SLOT(pause()));
                        QObject::connect(button3,SIGNAL(clicked()),p,SLOT(stop()));



                        window->resize(400,100);
                        window->setVisible(true);

                        if(vwidget->isVisible())
                        {
                            videoPlayer->stop();
                            vwidget->hide();

                            window->show();

                            picture->hide();
                            picture1->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();



                        }
                        else
                        {
                            window->show();
                            picture->hide();
                            picture1->hide();
                            vwidget->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();



                        }
                    }
                    else if (filename.endsWith(".jpeg") || filename.endsWith(".png") ||
                             filename.endsWith(".bmp") || filename.endsWith(".jpg") ||
                             filename.endsWith(".jpe"))
                    {
                        picture->setPixmap(QPixmap(filename));
                        picture->adjustSize();
                        picture->resize(500,200);
                        picture->setAlignment(Qt::AlignHCenter);
                        picture->show();
                        picture->setVisible(true);

                        if (vwidget->isVisible())
                        {
                            videoPlayer->stop();
                            vwidget->hide();
                            picture->show();
                            picture->setVisible(true);
                            picture1->hide();
                            window->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                        }
                        else
                        {
                            picture1->hide();
                            window->hide();
                            picture->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            picture->show();

                        }
                    }
                    else if (filename.endsWith(".gif") )
                    {
                        QMovie *movie = new QMovie(filename);
                        picture1->setMovie(movie);
                        movie->start();
                        picture1->adjustSize();
                        picture1->resize(500,150);
                        picture1->setAlignment(Qt::AlignCenter);
                        picture1->show();

                        if(vwidget->isVisible())
                        {
                            videoPlayer->stop();
                            vwidget->hide();
                            window->hide();
                            picture->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            picture1->show();
                        }
                        else
                        {
                            window->hide();
                            picture->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            picture1->show();
                        }
                    }


                    else if (filename.startsWith("http") ||filename.endsWith(".html") ||
                             filename.endsWith("htm") || filename.endsWith(".xhtml"))
                    {
                        view->load(QUrl(filename));
                        view->adjustSize();
                        view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

                        view->show();

                        if(vwidget->isVisible())
                        {

                            videoPlayer->stop();
                            vwidget->hide();
                            view->show();
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();

                        }
                        else
                        {
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            view->show();
                        }

                    }

                    else if (filename.endsWith(".txt"))
                    {

                        QFile inputFile(filename);
                        QTextStream in(&inputFile);
                        inputFile.open(QIODevice::ReadOnly);
                        textedit->setText(in.readAll());
                        textedit->setReadOnly(true);
                        textedit->adjustSize();
                        textedit->setStyleSheet("QTextEdit { background-color : white; color : black; }");
                        textedit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                        if(vwidget->isVisible())
                        {

                            videoPlayer->stop();
                            vwidget->hide();
                            view->hide();
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            textedit->show();
                        }



                        else
                        {
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            view->hide();
                            textedit->show();
                        }

                    }


                    else if (filename.endsWith(".ncl") && typemidia.contains("application/x-ginga-NCL"))
                    {

                        QFile inputFile(filename);
                        QTextStream in(&inputFile);
                        inputFile.open(QIODevice::ReadOnly);
                        texteditncl->setText(in.readAll());
                        texteditncl->setReadOnly(true);
                        texteditncl->adjustSize();
                        texteditncl->setStyleSheet("QTextEdit { background-color : white; color : black; }");
                        texteditncl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                        if(vwidget->isVisible())
                        {

                            videoPlayer->stop();
                            vwidget->hide();
                            view->hide();
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            textedit->hide();
                            texteditlua->hide();
                            texteditncl->show();
                        }
                        else
                        {
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            view->hide();
                            textedit->hide();
                            texteditlua->hide();
                            texteditncl->show();
                        }

                    }

                    else if (filename.endsWith(".lua") && typemidia.contains("application/x-ginga-NCLua"))
                    {

                        QFile inputFile(filename);

                        QTextStream in(&inputFile);
                        inputFile.open(QFile::ReadOnly);
                        texteditlua->setText(in.readAll());
                        texteditlua->setReadOnly(true);
                        texteditlua->adjustSize();
                        texteditlua->setStyleSheet("QTextEdit { background-color : white; color : black; }");
                        //texteditlua->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                        if(vwidget->isVisible())
                        {

                            videoPlayer->stop();
                            vwidget->hide();
                            view->hide();
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->show();
                        }
                        else
                        {
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->show();
                        }

                    }





                }
            }
        }
    }
}
