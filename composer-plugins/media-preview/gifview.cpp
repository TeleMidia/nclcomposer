/*INF2102 Projeto Final de Programação*****************
 *Período: 2012/2
 *Coordenador: Prof. Carlos José Pereira Lucena       *
 *Projeto: Media-Preview Plugin para o Composer 3.0   *
 *Nome do aluno: Amparito Alexandra Morales Figueroa. *                                 *
 *Matrícula: 1121838                                  *
 *Orientador: Prof. Luiz Fernando Gomes Soares        *
 *Classe: Gif Preview                                 *
 *Descrição: Exibição de imagens do tipo GIF***********/

#include "gifview.h"
#include <QLabel>
#include<QMovie>

gifview::gifview(QString filename)
{
    QMovie *movie = new QMovie(filename);
    setMovie(movie);
    movie->start();

}
