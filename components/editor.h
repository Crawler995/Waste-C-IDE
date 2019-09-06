#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include "../features/cpphighlighter.h"
#include "../features/ccompleter.h"
#include "textedit.h"

class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = 0);

    TextEdit *getTextEdit() const;

    QString getFileName() const;
    void setFileName(const QString &value);

    bool getIsSave() const;
    void setIsSave(bool value);

    bool getIsAlreadyCompile() const;
    void setIsAlreadyCompile(bool value);

private:
    TextEdit *textEdit;
    QHBoxLayout *layout;

    QString fileName;
    bool isSave;
    bool isAlreadyCompile;

    CPPHighLighter *highLighter;
    CCompleter *completer;
};

#endif // EDITOR_H
