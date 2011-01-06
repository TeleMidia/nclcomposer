#include "QsciLexerNCL.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerNCL::QsciLexerNCL(QObject *parent)
    : QsciLexerHTML(parent),
      scripts(true)
{
}


// The dtor.
QsciLexerNCL::~QsciLexerNCL()
{
}


// Returns the language name.
const char *QsciLexerNCL::language() const
{
    return "NCL";
}


// Returns the lexer name.
const char *QsciLexerNCL::lexer() const
{
    return "ncl";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerNCL::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x00, 0x00, 0x00);

    case Tag:
    case UnknownTag:
    case XMLTagEnd:
    case SGMLDefault:
    case SGMLCommand:
        return QColor(0x00, 0x00, 0x80);

    case Attribute:
    case UnknownAttribute:
        return QColor(0x00,  0x00, 0x80);

    case HTMLNumber:
        return QColor(0x00, 0x7f, 0x7f);

    case HTMLDoubleQuotedString:
    case HTMLSingleQuotedString:
        return QColor(0x99, 0x33, 0x00);

    case OtherInTag:
    case Entity:
    case XMLStart:
    case XMLEnd:
        return QColor(0x80, 0x00, 0x80);

    case HTMLComment:
    case SGMLComment:
        return QColor(0x00, 0x7f, 0x7f);

    case CDATA:
    case PHPStart:
    case SGMLDoubleQuotedString:
    case SGMLError:
        return QColor(0x80,0x00,0x00);

    case HTMLValue:
        return QColor(0x00,0xFF,0x00);

    case SGMLParameter:
        return QColor(0x00,0x66,0x00);

    case SGMLSingleQuotedString:
        return QColor(0x99,0x33,0x00);

    case SGMLSpecial:
        return QColor(0x33,0x66,0xff);

    case SGMLEntity:
        return QColor(0x00,0x00,0x33);

    case SGMLBlockDefault:
        return QColor(0x00,0x00,0x66);
    }

    return QsciLexerHTML::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerNCL::defaultEolFill(int style) const
{
    if (style == CDATA)
        return true;

    return QsciLexerHTML::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerNCL::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Default:
    case Entity:
    case CDATA:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman",11);
#else
        f = QFont("Bitstream Charter",10);
#endif
        break;

    case XMLStart:
    case XMLEnd:
    case SGMLCommand:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexerHTML::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerNCL::keywords(int set) const
{
    if (set == 6)
        return QsciLexerHTML::keywords(set);

    return 0;
}


// Returns the background colour of the text for a style.
QColor QsciLexerNCL::defaultPaper(int style) const
{
    switch (style)
    {
    case CDATA:
        return QColor(0xff,0xf0,0xf0);

    case SGMLDefault:
    case SGMLCommand:
    case SGMLParameter:
    case SGMLDoubleQuotedString:
    case SGMLSingleQuotedString:
    case SGMLSpecial:
    case SGMLEntity:
    case SGMLComment:
        return QColor(0xef,0xef,0xff);

    case SGMLError:
        return QColor(0xff,0x66,0x66);

    case SGMLBlockDefault:
        return QColor(0xcc,0xcc,0xe0);
    }

    return QsciLexerHTML::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerNCL::refreshProperties()
{
    setScriptsProp();
    emit propertyChanged("fold.html","1");
}


// Read properties from the settings.
bool QsciLexerNCL::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = QsciLexerHTML::readProperties(qs, prefix);

    scripts = qs.value(prefix + "scriptsstyled", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerNCL::writeProperties(QSettings &qs, const QString &prefix) const
{
    int rc = QsciLexerHTML::writeProperties(qs, prefix);

    qs.setValue(prefix + "scriptsstyled", scripts);

    return rc;
}


// Return true if scripts are styled.
bool QsciLexerNCL::scriptsStyled() const
{
    return scripts;
}


// Set if scripts are styled.
void QsciLexerNCL::setScriptsStyled(bool styled)
{
    scripts = styled;

    setScriptsProp();
}


// Set the "lexer.ncl.allow.scripts" property.
void QsciLexerNCL::setScriptsProp()
{
    emit propertyChanged("lexer.xml.allow.scripts",(scripts ? "1" : "0"));
}
