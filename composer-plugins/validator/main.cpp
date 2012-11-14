#include "nclmodel/nclmodel.h"
#include "langstruct/langstruct.h"
#include "definitions/definitions.h"
#include "validation/Validator.h"

#include <vector>

#include <QDebug>
#include <QString>
#include <QPair>
#include <QDomDocument>
#include <QStack>
#include <QDateTime>

using namespace std;
using namespace nclValidator;

void validate(QString document, QString language)
{
    QTextStream cout(stdout, QIODevice::WriteOnly);

    cout << "--" << endl;
    cout << "--" << " " << "nclvs - NCL Validation Service" << endl;
    cout << "--" << " " << "Version: 1.0.0" << endl;
    cout << "--" << endl;
    cout << "--" << " " << "File: '" << document << "'" << endl;
    cout << "--" << " " << "Date: " << QDateTime::currentDateTime().toString() << endl;
    cout << "--" << endl;
    cout << endl;

    QFile* file = new QFile(document);

    int erros = 0;

    if (file->open(QIODevice::ReadOnly)){
        QDomDocument* d = new QDomDocument();
        d->setContent(file);

        Model m;

        QStack<QPair<QString, QDomElement> > s;

        s.push(QPair<QString, QDomElement>("",d->firstChildElement()));

        while(!s.isEmpty()){
            QPair<QString, QDomElement> qp = s.pop();

            virtualId uid = qp.first.toStdString();

            QDomElement e = qp.second; string name = e.nodeName().toStdString();


            QDomNamedNodeMap a = e.attributes(); vector<Attribute> atts;
            for (int i=0; i< a.length(); i++){

                atts.push_back(Attribute(a.item(i).nodeName().toStdString(),
                                         a.item(i).nodeValue().toStdString()));
            }

            if (uid != ""){
                uid = m.addChild(uid, name, atts);
            }else{
                uid = m.addElement(name, atts);
            }

            m.element(uid)->setData((void*)  new pair<int, int>(e.lineNumber(), e.columnNumber()));

            QDomNodeList list = e.childNodes();
            for ( int i=list.length()-1;i>=0;--i){

                if (list.item(i).isElement()){
                    QDomElement element = list.item(i).toElement();

                    s.push(QPair<QString, QDomElement>(QString(uid.c_str()), element));
                }
            }
        }

        Langstruct::init();

        vector<pair<void*, string> > msg = Validator::validate(m, language.toStdString());

        erros = msg.size();

        for (int i = 0; i < erros; i++){
            pair<int, int> l = *((pair<int, int>*) msg.at(i).first);
            string txt = msg.at(i).second;

            cout << "[E]"
                 << " : "
                 << l.first
                 << " : "
                 << l.second
                 << " : "
                 << QString(txt.c_str())
                 << endl;
        }

        delete d;
    }

    if (erros > 0) cout << endl;
    cout << erros << " erros found." << endl;
    cout << endl;

    delete file;
}

int main(int argc, char *argv[])
{
    QTextStream cout(stdout, QIODevice::WriteOnly);

    if (argc > 1){
        int fi = 1;
        QString lang = "en";

        if (QString(argv[1]) == "-l"){
            if (QString(argv[2]) != "en" ||
                QString(argv[2]) != "es" ||
                QString(argv[2]) != "pt_BR"){

                lang = QString(argv[2]);
                fi = 3;
            }else{
                cout << "nclvs: invalid language value." << endl;
                cout << "Usage: nclvs [-l language] file ..." << endl;
                cout << endl;
                cout << "Avaiable language:" << endl;
                cout << "    en: English." << endl;
                cout << "    es: Spanish." << endl;
                cout << "    pt_BR: Portuguese (Brazil)." << endl;
            }
        }

        for (int i = fi; i < argc; i++){
            validate(QString(argv[i]), lang);
        }
    }else{
        cout << "nclvs: missing operand." << endl;
        cout << "Usage: nclvs [-l language] file ..." << endl;
        cout << endl;
        cout << "Avaiable language:" << endl;
        cout << "    en: English." << endl;
        cout << "    es: Spanish." << endl;
        cout << "    pt_BR: Portuguese (Brazil)." << endl;
    }

//    Langstruct::init();

//    vector<Attribute> atts; atts.push_back(Attribute("id", "eduardo"));

//    Model m;

//    virtualId rID = m.addElement("ncls",atts);


//    vector<pair<void *, string> > msg = Validator::validate(m, "en");

//    pair<void *, string> p = msg.at(0);

//    qDebug() << QString(p.second.c_str());

    return 0;
}
