#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>

class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = 0);

private:
    QTextEdit *textEdit;
    QHBoxLayout *layout;
};

#endif // EDITOR_H
