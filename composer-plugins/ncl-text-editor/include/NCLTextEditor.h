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
#ifndef NCLTEXTEDITOR_H
#define NCLTEXTEDITOR_H

#include <QColor>
#include <QShortcut>
#include <QWheelEvent>
#include <QMessageBox>
#include <QDomDocument>
#include <Qsci/qsciscintilla.h>

#include "MyLexer.h"

#include "QsciLexerNCL.h"
#include "QsciNCLAPIs.h"


#define PREF_FONT_FAMILY "Courier"
#define PREF_FONT_SIZE 10
/* Light Style */
#define PREF_CARET_LINE_BG_COLOR QColor("#B9D3EE")
#define PREF_FOLD_MARGIN_FORE_COLOR QColor("#B9D3EE")
#define PREF_FOLD_MARGIN_BACK_COLOR QColor("#FFFFFF")
#define MARGINS_BACKGROUND_COLOR QColor("#ffffff")

/* Dark Style */
/*#define PREF_CARET_LINE_BG_COLOR QColor("#B9D3EE")
#define PREF_FOLD_MARGIN_FORE_COLOR QColor("#FFFFFF")
#define PREF_FOLD_MARGIN_BACK_COLOR QColor("#AAAAAA")
#define MARGINS_BACKGROUND_COLOR QColor("#BBBBBB")*/

//! The NCL Text Editor
/*!
 * This is the main class of the NCL Editor program. It inherents from
 * QScintilla.
 *
 * @author Roberto Azevedo
 * @date 24 May 2011
 */
class NCLTextEditor : public QsciScintilla
{
    Q_OBJECT

public:
    typedef enum {
        TAB_BEHAVIOR_DEFAULT = 1,
        TAB_BEHAVIOR_NEXT_ATTR_AFTER_AUTOCOMPLETE,
        TAB_BEHAVIOR_NEXT_ATTR
    } TAB_BEHAVIOR;

    explicit NCLTextEditor(QWidget *parent = 0);
    virtual ~NCLTextEditor();

    void setTabBehavior(TAB_BEHAVIOR tabBehavior);
    void userFillingNextAttribute(int pos);
    void userFillingPreviousAttribute(int pos);
    void updateVisualFillingAttributeField( int line,
                                            int index,
                                            int &begin,
                                            int &end);

    void keepFocused();
    QString textWithoutUserInteraction();

    bool parseDocument();
    QDomElement elementById(QString id);
    QDomNodeList elementsByTagname(QString tagname);

private:
    enum INTERACTION_STATE {
        DEFAULT_STATE = 1,
        FILLING_ATTRIBUTES_STATE
    };

    INTERACTION_STATE interaction_state;

    QsciLexerNCL *nclexer;
    QsciNCLAPIs *apis;
    QDomDocument doc;

    int error_indicator;
    int error_marker;
    int filling_attribute_indicator;

    TAB_BEHAVIOR tabBehavior;
    bool focusInIgnoringCurrentText;
    QString textWithoutUserInter;

    void initParameters();

    /* events */
    void wheelEvent( QWheelEvent * event );
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *e);

protected:
    void AutoCompleteCompleted();
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

public slots:
    void Increasefont();
    void Decreasefont();
    void markError(QString description, QString file, int line, int column = 0,
                   int severity = 0);
    void MarkLine(int, int, Qt::KeyboardModifiers);
    void formatText();

signals:
    void focusLosted(QFocusEvent *event);
};

#endif // NCLTEXTEDITOR_H
