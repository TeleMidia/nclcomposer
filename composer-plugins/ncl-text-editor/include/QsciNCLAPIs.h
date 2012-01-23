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
#ifndef QSCINCLAPIS_H
#define QSCINCLAPIS_H

#include <NCLStructure.h>
using namespace composer::language;

#include <Qsci/qsciapis.h>
#include <QtDebug>

#include "QsciLexerNCL.h"

class QsciNCLAPIs : public QsciAPIs
{
    Q_OBJECT

private:
    NCLStructure *nclStructure;
    int suggesting;
    enum {
        SUGGESTING_ELEMENTS = 0,
        SUGGESTING_ATTRIBUTES,
        SUGGESTING_ATTRIBUTE_VALUES,
        SUGGESTING_OTHER
    };

    QString getRequiredAttributesAsStr (const QString &selection);
    bool isElement (int pos);
    bool isAttribute (int pos);
    bool isAttributeValue (int pos);
    QString getCurrentTagName (int pos);
    QString getCurrentAttribute (int pos);
    int getParentOffset(int pos);
    QString getParentTagName (int pos);
    void getAttributesTyped(int pos, QStringList &attrs);
    QString getAttributeValueFromCurrentElement(int pos, QString attr);
    int getStartTagBegin(int pos);
    int getStartTagLength(int pos);

    // \todo this function must be moved from here.
    QString relativePath( QString absolutePath, QString relativeTo,
                                       bool bIsFile = false);

public:
    QsciNCLAPIs(QsciLexer *lexer	 = 0);
    virtual ~QsciNCLAPIs();

    //! Update the list \a list with API entries derived from \a context.  \a
    //! context is the list of words in the text preceding the cursor position.
    //! The characters that make up a word and the characters that separate
    //! words are defined by the lexer.  The last word is a partial word and
    //! may be empty if the user has just entered a word separator.
    void updateAutoCompletionList(const QStringList &context,
            QStringList &list);

    //! This is called when the user selects the entry \a selection from the
    //! auto-completion list.  A sub-class can use this as a hint to provide
    //! more specific API entries in future calls to
    //! updateAutoCompletionList().  The default implementation does nothing.
    void autoCompletionSelected(const QString &selection);

    //! Return the call tips valid for the context \a context.  (Note that the
    //! last word of the context will always be empty.)  \a commas is the number
    //! of commas the user has typed after the context and before the cursor
    //! position.  The exact position of the list of call tips can be adjusted
    //! by specifying a corresponding left character shift in \a shifts.  This
    //! is normally done to correct for any displayed context according to \a
    //! style.
    //!
    //! \sa updateAutoCompletionList()
    QStringList callTips(const QStringList &context, int commas,
            QsciScintilla::CallTipsStyle style,
            QList<int> &shifts);


    bool event(QEvent *e);

signals:

public slots:

};

#endif // QSCINCLAPIS_H
