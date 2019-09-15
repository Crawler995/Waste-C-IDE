#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include "../features/cpphighlighter.h"
#include "../features/ccompleter.h"

enum Dir {LEFT, RIGHT};

class LineNumberArea;

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0);
    void setFont(const QFont &font);

protected:
    void keyPressEvent(QKeyEvent *event);
    bool event(QEvent *e);
    void mousePressEvent(QMouseEvent *event);

private:
    int getCurLineStartTabNum();
    QString getStringAroundCursor(Dir dir, int length);
    QString getPreWord();
    void highLightMatchBracketFromLeft(QChar left, QChar right);
    void highLightMatchBracketFromRight(QChar left, QChar right);
    void clearMatchBracketHighLight();

    CPPHighLighter *highLighter;
    CCompleter *completer;

    QTextCursor matchBracketCursor;

};

#endif // TEXTEDIT_H
