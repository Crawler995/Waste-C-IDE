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

public slots:
    void outputInfo(const QString &info);
    void outputError(const QString &error);

private:
    QWidget *titleWidget;
    QLabel *areaTitleLabel;
    ActionButton *compileButton, *runButton, *compileRunButton;

    QTextEdit *textEdit;

    QHBoxLayout *titleLayout;
    QVBoxLayout *layout;
};

#endif // RUNOUTPUTAREA_H
