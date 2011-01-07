#include "QsciNCLAPIs.h"

QsciNCLAPIs::QsciNCLAPIs(QsciLexer * 	lexer) :
        QsciAPIs(lexer)
{
    nclStructure = NCLStructure::getInstance();
    nclStructure->loadStructure();

    add (QString("ncl30"));
    prepare();
}

QsciNCLAPIs::~QsciNCLAPIs()
{

}

void QsciNCLAPIs::updateAutoCompletionList(const QStringList &context,
                                           QStringList &list) {
    suggesting = SUGGESTING_OTHER;

    int pos = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETSELECTIONSTART);
    qDebug() << "updateAutoCompletionList to";

    if ( isElement(pos) ){
        suggesting = SUGGESTING_ELEMENTS;
        qDebug() << "Must suggest elements" << endl;
        for (int i = 0; i < context.count(); ++i) {
            qDebug() << "'" << context[i] << "'";
            QString father = getFatherTagName(pos);
            qDebug() << "Father = " << father;
            if(father == "")
                father = "NULL";

            map <QString, char> *elements = nclStructure->getChildren(father);
            map <QString, char>::iterator it;

            if(elements != NULL) {
                it = elements->begin();
                for (; it != elements->end(); ++it){
                    if(it->first.startsWith(context[i])){
                        QString str(it->first);
                        list.push_back(str);
                        qDebug() << it->first;
                    }
                }
            }
        }
    }
    else if ( isAttribute(pos) ) {
        suggesting = SUGGESTING_ATTRIBUTES;
        QString tagname = getCurrentTagName(pos);
        QStringList current_attrs;

        getAttributesTyped(pos, current_attrs);
        for (int i = 0; i < current_attrs.size(); i++)
               qDebug() << "Already typed: " << current_attrs.at(i);

        qDebug() << "Must suggest attributes to " << tagname;
        if(tagname != ""){
            map <QString, bool> *attrs = nclStructure->getAttributes(tagname);
            if(attrs != NULL) {
                map <QString, bool>::iterator it;
                for (int i = 0; i < context.count(); ++i) {
                    it = attrs->begin();

                    for (; it != attrs->end(); ++it){
                        if(it->first.startsWith(context[i]) && !current_attrs.contains(it->first)){
                            QString str(it->first);
                            list.push_back(str);
                            qDebug() << it->first;
                        }
                    }
                }
            }
        }
    }
    else if ( isAttributeValue(pos) ){
        suggesting = SUGGESTING_ATTRIBUTE_VALUES;
        QString tagname = getCurrentTagName(pos);
        QString attribute = getCurrentAttribute(pos);

        vector <NCLReference *> references = nclStructure->getReferences(tagname, attribute);

        for(unsigned int i = 0; i < references.size(); i++)
            qDebug() << "Should refer to " << references[i]->getRefElement() << "." << references[i]->getRefAttribute();
    }
}

//TODO: END ELEMENT, ATTRIBUTE VALUE
void QsciNCLAPIs::autoCompletionSelected(const QString &selection) {
    int start = 0, line, pos;
    QString outputStr("");
    QString strline;
    bool fixidentation = false;

    //PREPARING TO INSERT TEXT
    lexer()->editor()->beginUndoAction();
    //cancelling autocomplete (avoid scintilla insert the selection)
    lexer()->editor()->SendScintilla(QsciScintilla::SCI_AUTOCCANCEL);
    lexer()->editor()->getCursorPosition(&line, &pos);
    //delete original word from text
    start = pos - 1;
    strline = lexer()->editor()->text(line);

    //if the user already put a word, delete this word (autocomplete will put it entirely
    if(start >= 0 && strline.at(start).isLetter()) {
        lexer()->editor()->SendScintilla(QsciScintilla::SCI_DELWORDLEFT);

        //update the start position and the content line
        lexer()->editor()->getCursorPosition(&line, &pos);
        strline = lexer()->editor()->text(line);
        start = pos-1;
    }

    if(suggesting == SUGGESTING_ELEMENTS) {
        QString attributes = getRequiredAttributesAsStr(selection);

        /*if(start >= 0 && strline.at(start).isLetter())
            lexer()->editor()->SendScintilla(QsciScintilla::SCI_DELWORDLEFT);*/

        if(start < 0 || strline.at(start) != '<')
            outputStr += "<";
        outputStr += selection + " " + attributes;

        map <QString, char> *children = nclStructure->getChildren(selection);
        if ( children != NULL && children->size() ) {
            outputStr += ">";
            outputStr += "\n";
            outputStr += "</";
            outputStr += selection;
            outputStr += ">";
            fixidentation = true;
        }
        else
            outputStr += "/>";

    } else if(suggesting == SUGGESTING_ATTRIBUTES){
        outputStr = selection + "=\"\"";

        //if the attribute is just after another word, put a space between then
        if(start >= 0){
            QChar ch = strline.at(start);
            qDebug() << ch;
            if(!ch.isSpace())
                outputStr.prepend(' ');
        }
    }

    //insert the new word (already managed)
    lexer()->editor()->insert(outputStr);
    //fix identation
    if(fixidentation){
        int lineident = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETLINEINDENTATION, line);
        lexer()->editor()->SendScintilla(QsciScintilla::SCI_SETLINEINDENTATION, line+1, lineident);
    }

    //move cursor to the new position
    lexer()->editor()->SendScintilla(QsciScintilla::SCI_WORDRIGHT);
    lexer()->editor()->SendScintilla(QsciScintilla::SCI_WORDRIGHT);

    lexer()->editor()->endUndoAction();
}


QStringList QsciNCLAPIs::callTips(const QStringList &context, int commas,
                                  QsciScintilla::CallTipsStyle style,
                                  QList<int> &shifts){

    (void) context; (void) commas; (void) style; (void) shifts;

    QStringList list;
    QString str("TODO");
    list.push_back(str);

    return list;
}

bool QsciNCLAPIs::event(QEvent *e){
    (void) e;
    //qDebug() << "QsciNCLAPIs::event" << e;
    return true;
}

QString QsciNCLAPIs::getRequiredAttributesAsStr(const QString &element){
    QString ret("");
    map <QString, bool> *attributes = NCLStructure::getInstance()->getAttributes(element);
    if(attributes != NULL) {
        map <QString, bool>::iterator it;
        bool first = true;
        for(it = attributes->begin(); it != attributes->end(); ++it){
            if(it->second) {
                if(!first)
                    ret += " ";
                first = false;
                ret += it->first + "=\"\"";
            }
        }
    }
    return ret;
}

bool QsciNCLAPIs::isElement(int pos){
    int style = lexer()->editor()->SendScintilla(QsciScintilla:: SCI_GETSTYLEAT, pos);
    qDebug() << "Style=" << style <<endl;
    if(style == QsciLexerNCL::Default){
        return true;
    } else if( style == QsciLexerNCL::Tag ||
               style == QsciLexerNCL::XMLTagEnd ||
               style == QsciLexerNCL::XMLStart ||
               style == QsciLexerNCL::OtherInTag ||
               style == QsciLexerNCL::UnknownTag){
        int p = pos-1;
        while(p >= 0){
            char ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            if(isspace(ch))
                break;
            if(ch == '<') return true;
            p--;
        }
    }
    return false;
}

//TODO: returning false when just before >
bool QsciNCLAPIs::isAttribute(int pos){
    qDebug() << "QsciNCLAPIs::isAttribute";
    int style = lexer()->editor()->SendScintilla(QsciScintilla:: SCI_GETSTYLEAT, pos);
    if ( style == QsciLexerNCL::Attribute )
        return true;
    else if( style == QsciLexerNCL::Tag ||
             style == QsciLexerNCL::XMLTagEnd ||
             style == QsciLexerNCL::XMLStart ||
             style == QsciLexerNCL::OtherInTag ||
             style == QsciLexerNCL::UnknownTag){
        int p = pos-1;
        while(p >= 0){
            char ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            if(isspace(ch)) return true;
            if(ch == '<') return true;

            p--;
        }
    }
    return false;
}

QString QsciNCLAPIs::getCurrentTagName(int pos){
    int p = pos;
    char ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);

    if(isElement(pos)){
        while (p >= 0 && ch != '<'){
            p--;
            ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
        }

        if(p >= 0){
            p++;
            ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            QString word("");
            while(isalnum(ch)){
                word += ch;
                p++;
                ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            }
            return word;
        }
    }
    else {
        while(p >= 0){
            QString word("");
            ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            while(p >= 0 && isalnum(ch)){
                word.prepend(ch);
                p--;
                ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            }
            if(ch == '<') return word;
            p--;
        }

    }
    return QString("");
}

QString QsciNCLAPIs::getCurrentAttribute (int pos)
{
    int p = pos;
    char ch;
    QString current_attribute = "";
    if (isAttributeValue(pos)) {
        bool quote_finded = false, equal_finded = false;
        //find quote
        while(p >= 0 && !quote_finded){
            ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            if(ch == '\'' || ch == '\"')
                quote_finded = true;
            p--;
        }
        //find equal
        while(p >= 0 && !equal_finded){
            ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            if(ch == '=')
                equal_finded = true;
            p--;
        }
        //remove any whitespace between = and attribute name
        while (p >= 0) {
            ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            if(!isspace(ch))
                break;
            p--;
        }
        //get the attribute name
        while(p >= 0){
            ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            if(!isalnum(ch))
                break;
            current_attribute.prepend(ch);
            p--;
        }

        current_attribute = current_attribute.trimmed();
    }

    return current_attribute;
}

//TODO: Eliminate closed tags in the way
QString QsciNCLAPIs::getFatherTagName(int pos) {
    int p = pos;
    char ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);

    int reading = -1;
    int closed_tags = 0;
    while (p >= 0)
    {
        p--;
        ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
        if(ch == '>') {
            reading = QsciLexerNCL::XMLStart;
            p--;
            if (p < 0) break;
            ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
            if ( ch == '/')
                reading = QsciLexerNCL::XMLTagEnd;
            else if (ch == '-')
                reading = QsciLexerNCL::HTMLComment;
            else {
                char lastch = ch;
                while (p >= 0 && ch != '<'){
                    p--;
                    lastch = ch;
                    ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
                }
                if(lastch == '/') reading = QsciLexerNCL::XMLEnd;

                if(reading == QsciLexerNCL::XMLEnd)
                    closed_tags ++;
                else if(reading == QsciLexerNCL::XMLStart)
                    closed_tags --;

                qDebug() << "closed_tags = " << closed_tags << " p=" << p;
                if(closed_tags < 0)
                    return getCurrentTagName( p+2 );
            }
        }
    }

    return "";
}

//FIXME: By now, it will consider any quoted string as attribute value, but
// this is not true.
bool QsciNCLAPIs::isAttributeValue(int pos){
    qDebug() << "QsciNCLAPIs::isAttributeValue";
    int style = lexer()->editor()->SendScintilla(QsciScintilla:: SCI_GETSTYLEAT, pos);
    if ( style == QsciLexerNCL::HTMLDoubleQuotedString || style == QsciLexerNCL::HTMLSingleQuotedString)
        return true;

    return false;
}

//FIXME: be sure we are in a start tag
int QsciNCLAPIs::getStartTagBegin(int pos){
    int p = pos;
    char ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
    while (p >= 0 && ch != '<'){
        p--;
        ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
    }
    return p;
}

//FIXME: Be sure that we are in a start tag
int QsciNCLAPIs::getStartTagLength(int pos){
    int start = getStartTagBegin(pos);
    int p = pos;
    int text_length = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETLENGTH);
    char ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);

    while (p < text_length && ch != '>'){
        p++;
        ch = lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETCHARAT, p);
    }
    return p >= text_length ? -1 : (p-start+1);
}

void QsciNCLAPIs::getAttributesTyped(int pos, QStringList &attrs) {
    int start = getStartTagBegin(pos);
    int length = getStartTagLength(pos);
    int end = start + length;

    qDebug() << "start=" << start << " end=" << end;

    if(start < 0 || end < 0) return;

    QString text;
    char *chars = (char *) malloc ((end - start) * sizeof(char) + 1);
    lexer()->editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, chars);
    text = QString(chars);
    qDebug() << "text = " << text;

    //FIXME: The following regex is not completely correct. Te text inside the attribute
    // will be matched in some cases.
    QRegExp attrRegex ("\\s[a-zA-Z]+");

    int lastIndex = 0;
    int index = attrRegex.indexIn(text, lastIndex);
    while(index != -1){
        if (index + attrRegex.matchedLength() > end)
            break;
        qDebug() << index << " " << attrRegex.matchedLength();
        attrs << text.mid(index, attrRegex.matchedLength()).trimmed();

        lastIndex = (index + attrRegex.matchedLength());
        index = attrRegex.indexIn(chars, lastIndex);
    }

    delete chars;
}
