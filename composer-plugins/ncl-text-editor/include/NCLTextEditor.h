#ifndef NCLTEXTEDITOR_H
#define NCLTEXTEDITOR_H

#include <QColor>
#include <QShortcut>
#include <QWheelEvent>
#include <Qsci/qsciscintilla.h>

#include "MyLexer.h"


#include "QsciLexerNCL.h"
#include "QsciNCLAPIs.h"


#define PREF_FONT_FAMILY "Courier"
#define PREF_FONT_SIZE 10
#define PREF_CARET_LINE_BG_COLOR QColor("#B9D3EE")
#define PREF_FOLD_MARGIN_FORE_COLOR QColor("#B9D3EE")
#define PREF_FOLD_MARGIN_BACK_COLOR QColor("#FFFFFF")

class NCLTextEditor : public QsciScintilla
{
    Q_OBJECT

public:
    explicit NCLTextEditor(QWidget *parent = 0);
    virtual ~NCLTextEditor();

private:
    QShortcut *shortcut_ctrl_space;
    QShortcut *shortcut_zoomout;
    QShortcut *shortcut_zoomin;
    QShortcut *shortcut_showindic;
    QsciNCLAPIs *apis;

    void initParameters();

    /* events */
    void wheelEvent( QWheelEvent * event );
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

protected:
    void AutoCompleteCompleted();

public slots:
    void Increasefont();
    void Decreasefont();
    void ShowIndicators();
    void MarkLine(int, int, Qt::KeyboardModifiers);
};

#endif // NCLTEXTEDITOR_H
