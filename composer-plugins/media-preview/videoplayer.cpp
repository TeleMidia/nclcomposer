/*INF2102 Projeto Final de Programação*****************
 *Período: 2012/2
 *Coordenador: Prof. Carlos José Pereira Lucena       *
 *Projeto: Media-Preview Plugin para o Composer 3.0   *
 *Nome do aluno: Amparito Alexandra Morales Figueroa. *
 *Matrícula: 1121838                                  *
 *Orientador: Prof. Luiz Fernando Gomes Soares        *
 *Classe: VideoPlayer                                 *
 *Descrição: Exibição e controle de mídias de vídeo****/

#include "videoplayer.h"
#include "ui_videoplayer.h"
#include<QBoxLayout>
#include<QSizePolicy>
#include<VolumeSlider>


videoplayer::videoplayer(QString filename): ui(new Ui::videoplayer)
{

    ui->setupUi(this);
    ui->playbutton->setFont(QFont("Comic Sans MS",10,QFont::Bold));
    ui->playbutton->setIcon(QIcon("play.jpeg"));
    ui->playbutton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);

    ui->playbutton->setIconSize(QSize (30,30));
    ui->playbutton->setStyleSheet("* { background-color: rgb(173,225,47) }");

    ui->pausebutton->setFont(QFont("Comic Sans MS",10,QFont::Bold));
    ui->pausebutton->setIcon(QIcon("play.jpeg"));
    ui->pausebutton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);

    ui->pausebutton->setIconSize(QSize (30,30));
    ui->pausebutton->setStyleSheet("* { background-color: rgb(173,225,47) }");

    ui->stopbutton->setFont(QFont("Comic Sans MS",10,QFont::Bold));
    ui->stopbutton->setIcon(QIcon("play.jpeg"));
    ui->stopbutton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);

    ui->stopbutton->setIconSize(QSize (30,30));
    ui->stopbutton->setStyleSheet("* { background-color: rgb(173,225,47) }");

    mediaObject = new Phonon::MediaObject(this);
    mediaObject->setCurrentSource(Phonon::MediaSource(filename));

    Phonon::createPath(mediaObject, ui->videoDisplay);

    Audioutput =new Phonon::AudioOutput(Phonon::VideoCategory, this);
       Phonon::createPath(mediaObject, Audioutput);


     ui->volumeSlider->setAudioOutput(Audioutput);
     ui->volumeSlider->setSingleStep(1);
     ui->volumeSlider->setMuteVisible(true);
     ui->volumeSlider->setFixedSize(150,20);
     ui->volumeSlider->setFixedWidth(150);
     ui->volumeSlider->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
     Audioutput->setVolume(-0.1);


     ui->seekSlider->setMediaObject(mediaObject);
     ui->seekSlider->setSingleStep(1);

    QHBoxLayout *layoutcontrol= new QHBoxLayout();
    layoutcontrol->addWidget(ui->playbutton);
    layoutcontrol->addWidget(ui->pausebutton);
    layoutcontrol->addWidget(ui->stopbutton);
    layoutcontrol->addWidget(ui->volumeSlider);

    QHBoxLayout *layoutslider=new QHBoxLayout();
    layoutslider->addWidget(ui->seekSlider);


    QVBoxLayout *layoutvideo = new QVBoxLayout();
    layoutvideo->addWidget(ui->videoDisplay);
    layoutvideo->addLayout(layoutcontrol);
    layoutvideo->addLayout(layoutslider);

    setLayout(layoutvideo);

    ui->videoDisplay->setStyleSheet("* { background-color: rgb(0,0,0) }");
    ui->videoDisplay->setVisible(true);

    /*Slots controle de video*/

   connect(ui->videoDisplay, SIGNAL(finished()), ui->videoDisplay, SLOT(deleteLater()));
   connect(ui->playbutton, SIGNAL(clicked()),ui->videoDisplay,SLOT(on_playbutton_clicked()));
   connect(ui->pausebutton,SIGNAL(clicked()),ui->videoDisplay,SLOT(on_paused_clicked()));
   connect(ui->stopbutton,SIGNAL(clicked()),ui->videoDisplay,SLOT(on_stop_clicked()));


}


videoplayer::~videoplayer()
{
    delete ui;
}

void videoplayer::on_playbutton_clicked()
{

   mediaObject->play();
}

void videoplayer::on_pausebutton_clicked()
{
    mediaObject->pause();
}

void videoplayer::on_stopbutton_clicked()
{
    mediaObject->stop();
}
