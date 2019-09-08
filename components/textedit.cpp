#include "textedit.h"
#include "../features/colorboard.h"
#include <QDebug>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QScrollBar>

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent)
{
    highLighter = new CPPHighLighter(this->document());

    completer = new CCompleter(this);
    completer->setWidget(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    completer->popup()->setStyleSheet("QAbstractItemView{background: " + ColorBoard::black1 + "; border:none;"
                                      "color: " + ColorBoard::lightGray + ";}"
                                      "QAbstractItemView::item {background: " + ColorBoard::black1 + ";}"
                                      "QListView::item:selected{ background: " + ColorBoard::black2 + ";"
                                      "color: " + ColorBoard::blue + ";}"
                                      "QListView::item:selected:active{ background: " + ColorBoard::black2 + ";"
                                      "color: " + ColorBoard::blue + ";}"
                                      "QListView::item:selected:!active{ background: " + ColorBoard::black2 + ";"
                                      "color: " + ColorBoard::blue + ";}");

    connect(completer, static_cast<void(QCompleter::*)(const QString &)>(&QCompleter::activated),
            this, [=](const QString &completion) {
        QTextCursor tc = textCursor();
        int extra = completion.length() - completer->completionPrefix().length();
        tc.movePosition(QTextCursor::Left);
        tc.movePosition(QTextCursor::EndOfWord);
        tc.insertText(completion.right(extra));
        setTextCursor(tc);

        completer->popup()->hide();
    });

    connect(this, &QTextEdit::textChanged, this,
            [=]() {
        if(this->getPreWord() == "") {
            completer->popup()->hide();
            return;
        }
        completer->setCompletionPrefix(this->getPreWord());
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));

        QRect cr = cursorRect();
        cr.setWidth(completer->popup()->sizeHintForColumn(0)
               + completer->popup()->verticalScrollBar()->sizeHint().width());
        completer->complete(cr);
    });
}

void TextEdit::setFont(const QFont &font)
{
    QTextEdit::setFont(font);
    QFontMetrics metrics(this->font());
    this->setTabStopWidth(4 * metrics.width(' '));
    completer->popup()->setFont(font);
}

QString TextEdit::getPreWord()
{
    QTextCursor initCursor = this->textCursor();

    this->moveCursor(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
    QString res = this->textCursor().selectedText();
    this->setTextCursor(initCursor);
    return res;
}

void TextEdit::highLightMatchBracketFromLeft(QChar left, QChar right)
{
    QTextCursor endCursor = QTextCursor(this->textCursor());
    endCursor.movePosition(QTextCursor::End);
    QTextCursor cursor = this->textCursor(), initCursor = cursor;
    QTextCharFormat format;
    format.setBackground(QColor(255, 255, 255, 60));

    bool find = true;
    int count = 1;
    while(true) {
        this->moveCursor(QTextCursor::NextCharacter);
        QChar c = getStringAroundCursor(LEFT, 1)[0];
        if(c == right) {
            count--;
            if(count == 0) {
                break;
            }
        }
        else if(c == left) {
            count++;
        }

        if(this->textCursor().position() == endCursor.position()) {
            find = false;
            break;
        }
    }

    if(find) {
        this->moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        this->textCursor().mergeCharFormat(format);
    }

    this->setTextCursor(initCursor);
}

void TextEdit::highLightMatchBracketFromRight(QChar left, QChar right)
{
    QTextCursor endCursor = QTextCursor(this->textCursor());
    endCursor.movePosition(QTextCursor::End);
    QTextCursor cursor = this->textCursor(), initCursor = cursor;
    QTextCharFormat format;
    format.setBackground(QColor(255, 255, 255, 60));

    bool find = true;
    int count = 1;
    while(true) {
        this->moveCursor(QTextCursor::PreviousCharacter);
        QChar c = getStringAroundCursor(LEFT, 1)[0];
        if(c == left) {
            count--;
            if(count == 0) {
                break;
            }
        }
        else if(c == right) {
            count++;
        }

        if(this->textCursor().position() == endCursor.position()) {
            find = false;
            break;
        }
    }

    if(find) {
        this->moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        this->textCursor().mergeCharFormat(format);
    }

    this->setTextCursor(initCursor);
}

void TextEdit::clearMatchBracketHighLight()
{
    QTextCursor cursor = this->textCursor(), initCursor = cursor;
    QTextCharFormat format;
    format.setBackground(QColor(0, 0, 0, 0));

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.mergeCharFormat(format);

    this->setTextCursor(initCursor);
}

void TextEdit::keyPressEvent(QKeyEvent *event)
{
    bool isIgnore = false;

    if(event->key() == '[') {
        this->insertPlainText("]");
        this->moveCursor(QTextCursor::PreviousCharacter);
    }
    else if(event->key() == '(') {
        this->insertPlainText(")");
        this->moveCursor(QTextCursor::PreviousCharacter);
    }
    else if(event->key() == '\'') {
        this->insertPlainText("'");
        this->moveCursor(QTextCursor::PreviousCharacter);
    }
    else if(event->key() == '"') {
        this->insertPlainText("\"");
        this->moveCursor(QTextCursor::PreviousCharacter);
    }
    else if(event->key() == '{') {
        this->insertPlainText("}");
        this->moveCursor(QTextCursor::PreviousCharacter);
    }
    else if(event->key() == '<') {
        this->insertPlainText(">");
        this->moveCursor(QTextCursor::PreviousCharacter);
    }


    else if(event->key() == Qt::Key_Return) {
        if(completer && completer->popup()->isVisible()) {
            event->ignore();
            isIgnore = true;
        }
    }

    if(!isIgnore) {
        QTextEdit::keyPressEvent(event);
    }
}

bool TextEdit::event(QEvent *e)
{
    if(e->type() == QEvent::KeyPress) {
        QKeyEvent *event = static_cast<QKeyEvent*>(e);
        if(event->key() == Qt::Key_Return) {
            QChar preChar = getStringAroundCursor(LEFT, 1)[0];
            if(preChar == '(' || preChar == '[' || preChar == '{') {
                int curLineStartTabNum = getCurLineStartTabNum();
                this->insertPlainText("\r\n");
                for(int i = 0; i <= curLineStartTabNum; i++) {
                    this->insertPlainText("\t");
                }
                this->insertPlainText("\r\n");
                for(int i = 0; i < curLineStartTabNum; i++) {
                    this->insertPlainText("\t");
                }
                for(int i = 0; i <= curLineStartTabNum; i++) {
                    this->moveCursor(QTextCursor::PreviousCharacter);
                }
            }
            else {
                if(completer && completer->popup()->isVisible()) {
                    QTextEdit::event(e);
                    return true;
                }
                int curLineStartTabNum = getCurLineStartTabNum();
                this->insertPlainText("\r\n");
                for(int i = 0; i < curLineStartTabNum; i++) {
                    this->insertPlainText("\t");
                }
            }
            return true;
        }

        else if(event->key() == ')') {
            QChar nextChar = getStringAroundCursor(RIGHT, 1)[0];
            if(nextChar == ')') {
                this->moveCursor(QTextCursor::NextCharacter);
            }
            else {
                QTextEdit::event(e);
            }
            return true;
        }

        else if(event->key() == ']') {
            QChar nextChar = getStringAroundCursor(RIGHT, 1)[0];
            if(nextChar == ']') {
                this->moveCursor(QTextCursor::NextCharacter);
            }
            else {
                QTextEdit::event(e);
            }
            return true;
        }

        else if(event->key() == '}') {
            QChar nextChar = getStringAroundCursor(RIGHT, 1)[0];
            if(nextChar == '}') {
                this->moveCursor(QTextCursor::NextCharacter);
            }
            else {
                QTextEdit::event(e);
            }
            return true;
        }

        else if(event->key() == '"') {
            QChar nextChar = getStringAroundCursor(RIGHT, 1)[0];
            if(nextChar == '"') {
                this->moveCursor(QTextCursor::NextCharacter);
            }
            else {
                QTextEdit::event(e);
            }
            return true;
        }

        else if(event->key() == '\'') {
            QChar nextChar = getStringAroundCursor(RIGHT, 1)[0];
            if(nextChar == '\'') {
                this->moveCursor(QTextCursor::NextCharacter);
            }
            else {
                QTextEdit::event(e);
            }
            return true;
        }

        else if(event->key() == '>') {
            QChar nextChar = getStringAroundCursor(RIGHT, 1)[0];
            if(nextChar == '>') {
                this->moveCursor(QTextCursor::NextCharacter);
            }
            else {
                QTextEdit::event(e);
            }
            return true;
        }
    }
    QTextEdit::event(e);
    return true;
}

void TextEdit::mousePressEvent(QMouseEvent *event)
{
    QTextEdit::mousePressEvent(event);
    clearMatchBracketHighLight();

    if(event->button() == Qt::LeftButton) {
        QTextCursor cursor = cursorForPosition(event->pos());
        this->setTextCursor(cursor);

        if(getStringAroundCursor(LEFT, 1)[0] == '(') {
            highLightMatchBracketFromLeft('(', ')');
        }
        else if(getStringAroundCursor(LEFT, 1)[0] == '[') {
            highLightMatchBracketFromLeft('[', ']');
        }
        else if(getStringAroundCursor(LEFT, 1)[0] == '{') {
            highLightMatchBracketFromLeft('{', '}');
        }
        else if(getStringAroundCursor(LEFT, 1)[0] == ')') {
            highLightMatchBracketFromRight('(', ')');
        }
        else if(getStringAroundCursor(LEFT, 1)[0] == ']') {
            highLightMatchBracketFromRight('[', ']');
        }
        else if(getStringAroundCursor(LEFT, 1)[0] == '}') {
            highLightMatchBracketFromRight('{', '}');
        }
    }
}

int TextEdit::getCurLineStartTabNum()
{
    QTextCursor initCursor = this->textCursor();

    this->moveCursor(QTextCursor::StartOfLine);

    int res = 0;
    while(true) {
        this->moveCursor(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        if(this->textCursor().selectedText() != QString(res + 1, '\t')) {
            break;
        }
        res++;
    }

    this->setTextCursor(initCursor);
    return res;
}

QString TextEdit::getStringAroundCursor(Dir dir, int length)
{
    for(int i = 0; i < length; i++) {
        this->moveCursor(dir == LEFT ?
                             QTextCursor::PreviousCharacter :
                             QTextCursor::NextCharacter,
                         QTextCursor::KeepAnchor);
    }

    QString res = this->textCursor().selectedText();

    for(int i = 0; i < length; i++) {
        this->moveCursor(dir == RIGHT ?
                             QTextCursor::PreviousCharacter :
                             QTextCursor::NextCharacter,
                         QTextCursor::KeepAnchor);
    }

    return res;
}
