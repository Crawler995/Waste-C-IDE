#ifndef COMMENTMANAGER_H
#define COMMENTMANAGER_H

#include "../components/textedit.h"
#include <QTextCursor>
#include <QVector>

struct Comment {
    QTextCursor start;
    QString content;
};

class CommentManager : public QObject
{
    Q_OBJECT
public:
    explicit CommentManager(TextEdit *parent = 0);

    bool getIsHide() const;
    void setIsHide(bool value);

public slots:
    void showAllComment();
    void hideAllComment();
    void updateComment();

private:
    TextEdit *textEdit;

    QVector<Comment*> comments;
    bool isHide;

    void appendComment(QTextCursor start, const QString &content);
    void findLineComment(QRegExp regx);
};

#endif // COMMENTMANAGER_H
