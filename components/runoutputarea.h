#ifndef RUNOUTPUTAREA_H
#define RUNOUTPUTAREA_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

class RunOutputArea : public QWidget
{
    Q_OBJECT
public:
    explicit RunOutputArea(QWidget *parent = 0);

private:
    QLabel *areaTitleLabel;
    QTextEdit *textEdit;
    QVBoxLayout *layout;
};

#endif // RUNOUTPUTAREA_H
