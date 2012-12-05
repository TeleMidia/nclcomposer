/*INF2102 Projeto Final de Programação*****************
 *Período: 2012/2
 *Coordenador: Prof. Carlos José Pereira Lucena       *
 *Projeto: Media-Preview Plugin para o Composer 3.0   *
 *Nome do aluno: Amparito Alexandra Morales Figueroa. *                                 *
 *Matrícula: 1121838                                  *
 *Orientador: Prof. Luiz Fernando Gomes Soares        *
 *Classe: Imagem Preview                              *
 *Descrição: Exibição de objetos de mídia do tipo imagem**/

#include "imagview.h"
#include<QIcon>
#include<QWidget>
#include <QLabel>

imagview::imagview(QString filename)
{

    setPixmap(QPixmap(filename));
    setVisible(true);

}
