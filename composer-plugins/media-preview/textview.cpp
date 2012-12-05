/*INF2102 Projeto Final de Programação*****************
 *Período: 2012/2
 *Coordenador: Prof. Carlos José Pereira Lucena       *
 *Projeto: Media-Preview Plugin para o Composer 3.0   *
 *Nome do aluno: Amparito Alexandra Morales Figueroa. *                                 *
 *Matrícula: 1121838                                  *
 *Orientador: Prof. Luiz Fernando Gomes Soares        *
 *Classe: Texto Preview                                 *
 *Descrição: Apresentação de objetos de mída de tipo texto**/

#include "textview.h"
#include<QFile>
#include<QTextStream>
#include<QIODevice>

textview::textview(QString filename)
{
    QFile inputFile(filename);
    QTextStream in(&inputFile);
    inputFile.open(QIODevice::ReadOnly);
    setText(in.readAll());
    setReadOnly(true);
    setStyleSheet("QTextEdit { background-color : white; color : black; }");
    setAlignment(Qt::AlignCenter);

}
