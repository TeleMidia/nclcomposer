#include "NCLTextEditor.h"

NCLTextEditor::NCLTextEditor(QWidget *parent) :
        QsciScintilla(parent)
{
    shortcut_ctrl_space = new QShortcut(QKeySequence("Ctrl+Space"), this);
    shortcut_zoomout = new QShortcut(QKeySequence::ZoomOut, this);  /* CTRL- */
    shortcut_zoomin = new QShortcut(QKeySequence::ZoomIn, this);  /* CTRL+ */

    initParameters();
}

NCLTextEditor::~NCLTextEditor(){
    delete apis;
    delete shortcut_ctrl_space;
    delete shortcut_zoomout;
    delete shortcut_zoomin;
}

void NCLTextEditor::initParameters(){
    setAutoIndent(true);
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setFoldMarginColors(PREF_FOLD_MARGIN_FORE_COLOR, PREF_FOLD_MARGIN_BACK_COLOR);
    setMarginLineNumbers(1, true);
    setMarginWidth(1, 25);
    setMarginsBackgroundColor(QColor("#dddddd"));

    setCaretWidth(20);
    setCaretLineBackgroundColor(QColor("#e6fff0"));
    setCaretLineVisible(true);

    ensureLineVisible(1);
    setUtf8(true);
    setWrapMode(WrapWord);
    setWrapVisualFlags(WrapFlagByText);

    /* AutoCompletetion parameters */
    setAutoCompletionThreshold(-1);
    setAutoCompletionFillupsEnabled(false);
    setAutoIndent(true);


    //adding text partitions
    /*MyLexer *mylexer = new MyLexer(this);

    QRegExp attrValueRegex ("\".*\"");
    QsciStyle attrValueStyle (4, "ATTRIBUTE_VALUE", QColor(0x0, 0x0, 0xFF),
                              mylexer->defaultPaper(1), mylexer->defaultFont(1));
    mylexer->addTextPartition(4, attrValueRegex, attrValueStyle);

    QRegExp startTagRegex ("<.[^><.]+>");
    QsciStyle startTagStyle (10, "START_TAG",  QColor(0x0, 0x0, 0xFF),
                             mylexer->defaultPaper(1), mylexer->defaultFont(1));
    mylexer->addTextPartition(10, startTagRegex, startTagStyle);
    */
    setLexer(new QsciLexerNCL ());

    //APIS
    apis = new QsciNCLAPIs(lexer());

    QFont font;
    font.setFamily(QString::fromUtf8(PREF_FONT_FAMILY));
    font.setPointSize(PREF_FONT_SIZE);
    setFont(font);
    setCaretLineBackgroundColor(QColor(PREF_CARET_LINE_BG_COLOR));

    /* Ctrl + Space == Autocomplete */
    connect(shortcut_ctrl_space, SIGNAL(activated()), this, SLOT(autoCompleteFromAPIs()));

    /* Zoomin e Zoomout == Ctrl + -  && Ctrl + + */
    connect(shortcut_zoomout, SIGNAL(activated()), this, SLOT(Decreasefont()));
    connect(shortcut_zoomin, SIGNAL(activated()), this, SLOT(Increasefont()));

    //connect(this, SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)), this, SLOT(MarkLine(int,int,Qt::KeyboardModifiers)));

    error_marker = markerDefine(QPixmap(":/images/error-icon-16.png"), -1);
    error_indicator = indicatorDefine(SquiggleIndicator, -1);
    setIndicatorForegroundColor(QColor("#FF0000"), error_indicator);
    filling_attribute_indicator = indicatorDefine (RoundBoxIndicator, -1);
}

void NCLTextEditor::Increasefont(){
    zoomIn();
}

void NCLTextEditor::Decreasefont(){
    zoomOut();
}

void NCLTextEditor::markError(QString description, QString file, int line, int column, int severity){
    //TODO: Show Error Messages
    qDebug() << "MarkError line=" << line;
    QString tmp = this->text(line-1);

    //ADD MARKER
    markerDeleteAll(error_marker);

    if (text() != "")
        markerAdd(line-1, error_marker);

    //ADD INDICATOR
    int nr_lines = lines();
    QString tmp2 = this->text(nr_lines);


    int indentation = 0;
    while (tmp[indentation].isSpace())
        indentation++;
    clearIndicatorRange(0, 0, nr_lines, tmp2.size(), error_indicator);
    fillIndicatorRange(line-1, indentation, line-1, tmp.size()-1, error_indicator);

}

void NCLTextEditor::wheelEvent (QWheelEvent *event){
    if(event->modifiers() == Qt::ControlModifier){
        if(event->delta() > 0)
            zoomIn();
        else
            zoomOut();
    }
    QsciScintilla::wheelEvent(event);
}

void NCLTextEditor::keyPressEvent(QKeyEvent *event) {
    int begin, end;
    if(state == FILLING_ATTRIBUTES_STATE) {
        int line, index;
        getCursorPosition(&line, &index);

        QString strline = text(line);
        if(event->key() == Qt::Key_Return){
            clearIndicatorRange(line, 0, line, strline.size(), filling_attribute_indicator);
            state = DEFAULT_STATE;
            setSelection(line, index, line, index);
            return;
        }
        if(event->key() == Qt::Key_Tab){
            clearIndicatorRange(line, 0, line, strline.size(), filling_attribute_indicator);

            QString strline = text(line);
            int pos = SendScintilla(SCI_GETCURRENTPOS);
            int style = SendScintilla(SCI_GETSTYLEAT, pos);
            bool error = false;

            //SHIFT+TAB -> GO TO PREVIOUS ATRIBUTE
            if(event->modifiers() & Qt::ControlModifier) {
                while (pos > 0 && index > 0 && style == QsciLexerNCL::HTMLDoubleQuotedString) {
                    pos--;
                    index--;
                    if (pos >= 0)
                        style = SendScintilla(SCI_GETSTYLEAT, pos);
                }
                if (pos >= 0)
                    userFillingPreviousAttribute(line, index-1);

                else
                   error = true;

            }
            //JUST TAB -> GO TO NEXT ATTRIBUTE
            else if(event->modifiers() == Qt::NoModifier) {
                while (index < strline.size() && style == QsciLexerNCL::HTMLDoubleQuotedString){
                    index++;
                    pos++;
                    if ( index < strline.size())
                        style = SendScintilla(SCI_GETSTYLEAT, pos);
                }
                if(index <= strline.size())
                    userFillingNextAttribute(line, index+1);
                else
                    error = true;
            }

            if (error) {
                clearIndicatorRange(line, 0, line, strline.size(), filling_attribute_indicator);
                state = DEFAULT_STATE;
            }
            return;

        } else {
            QsciScintilla::keyPressEvent(event);
            getCursorPosition(&line, &index);
            updateVisualFillingAttributeField(line, index, begin, end);
        }
    }
    else {
        QsciScintilla::keyPressEvent(event);
    }
}

void NCLTextEditor::keyReleaseEvent(QKeyEvent *event) {
    QsciScintilla::keyReleaseEvent(event);
}

void NCLTextEditor::AutoCompleteCompleted(){
    qDebug() << "NCLTextEditor::AutoCompleteCompleted()";
}

void NCLTextEditor::MarkLine(int margin, int line, Qt::KeyboardModifiers state){
    (void) margin;
    (void) line;
    (void) state;
    qDebug() << "NCLTextEditor::MarkLine()";
}


//TODO: SPLIT AND MERGE THE TWO NEXT FUNCTION
void NCLTextEditor::userFillingNextAttribute(int line, int pos){
    QString strline = text(line);
    int begin, end;
    state = FILLING_ATTRIBUTES_STATE;
    int i = pos;
    while( i < strline.size() && strline[i] != '\"')
        i++;
    i++;

    if( i >= strline.size()) {
        state = DEFAULT_STATE;
        return;
    }
    setCursorPosition(line, i);

    updateVisualFillingAttributeField (line, i, begin, end);
    setSelection(line, begin, line, end);
}

void NCLTextEditor::userFillingPreviousAttribute(int line, int pos){
    QString strline = text(line);
    int begin, end;
    state = FILLING_ATTRIBUTES_STATE;
    int i = pos-1;
    while( i >= 0 && strline[i] != '\"')
        i--;

    if( i < 0) {
        state = DEFAULT_STATE;
        return;
    }
    setCursorPosition(line, i);
    updateVisualFillingAttributeField (line, i, begin, end);
    setSelection(line, begin, line, end);
}

//FIXME: ESTA INSERINDO UM ESPACO NO FINAL
void NCLTextEditor::updateVisualFillingAttributeField(int line, int pos, int &begin, int &end) {
    QString strline = text(line);
    begin = pos-1;
    end = pos;
    bool inserted_space = false;

    clearIndicatorRange(line, 0, line, strline.size(), filling_attribute_indicator);
    while( begin >= 0 && strline[begin] != '\"')
        begin--;

    if(begin < 0) {
        state = DEFAULT_STATE;
        return;
    }
    begin++;

    while( end < strline.size() && strline[end] != '\"')
        end++;

    if(end >= strline.size()) {
        state = DEFAULT_STATE;
        return;
    }

    if(begin == end) {
        insert(" ");
        end++;
        inserted_space = true;
    }
    // qDebug() << begin << end;
    fillIndicatorRange(line, begin, line, end, filling_attribute_indicator);

    if(inserted_space)
        setSelection(line, begin, line, end);
}



