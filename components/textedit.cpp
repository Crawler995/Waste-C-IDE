#include "textedit.h"
#include "../features/colorboard.h"
#include <QDebug>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

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

    matchBracketCursor = QTextCursor();

    lineNumberArea = new LineNumberArea(this);
    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea/*_2*/(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
    updateLineNumberAreaWidth(0);
}

void TextEdit::setFont(const QFont &font)
{
    QTextEdit::setFont(font);
    QFontMetrics metrics(this->font());
    this->setTabStopWidth(4 * metrics.width(' '));
    completer->popup()->setFont(font);

    updateLineNumberArea();
}

int TextEdit::getFirstVisibleBlockId()
{
    QTextCursor curs = QTextCursor(this->document());
        curs.movePosition(QTextCursor::Start);
        for(int i=0; i < this->document()->blockCount(); ++i)
        {
            QTextBlock block = curs.block();

            QRect r1 = this->viewport()->geometry();
            QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                        this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                            this->verticalScrollBar()->sliderPosition()
                            ) ).toRect();

            if (r1.contains(r2, true)) { return i; }

            curs.movePosition(QTextCursor::NextBlock);
        }

        return 0;
}

void TextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(ColorBoard::black3));
    painter.fillRect(QRect(event->rect().topRight() - QPoint(1, 0), event->rect().bottomRight()),
                     QColor(ColorBoard::black1));
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    int additional_margin;
    if (blockNumber == 0)
        additional_margin = (int) this->document()->documentMargin() -1 - this->verticalScrollBar()->sliderPosition();
    else
        additional_margin = (int) this->document()->documentLayout()->blockBoundingRect(prev_block)
                .translated(0, translate_y).intersected(this->viewport()->geometry()).height();

    top += additional_margin;

    int bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();

    QColor col_1(ColorBoard::keyWordRed);
    QColor col_0(ColorBoard::lightGray);
    int width = fontMetrics().width(QLatin1Char('9'));

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(120, 120, 120));
            painter.setFont(this->font());
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? col_1 : col_0);
            painter.drawText(-width*2, top,
                             lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int TextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int width = fontMetrics().width(QLatin1Char('9'));
    int space = width * 4 +  width * (digits);

    return space;
}

void TextEdit::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);

        QRect cr = this->contentsRect();
        lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
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
        cursor.movePosition(QTextCursor::NextCharacter);
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        QChar c = cursor.selectedText()[0];
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
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
        matchBracketCursor = cursor;
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(format);
    }
    else {
        matchBracketCursor = QTextCursor();
    }

    this->setTextCursor(initCursor);
}

void TextEdit::highLightMatchBracketFromRight(QChar left, QChar right)
{
    QTextCursor endCursor = QTextCursor(this->textCursor());
    endCursor.movePosition(QTextCursor::Start);
    QTextCursor cursor = this->textCursor(), initCursor = cursor;
    QTextCharFormat format;
    format.setBackground(QColor(255, 255, 255, 60));

    bool find = true;
    int count = 1;
    while(true) {
        cursor.movePosition(QTextCursor::PreviousCharacter);
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        QChar c = cursor.selectedText()[0];
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

        if(c == left) {
            count--;
            if(count == 0) {
                break;
            }
        }
        else if(c == right) {
            count++;
        }

        if(cursor.position() == endCursor.position() + 1) {
            find = false;
            break;
        }
    }

    if(find) {
        matchBracketCursor = cursor;
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(format);
    }
    else {
        matchBracketCursor = QTextCursor();
    }

    this->setTextCursor(initCursor);
}

void TextEdit::clearMatchBracketHighLight()
{
    QTextCharFormat format;
    format.setBackground(QColor(0, 0, 0, 0));

    matchBracketCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
    matchBracketCursor.mergeCharFormat(format);

    matchBracketCursor = QTextCursor();
}

void TextEdit::updateLineNumberAreaWidth(int newBlockCount)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TextEdit::updateLineNumberArea(QRectF)
{
    TextEdit::updateLineNumberArea();
}

void TextEdit::updateLineNumberArea(int)
{
    TextEdit::updateLineNumberArea();
}

void TextEdit::updateLineNumberArea()
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());


    QRect rect =  this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);

    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineNumberArea->scroll(0, dy);
    }

    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1)
        this->verticalScrollBar()->setSliderPosition(dy-this->document()->documentMargin());
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

    else if(event->key() == ' ') {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
        QString str = cursor.selectedText();

        QRegExp regx("^[a-zA-Z0-9_]+$");
        if(regx.indexIn(str) == 0 && regx.matchedLength() == str.length()) {
            this->completer->addCompleteItem(str);
        }
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
    }
    QTextEdit::event(e);
    return true;
}

void TextEdit::mousePressEvent(QMouseEvent *event)
{
    QTextEdit::mousePressEvent(event);

    if(matchBracketCursor != QTextCursor()) {
        clearMatchBracketHighLight();
    }

    if(event->button() == Qt::LeftButton) {
        QTextCursor cursor = cursorForPosition(event->pos());
        //this->setTextCursor(cursor);

        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        QChar c = cursor.selectedText()[0];
        if(c == '(') {
            highLightMatchBracketFromLeft('(', ')');
        }
        else if(c == '[') {
            highLightMatchBracketFromLeft('[', ']');
        }
        else if(c == '{') {
            highLightMatchBracketFromLeft('{', '}');
        }
        else if(c == ')') {
            highLightMatchBracketFromRight('(', ')');
        }
        else if(c == ']') {
            highLightMatchBracketFromRight('[', ']');
        }
        else if(c == '}') {
            highLightMatchBracketFromRight('{', '}');
        }
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
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
    if(dir == RIGHT && length == 1 && this->textCursor().position() == this->toPlainText().length()) {
        return "\u0209";
    }
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
