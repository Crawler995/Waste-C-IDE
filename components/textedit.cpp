#include "textedit.h"
#include <QDebug>
#include <QKeyEvent>

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent)
{
}

void TextEdit::keyPressEvent(QKeyEvent *event)
{
    QTextEdit::keyPressEvent(event);

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
    }
    QTextEdit::event(e);
    return true;
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
