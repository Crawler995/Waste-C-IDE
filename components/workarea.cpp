#include "workarea.h"
#include "features/colorboard.h"

#include <QApplication>

WorkArea::WorkArea(QWidget *parent) : QSplitter(parent)
{
    setOrientation(Qt::Horizontal);

    debugInfoArea = new DebugInfoArea(this);
    vSplitter = new QSplitter(Qt::Vertical, this);

    editorArea = new EditorArea(vSplitter);
    runOutputArea = new RunOutputArea(vSplitter);

    setHStretchFactor(debugInfoArea, 2);
    setHStretchFactor(vSplitter, 5);
    setVStretchFactor(editorArea, 6);
    setVStretchFactor(runOutputArea, 1);


    initStyle();
}

void WorkArea::initStyle() {
    editorArea->setAttribute(Qt::WA_StyledBackground);
    editorArea->setStyleSheet("QTabWidget::pane{border: none; background: " + ColorBoard::black3 + ";}"
                              "QTabBar::tab{background: " + ColorBoard::black2 + ";"
                              "color: " + ColorBoard::lightGray + ";"
                              "font-size: 16px;"
                              "padding: 10px 30px 10px 30px;}"
                              "QTabBar::tab:selected{background: " + ColorBoard::black3 + "; "
                              "border-bottom: 2px solid " + ColorBoard::blue + ";}"
                              "QTabBar::close-button {"
                              "image: url(:/icon/assets/close.png);"
                              "}");
    debugInfoArea->setAttribute(Qt::WA_StyledBackground);
    debugInfoArea->setStyleSheet("background: " + ColorBoard::black2 + ";");
    runOutputArea->setAttribute(Qt::WA_StyledBackground);
    runOutputArea->setStyleSheet("background: " + ColorBoard::black3 + ";");
    vSplitter->setHandleWidth(0);
    this->setHandleWidth(0);
}

void WorkArea::setVStretchFactor(QWidget *widget, int factor)
{
    QSizePolicy policy = widget->sizePolicy();
    policy.setVerticalStretch(factor);
    policy.setHorizontalPolicy(QSizePolicy::Maximum);
    policy.setVerticalPolicy(QSizePolicy::Maximum);
    widget->setSizePolicy(policy);
}

void WorkArea::setHStretchFactor(QWidget *widget, int factor)
{
    QSizePolicy policy = widget->sizePolicy();
    policy.setHorizontalStretch(factor);
    policy.setHorizontalPolicy(QSizePolicy::Maximum);
    policy.setVerticalPolicy(QSizePolicy::Maximum);
    widget->setSizePolicy(policy);
}

DebugInfoArea *WorkArea::getDebugInfoArea() const
{
    return debugInfoArea;
}

RunOutputArea *WorkArea::getRunOutputArea() const
{
    return runOutputArea;
}

EditorArea *WorkArea::getEditorArea() const
{
    return editorArea;
}
