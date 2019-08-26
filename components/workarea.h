#ifndef WORKAREA_H
#define WORKAREA_H

#include <QWidget>
#include <QGridLayout>
#include <QSplitter>

class WorkArea : public QWidget
{
    Q_OBJECT
public:
    explicit WorkArea(QWidget *parent = 0);

private:
    QSplitter *vSplitter, *hSplitter;
    QWidget *debugInfoArea;
    QWidget *editorArea;
    QWidget *runOutputArea;

    QGridLayout *layout;
};

#endif // WORKAREA_H
