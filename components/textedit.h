#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
enum Dir {LEFT, RIGHT};

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    bool event(QEvent *e);

private:
    int getCurLineStartTabNum();
    bool isCursorWrappedInBracket();
    QString getStringAroundCursor(Dir dir, int length);
};

#endif // TEXTEDIT_H
