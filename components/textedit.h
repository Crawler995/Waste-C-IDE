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

    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

public slots:
    void resizeEvent(QResizeEvent *e);

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

    QWidget *lineNumberArea;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(TextEdit *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEdit *codeEditor;
};

#endif // TEXTEDIT_H
