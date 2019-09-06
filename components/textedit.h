#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include "../features/cpphighlighter.h"
#include "../features/ccompleter.h"

enum Dir {LEFT, RIGHT};

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0);
    void setFont(const QFont &font);

protected:
    void keyPressEvent(QKeyEvent *event);
    bool event(QEvent *e);

private:
    int getCurLineStartTabNum();
    QString getStringAroundCursor(Dir dir, int length);
    QString getPreWord();

    CPPHighLighter *highLighter;
    CCompleter *completer;
};

#endif // TEXTEDIT_H