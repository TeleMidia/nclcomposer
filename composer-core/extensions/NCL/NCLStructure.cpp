#include "NCLStructure.h"

NCLStructure *NCLStructure::instance = NULL;

NCLStructure::NCLStructure() {
    attributes = new map <QString, map <QString, bool > * > ();
    nesting = new map <QString, map <QString, char > * > ();
    dataType = new map <QString, QString> ();
    references = new QMultiMap <QString, AttributeReferences*> ();
}

NCLStructure *NCLStructure::getInstance(){
    if(instance == NULL){
        instance = new NCLStructure();
    }
    return instance;
}

void NCLStructure::loadStructure(){
    QFile fInput ( NCLSTRUCTURE_FILE );
    if(!fInput.open(QIODevice::ReadOnly)){
        qErrnoWarning("Should not open the NCL_STRUCTURE file.");
        return;
    }

    QTextStream in (&fInput);
    while(!in.atEnd()){
        QString line = in.readLine();
        vector <QString> tokens = parseLine(line);

        //this is a commentary
        if(tokens.size() == 0) {
            continue;
        }
        if(tokens[0] == "datatype"){
            qDebug() << "I'm reading a new DATATYPE element - This is not supported yet." << endl;

        } else if(tokens[0] == "element"){
//            qDebug() << "I'm reading a new ELEMENT element";
//            qDebug() << "Adding Element -- " << tokens[1];

            if(tokens.size() == 5 ){
                addElement(tokens[1], tokens[2], 0);
            }
            else {
                qErrnoWarning("element primitive must be exactly 3 arguments (ELEMENT NAME, ELEMENT FATHER, CARDINALITY)");
            }

        } else if(tokens[0] == "attribute"){
//            qDebug() << "I'm reading a new ATTRIBUTE element" << endl;

            if(tokens.size() == 5) {
                bool required = false;
                if(tokens[3].toLower() == "true")
                    required = true;

                addAttribute(tokens[1], tokens[2], tokens[4], required);
            } else {
                qErrnoWarning("attribute primitive must be exactly 4 arguments \
                              (ELEMENT NAME, ATTR NAME, ATTR TYPE, REQUIRED)");
            }

        } else if(tokens[0] == "scope"){
            qDebug() << "I'm reading a new SCOPE element - This is not supported yet" << endl;

        } else if(tokens[0] == "reference"){
//            qDebug() << "I'm reading a new REFERENCE element" << endl;
            addReference(tokens[1], tokens[2], tokens[3], tokens[4]);

        }
        //qDebug() << line << endl;
    }
    fInput.close();
}

vector <QString> NCLStructure::parseLine(QString line){
    vector <QString> ret;
    QChar ch;
    int size = line.size(), i = 0;
    QString token;
    bool reading_attributes = false;

    while (i < line.size()) {
        ch = line.at(i);

        if (ch == '/') {
            if (i+1 < size && line[i+1] == '/') {
                //comment was found, it will ignore the remaining caracteres in the line
                token = "//";
                break;
            }
        }
        else if(ch == '(') {
            if(token != "")
                ret.push_back(token);
            token = "";
            reading_attributes = true;
        }
        else if(ch == ',') {
            if(reading_attributes && token != "")
                ret.push_back(token);
            token = "";
        }
        else if (ch == ')') {
            if(reading_attributes && token != "")
                ret.push_back(token);
            reading_attributes = false;
            token = "";
        }
        else {
            if(!ch.isSpace())
                token.append(line.at(i));
        }
        i++;
    }
    return ret;
}

//TODO: SCOPE
void NCLStructure::addElement(QString name, QString father, char cardinality){
    if(!nesting->count(father))
        (*nesting)[father] = new map <QString, char>();
    if(!attributes->count(name))
        (*attributes)[name] = new map <QString, bool>();

    (*(*nesting)[father])[name] = cardinality;
}

void NCLStructure::addAttribute(QString element, QString attr, QString type, bool required){
    if(!attributes->count(element))
        (*attributes)[element] = new map <QString, bool>();

    qDebug() << "NCLStructure::addAttribute (" << element << ", " << attr << ")";
    (*(*attributes)[element])[attr] = required;
}

void NCLStructure::addReference(QString element, QString attr, QString ref_element, QString ref_attr)
{
    AttributeReferences *ref = new AttributeReferences (element, attr, ref_element, ref_attr);
    references->insert(element, ref);
}

map <QString, bool> *NCLStructure::getAttributes(QString element){
    if(attributes->count(element))
        return (*attributes)[element];
    return NULL;
}

map <QString, map <QString, char> *> *NCLStructure::getNesting(){
    return this->nesting;
}

map <QString, char> * NCLStructure::getChildren (QString tagname) {
    if(nesting->count(tagname))
        return (*nesting)[tagname];
    return NULL;
}

vector <AttributeReferences*> NCLStructure::getReferences(QString element, QString attr){
    vector <AttributeReferences *> ref;
    foreach( AttributeReferences *value, references->values(element) ){
        if(value->getAttribute() == attr)
            ref.push_back(value);
    }

    return ref;
}

/************ NCL REFERENCE **********************/
AttributeReferences::AttributeReferences ( QString element, QString attr,
                             QString ref_element, QString ref_attr)
{
    this->element = element;
    this->attr = attr;
    this->ref_element = ref_element;
    this->ref_attr = ref_attr;
}

QString AttributeReferences::getElement()
{
    return this->element;
}

QString AttributeReferences::getAttribute()
{
    return this->attr;
}

QString AttributeReferences::getRefElement()
{
    return this->ref_element;
}

QString AttributeReferences::getRefAttribute()
{
    return this->ref_attr;
}
