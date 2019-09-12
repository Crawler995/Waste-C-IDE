#include "commentmanager.h"
#include <QRegExp>
#include <QDebug>

CommentManager::CommentManager(TextEdit *parent)
{
    this->textEdit = parent;
    isHide = false;
}

void CommentManager::showAllComment()
{
    foreach (Comment *comment, comments) {
        comment->start.insertText(comment->content);
    }
    isHide = false;
}

void CommentManager::hideAllComment()
{
    updateComment();
    foreach (Comment *comment, comments) {
        int len = comment->content.length();
        for(int i = 0; i < len; i++) {
            comment->start.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        }
        comment->start.removeSelectedText();

    }

    isHide = true;
}

void CommentManager::updateComment()
{
    comments.clear();
    QRegExp singleLineCommentRegx("//[^\n]*");
    QRegExp multLineCommentRegx("/\\*.*\\*/");
    multLineCommentRegx.setMinimal(true);
    findLineComment(singleLineCommentRegx);
    findLineComment(multLineCommentRegx);
}

void CommentManager::appendComment(QTextCursor start, const QString &content)
{
    Comment *comment = new Comment();
    comment->start = start;
    comment->content = content;
    comments.append(comment);
}

void CommentManager::findLineComment(QRegExp regx)
{
    QTextCursor cursor = this->textEdit->textCursor();
    int pos = 0;
    const QString textEditText = this->textEdit->toPlainText();
    int index = regx.indexIn(textEditText, pos);

    while(index != -1) {
        cursor.setPosition(index);
        QTextCursor start = cursor;
        for(int i = 0; i < regx.matchedLength(); i++) {
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        }

        qDebug() << cursor.selectedText();
        appendComment(start, cursor.selectedText());

        pos = index + regx.matchedLength();
        index = regx.indexIn(textEditText, pos);
    }
}

bool CommentManager::getIsHide() const
{
    return isHide;
}

void CommentManager::setIsHide(bool value)
{
    isHide = value;
}
