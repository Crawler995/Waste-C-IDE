#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include "../features/cpphighlighter.h"

class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = 0);

    QTextEdit *getTextEdit() const;

    QString getFileName() const;
    void setFileName(const QString &value);

    bool getIsSave() const;
    void setIsSave(bool value);

    bool getIsAlreadyCompile() const;
    void setIsAlreadyCompile(bool value);

private:
    QTextEdit *textEdit;
    QHBoxLayout *layout;

    QString fileName;
    bool isSave;
    bool isAlreadyCompile;

    CPPHighLighter *highLighter;
};

#endif // EDITOR_H
