#include "NCLTextEditor.h"

NCLTextEditor::NCLTextEditor(QWidget *parent) :
        QsciScintilla(parent)
{
    shortcut_ctrl_space = new QShortcut(QKeySequence("Ctrl+Space"), this);
    shortcut_zoomout = new QShortcut(QKeySequence::ZoomOut, this);  /* CTRL- */
    shortcut_zoomin = new QShortcut(QKeySequence::ZoomIn, this);  /* CTRL+ */
    shortcut_showindic = new QShortcut(QKeySequence(tr("Ctrl+I")), this);  /* CTRL I*/

    initParameters();
}

NCLTextEditor::~NCLTextEditor(){
    delete apis;
    delete shortcut_ctrl_space;
    delete shortcut_zoomout;
    delete shortcut_zoomin;
    delete shortcut_showindic;
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
    MyLexer *mylexer = new MyLexer(this);

    QRegExp attrValueRegex ("\".*\"");
    QsciStyle attrValueStyle (4, "ATTRIBUTE_VALUE", QColor(0x0, 0x0, 0xFF),
                              mylexer->defaultPaper(1), mylexer->defaultFont(1));
    mylexer->addTextPartition(4, attrValueRegex, attrValueStyle);

    QRegExp startTagRegex ("<.[^><.]+>");
    QsciStyle startTagStyle (10, "START_TAG",  QColor(0x0, 0x0, 0xFF),
                             mylexer->defaultPaper(1), mylexer->defaultFont(1));
    mylexer->addTextPartition(10, startTagRegex, startTagStyle);

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
    connect(shortcut_showindic, SIGNAL(activated()), this, SLOT(ShowIndicators()));

    //connect(this, SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)), this, SLOT(MarkLine(int,int,Qt::KeyboardModifiers)));
}

void NCLTextEditor::Increasefont(){
    zoomIn();
}

void NCLTextEditor::Decreasefont(){
    zoomOut();
}

void NCLTextEditor::ShowIndicators(){
    //TODO: Show Error Messages
    fillIndicatorRange(0, 0, 0, 10, TTIndicator);
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

void NCLTextEditor::keyPressEvent(QKeyEvent *event){
    QsciScintilla::keyPressEvent(event);
}

void NCLTextEditor::keyReleaseEvent(QKeyEvent *event){
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
