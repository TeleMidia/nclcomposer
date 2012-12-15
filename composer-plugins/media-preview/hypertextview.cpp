/*INF2102 Projeto Final de Programação*****************
 *Período: 2012/2
 *Coordenador: Prof. Carlos José Pereira Lucena       *
 *Projeto: Media-Preview Plugin para o Composer 3.0   *
 *Nome do aluno: Amparito Alexandra Morales Figueroa. *                                 *
 *Matrícula: 1121838                                  *
 *Orientador: Prof. Luiz Fernando Gomes Soares        *
 *Classe: Hypertexto Preview                          *
 *Descrição: Exibição de objetos de tipo hypertext
 *(http, html, htm, xhtml *****************************/

#include "hypertextview.h"
#include <QtWebKit/QWebView>
#include <QUrl>

hypertextview::hypertextview(QString filename)
{
    load(QUrl(filename));
    adjustSize();



}

hypertextview::~hypertextview()
{

}
