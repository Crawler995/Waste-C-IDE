#ifndef DEBUGINFOAREA_H
#define DEBUGINFOAREA_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class DebugInfoArea : public QWidget
{
    Q_OBJECT
public:
    explicit DebugInfoArea(QWidget *parent = 0);

private:
    QLabel *areaTitleLabel;
    QVBoxLayout *layout;
};

#endif // DEBUGINFOAREA_H
