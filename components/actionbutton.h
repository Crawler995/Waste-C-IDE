#ifndef ACTIONBUTTON_H
#define ACTIONBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QString>

class ActionButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ActionButton(const QString &text, QWidget *parent = 0);


};

#endif // ACTIONBUTTON_H
