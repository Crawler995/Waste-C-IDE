#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class WelcomePage : public QWidget
{
    Q_OBJECT
public:
    explicit WelcomePage(QWidget *parent = 0);

private:
    QLabel *welcomeLabel;
    QVBoxLayout *layout;
};

#endif // WELCOMEPAGE_H
