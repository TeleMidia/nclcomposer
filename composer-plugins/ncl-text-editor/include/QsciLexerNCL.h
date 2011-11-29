/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>. 
 */
#ifndef QSCILEXERNCL_H
#define QSCILEXERNCL_H

#include <QObject>
#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexerhtml.h>

//! \brief The QsciLexerNCL class encapsulates the Scintilla NCL lexer.
class QsciLexerNCL : public QsciLexerHTML
{
    Q_OBJECT

public:
    //! Construct a QsciLexerNCL with parent \a parent.  \a parent is typically
    //! the QsciScintilla instance.
    QsciLexerNCL(QObject *parent = 0);

    //! Destroys the QsciLexerNCL instance.
    virtual ~QsciLexerNCL();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the end-of-line fill for style number \a style.
    bool defaultEolFill(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! If \a allowed is true then scripts are styled.  The default is true.
    //!
    //! \sa scriptsStyled()
    void setScriptsStyled(bool styled);

    //! Returns true if scripts are styled.
    //!
    //! \sa setScriptsStyled()
    bool scriptsStyled() const;

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    bool readProperties(QSettings &qs, const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    void setScriptsProp();

    bool scripts;

    QsciLexerNCL(const QsciLexerNCL &);
    QsciLexerNCL &operator=(const QsciLexerNCL &);
};

#ifdef __APPLE__
}
#endif

#endif
