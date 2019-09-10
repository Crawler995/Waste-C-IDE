#ifndef RUNOUTPUTAREA_H
#define RUNOUTPUTAREA_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "actionbutton.h"

class RunOutputArea : public QWidget
{
    Q_OBJECT
public:
    explicit RunOutputArea(QWidget *parent = 0);

    QTextEdit *getTextEdit() const;

    ActionButton *getCompileButton() const;
    void setCompileButton(ActionButton *value);

    ActionButton *getRunButton() const;
    void setRunButton(ActionButton *value);

    ActionButton *getCompileRunButton() const;
    void setCompileRunButton(ActionButton *value);

public slots:
    void outputInfo(const QString &info);
    void outputError(const QString &error);


protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QWidget *titleWidget;
    QLabel *areaTitleLabel;
    ActionButton *compileButton, *runButton, *compileRunButton;

    QWidget *textEditWidget;
    QTextEdit *textEdit, *inputTextEdit;

    QHBoxLayout *titleLayout;
    QHBoxLayout *textEditLayout;
    QVBoxLayout *layout;

signals:
    void userInputData(const QString &data);
};

#endif // RUNOUTPUTAREA_H
