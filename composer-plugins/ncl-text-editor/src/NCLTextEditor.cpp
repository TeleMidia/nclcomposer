#include "NCLTextEditor.h"

NCLTextEditor::NCLTextEditor(QWidget *parent) :
        QsciScintilla(parent)
{
    shortcut_ctrl_space = new QShortcut(QKeySequence("Ctrl+Space"), this);
    shortcut_ctrl_shift_f = new QShortcut(QKeySequence("Ctrl+Shift+f"), this);
    shortcut_zoomout = new QShortcut(QKeySequence::ZoomOut, this);  /* CTRL- */
    shortcut_zoomin = new QShortcut(QKeySequence::ZoomIn, this);  /* CTRL+ */

    initParameters();
}

NCLTextEditor::~NCLTextEditor()
{
    delete apis;
    delete shortcut_ctrl_space;
    delete shortcut_ctrl_shift_f;
    delete shortcut_zoomout;
    delete shortcut_zoomin;
}

void NCLTextEditor::initParameters()
{
    tabBehavior = TAB_BEHAVIOR_DEFAULT;

    setAutoIndent(true);
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setFoldMarginColors(PREF_FOLD_MARGIN_FORE_COLOR,
                        PREF_FOLD_MARGIN_BACK_COLOR);

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
                              mylexer->defaultPaper(1),
                              mylexer->defaultFont(1));
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
    connect( shortcut_ctrl_space, SIGNAL(activated()),
             this, SLOT(autoCompleteFromAPIs()));

    /* Ctrl + Space == Autocomplete */
    connect( shortcut_ctrl_shift_f, SIGNAL(activated()),
             this, SLOT(formatText()));

    /* Zoomin e Zoomout == Ctrl + -  && Ctrl + + */
    connect(shortcut_zoomout, SIGNAL(activated()), this, SLOT(Decreasefont()));
    connect(shortcut_zoomin, SIGNAL(activated()), this, SLOT(Increasefont()));

    // connect(this, SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)), this,
    // SLOT(MarkLine(int,int,Qt::KeyboardModifiers)));

    error_marker = markerDefine(QPixmap(":/images/error-icon-16.png"), -1);
    error_indicator = indicatorDefine(RoundBoxIndicator, 1);
    setIndicatorForegroundColor(QColor("#FF0000"), error_indicator);
    filling_attribute_indicator = indicatorDefine (RoundBoxIndicator, 2);
    qDebug() << error_marker << " " << error_indicator;
}

void NCLTextEditor::Increasefont()
{
    zoomIn();
}

void NCLTextEditor::Decreasefont()
{
    zoomOut();
}

void NCLTextEditor::markError ( QString description,
                                QString file,
                                int line,
                                int column,
                                int severity)
{
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

    clearIndicatorRange (0,
                         0,
                         nr_lines,
                         tmp2.size(),
                         error_indicator);

    fillIndicatorRange ( line-1,
                         indentation,
                         line-1,
                         tmp.size()-1,
                         error_indicator);

}

void NCLTextEditor::wheelEvent (QWheelEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier){
        if(event->delta() > 0)
            zoomIn();
        else
            zoomOut();
    }
    QsciScintilla::wheelEvent(event);
}

//TODO: Maybe move the mousePressEvent function content, and part of the
// keyPressEvent, to a function connected to selectionChanged signal.
void NCLTextEditor::mousePressEvent(QMouseEvent *event)
{
    int previousLine, previousIndex;
    getCursorPosition(&previousLine, &previousIndex);

    QString strline = text(previousLine);
    clearIndicatorRange ( previousLine,
                          0,
                          previousLine,
                          strline.size(),
                          filling_attribute_indicator);

    QsciScintilla::mousePressEvent(event);

    int line, index, begin, end;
    getCursorPosition(&line, &index);
    int pos, style;
    pos = SendScintilla(SCI_GETCURRENTPOS);
    style = SendScintilla(SCI_GETSTYLEAT, pos);

    //FIXME: Attribute also can be between Single quotes
    if (style == QsciLexerNCL::HTMLDoubleQuotedString) {
        interaction_state = FILLING_ATTRIBUTES_STATE;
        getCursorPosition (&line, &index);
        updateVisualFillingAttributeField(line, index, begin, end);
    } else {
        interaction_state = DEFAULT_STATE;
    }
}

//FIXME: I DONT KNOW WHY (or WHERE), BUT THE UNDO IS NOT WORKING EVERY TIME.
void NCLTextEditor::keyPressEvent(QKeyEvent *event)
{
    int begin, end;
    int line, index;
    getCursorPosition(&line, &index);
    int pos, style, size;
    pos = SendScintilla(SCI_GETCURRENTPOS);
    size = SendScintilla(SCI_GETTEXTLENGTH);

    // If I receive a Qt::Key_Backtab
    // change event to Tab+Shift
    if(event->key() == Qt::Key_Backtab) {
        event->accept();
        event = new QKeyEvent(event->type(), Qt::Key_Tab, Qt::ShiftModifier);
    }

    if (interaction_state == FILLING_ATTRIBUTES_STATE) {
        QString strline = text(line);
        clearIndicatorRange ( line,
                              0,
                              line,
                              strline.size(),
                              filling_attribute_indicator);

        if(event->key() == Qt::Key_Return){
            interaction_state = DEFAULT_STATE;
            setSelection(line, index, line, index);
            return;
        }

        if(event->key() == Qt::Key_Tab){
            QString strline = text(line);
            pos = SendScintilla(SCI_GETCURRENTPOS);
            style = SendScintilla(SCI_GETSTYLEAT, pos);
            bool error = false;

            //SHIFT+TAB -> GO TO PREVIOUS ATRIBUTE
            if(event->modifiers() & Qt::ShiftModifier) {
                while (pos > 0 ) {
                    style = SendScintilla(SCI_GETSTYLEAT, pos);
                    if(style != QsciLexerNCL::HTMLDoubleQuotedString)
                        break;
                    pos--;
                }

                if (pos >= 0)
                    userFillingPreviousAttribute(pos);
                else
                    error = true;

            }
            //JUST TAB -> GO TO NEXT ATTRIBUTE
            else if(event->modifiers() == Qt::NoModifier) {
                while (pos < size){
                    style = SendScintilla(SCI_GETSTYLEAT, pos);
                    if(style != QsciLexerNCL::HTMLDoubleQuotedString)
                        break;
                    pos++;
                }

                if(pos < size)
                    userFillingNextAttribute(pos);
                else
                    error = true;
            }

            if (error) {
                clearIndicatorRange( line,
                                     0,
                                     line,
                                     strline.size(),
                                     filling_attribute_indicator );
                interaction_state = DEFAULT_STATE;
            }
            return;

        } else {
            QsciScintilla::keyPressEvent ( event ) ;
            getCursorPosition (&line, &index);
            pos = SendScintilla(SCI_GETCURRENTPOS);
            style = SendScintilla(SCI_GETSTYLEAT, pos);

            // Test if pos-1 is also inside the attribute, otherwise it will
            // treat a text inside the end of previous Quote and the start of
            // the current one as an attribute
            if (    style == QsciLexerNCL::HTMLDoubleQuotedString
                 && pos-1 >=0 ) {
                //TODO: IMPROVE PERFORMANCE
                recolor();
                style = SendScintilla(SCI_GETSTYLEAT, pos-1);

                if(style == QsciLexerNCL::HTMLDoubleQuotedString) {
                    updateVisualFillingAttributeField(line, index, begin, end);
                    return;
                }
            }
            clearIndicatorRange( line,
                                 0,
                                 line,
                                 strline.size(),
                                 filling_attribute_indicator);

            interaction_state = DEFAULT_STATE;
        }
    }
    else {
        QsciScintilla::keyPressEvent(event);
        pos = SendScintilla(SCI_GETCURRENTPOS);
        style = SendScintilla(SCI_GETSTYLEAT, pos);

        //Test if pos-1 is also inside the attribute, otherwise it will
        // treat a text inside the end of previous Quote and the start of
        // the current one as an attribute
        if (style == QsciLexerNCL::HTMLDoubleQuotedString &&
            pos-1 >=0 ) {
            //TODO: IMPROVE PERFORMANCE
            recolor();
            style = SendScintilla(SCI_GETSTYLEAT, pos-1);

            //FIXME: Attribute also can be between Single quotes
            if (tabBehavior == TAB_BEHAVIOR_NEXT_ATTR &&
                (style == QsciLexerNCL::HTMLDoubleQuotedString)) {

                interaction_state = FILLING_ATTRIBUTES_STATE;
                getCursorPosition (&line, &index);

                updateVisualFillingAttributeField(line, index, begin, end);
            }
        }
    }
}

void NCLTextEditor::keyReleaseEvent(QKeyEvent *event)
{
    QsciScintilla::keyReleaseEvent(event);
}

void NCLTextEditor::AutoCompleteCompleted()
{
//    qDebug() << "NCLTextEditor::AutoCompleteCompleted()";
}

void NCLTextEditor::MarkLine(int margin, int line, Qt::KeyboardModifiers state)
{
    (void) margin;
    (void) line;
    (void) state;
//    qDebug() << "NCLTextEditor::MarkLine()";
}

void NCLTextEditor::userFillingNextAttribute(int pos)
{
    int begin, end, style, i = pos;
    int size = SendScintilla(SCI_GETTEXTLENGTH);

    interaction_state = FILLING_ATTRIBUTES_STATE;

    qDebug() << pos;

    while( i < size ) {
        style = SendScintilla(SCI_GETSTYLEAT, i);
        if (style == QsciLexerNCL::HTMLDoubleQuotedString)
            break;
        i++;
    }
    i++;

    qDebug() << i;

    if( i >= size ) {
        interaction_state = DEFAULT_STATE;
        return;
    }

    int newline, newindex;
    lineIndexFromPosition(i, &newline, &newindex);
    setCursorPosition(newline, newindex);
    updateVisualFillingAttributeField (newline, newindex, begin, end);
    setSelection(newline, begin, newline, end);
}

void NCLTextEditor::userFillingPreviousAttribute(int pos)
{
    int begin, end, style;
    int i = pos-1;
    interaction_state = FILLING_ATTRIBUTES_STATE;

    while( i >= 0) {
        style = SendScintilla(SCI_GETSTYLEAT, i);
        if (style == QsciLexerHTML::HTMLDoubleQuotedString)
            break;
        i--;
    }

    if( i < 0) {
        interaction_state = DEFAULT_STATE;
        return;
    }

    int newline, newindex;
    lineIndexFromPosition(i, &newline, &newindex);
    setCursorPosition(newline, newindex);
    updateVisualFillingAttributeField (newline, newindex, begin, end);
    setSelection(newline, begin, newline, end);
}

//FIXME: ESTA INSERINDO UM ESPACO NO FINAL
void NCLTextEditor::updateVisualFillingAttributeField( int line,
                                                       int pos,
                                                       int &begin,
                                                       int &end)
{
    QString strline = text(line);
    begin = pos-1;
    end = pos;
    bool inserted_space = false;

    clearIndicatorRange( line,
                         0,
                         line,
                         strline.size(),
                         filling_attribute_indicator);

    while( begin >= 0 && strline[begin] != '\"')
        begin--;

    if(begin < 0) {
        interaction_state = DEFAULT_STATE;
        return;
    }
    begin++;

    while( end < strline.size() && strline[end] != '\"')
        end++;

    if(end >= strline.size()) {
        interaction_state = DEFAULT_STATE;
        return;
    }

    if(begin == end) {
        insert(" ");
        end++;
        inserted_space = true;
    }
    fillIndicatorRange(line, begin, line, end, filling_attribute_indicator);

    if(inserted_space) setSelection(line, begin, line, end);
}

void NCLTextEditor::setTabBehavior(TAB_BEHAVIOR tabBehavior)
{
    this->tabBehavior = tabBehavior;
}

void NCLTextEditor::formatText()
{
    qDebug() << "NCLTextEditor::formatText()";
}

