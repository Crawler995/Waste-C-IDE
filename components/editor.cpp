#include "editor.h"
#include "features/colorboard.h"
#include "editorarea.h"

#include <QFont>
#include <QDebug>
#include <QFontMetrics>
#include <QTextCharFormat>
#include <QRegExp>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background: " + ColorBoard::black3 + "; border: none;"
                  "color: " + ColorBoard::normalTextLightYellow + ";"
                  "padding-left: 10px;");

    layout = new QHBoxLayout(this);
    textEdit = new TextEdit(this);
    textEdit->setFont(qobject_cast<EditorArea*>(parent)->getDefaultFont());
    layout->addWidget(textEdit);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    fileName = "";
    isAlreadyCompile = false;

    connect(textEdit, &QTextEdit::textChanged,
            this, [=] {
        isSave = false;
    });

    isSave = true;
}

TextEdit *Editor::getTextEdit() const
{
    return textEdit;
}

QString Editor::getFileName() const
{
    return fileName;
}

void Editor::setFileName(const QString &value)
{
    fileName = value;
}

bool Editor::getIsSave() const
{
    return isSave;
}

void Editor::setIsSave(bool value)
{
    isSave = value;
}

bool Editor::getIsAlreadyCompile() const
{
    return isAlreadyCompile;
}

void Editor::setIsAlreadyCompile(bool value)
{
    isAlreadyCompile = value;
}

void Editor::findWordAndHighLight(const QString &word, bool isRegex, bool isCaseSensitive)
{
    if(!indexes.empty()) {
        clearHighLightOfFoundWord();
    }

    QTextCharFormat format;
    format.setBackground(QColor(255, 255, 255, 60));
    QTextCursor cursor = this->textEdit->textCursor(), initCursor = cursor;

    if(isRegex) {
        QRegExp regx(word);
        regx.setMinimal(true);
        regx.setCaseSensitivity(Qt::CaseSensitivity(isCaseSensitive));
        int pos = 0;
        const QString textEditText = this->textEdit->toPlainText();
        int index = regx.indexIn(textEditText, pos);

        while(index != -1) {
            indexes.append(index);

            cursor.setPosition(index);
            for(int i = 0; i < regx.matchedLength(); i++) {
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            }
            cursor.mergeCharFormat(format);

            pos = index + regx.matchedLength();
            index = regx.indexIn(textEditText, pos);
        }
    }
    else {
        int pos = 0;
        const QString textEditText = this->textEdit->toPlainText();
        int index = textEditText.indexOf(word, pos, Qt::CaseSensitivity(isCaseSensitive));

        while(index != -1) {
            indexes.append(index);

            cursor.setPosition(index);
            for(int i = 0; i < word.size(); i++) {
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            }
            cursor.mergeCharFormat(format);

            pos = index + word.size();
            index = textEditText.indexOf(word, pos, Qt::CaseSensitivity(isCaseSensitive));
        }
    }

    this->textEdit->setTextCursor(initCursor);
}

void Editor::replaceWordAndHighLight(const QString &word, const QString &target, bool isRegex, bool isCaseSensitive)
{
    if(!indexes.empty()) {
        clearHighLightOfFoundWord();
    }

    QTextCharFormat format;
    format.setBackground(QColor(255, 255, 255, 60));
    QTextCursor cursor = this->textEdit->textCursor(), initCursor = cursor;

    if(isRegex) {
        QRegExp regx(word);
        regx.setMinimal(true);
        regx.setCaseSensitivity(Qt::CaseSensitivity(isCaseSensitive));
        int pos = 0;
        QString textEditText = this->textEdit->toPlainText();
        int index = regx.indexIn(textEditText, pos);

        while(index != -1) {
            indexes.append(index);

            cursor.setPosition(index);
            for(int i = 0; i < regx.matchedLength(); i++) {
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            }
            cursor.removeSelectedText();
            cursor.insertText(target, format);

            pos = index + regx.matchedLength();
            textEditText = this->textEdit->toPlainText();

            index = regx.indexIn(textEditText, pos);
        }
    }
    else {
        int pos = 0;
        QString textEditText = this->textEdit->toPlainText();
        int index = textEditText.indexOf(word, pos, Qt::CaseSensitivity(isCaseSensitive));

        while(index != -1) {
            indexes.append(index);

            cursor.setPosition(index);
            for(int i = 0; i < word.size(); i++) {
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            }
            cursor.removeSelectedText();
            cursor.insertText(target, format);

            pos = index + target.size();
            textEditText = this->textEdit->toPlainText();

            index = textEditText.indexOf(word, pos, Qt::CaseSensitivity(isCaseSensitive));
        }
    }

    this->textEdit->setTextCursor(initCursor);
}

void Editor::clearHighLightOfFoundWord()
{
    QTextCursor cursor = this->textEdit->textCursor(), initCursor = cursor;
    QTextCharFormat format;
    format.setBackground(QColor(0, 0, 0, 0));

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.mergeCharFormat(format);

    this->textEdit->setTextCursor(initCursor);

    indexes.clear();
}
