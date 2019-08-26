#include "workarea.h"

WorkArea::WorkArea(QWidget *parent) : QWidget(parent)
{
    vSplitter = new QSplitter(Qt::Vertical, this);

    hSplitter = new QSplitter(Qt::Horizontal);
    debugInfoArea = new QWidget(hSplitter);
    debugInfoArea->setStyleSheet("background: red");
    editorArea = new QWidget(hSplitter);
    editorArea->setStyleSheet("background: green");

    runOutputArea = new QWidget();
    runOutputArea->setStyleSheet("background: yellow");

    vSplitter->addWidget(hSplitter);
    vSplitter->addWidget(runOutputArea);
    vSplitter->setStretchFactor(0, 4);
    vSplitter->setStretchFactor(1, 1);

    hSplitter->setStretchFactor(0, 1);
    hSplitter->setStretchFactor(1, 4);

    layout = new QGridLayout(this);
    layout->addWidget(vSplitter);
    setLayout(layout);
}
