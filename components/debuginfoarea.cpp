#include "debuginfoarea.h"
#include "features/colorboard.h"

DebugInfoArea::DebugInfoArea(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);

    areaTitleLabel = new QLabel(tr("调试"), this);
    areaTitleLabel->setStyleSheet("color: " + ColorBoard::lightGray + ";"
                                  "background: " + ColorBoard::black2 + ";"
                                  "padding: 8px 0px 8px 0px;");

    layout->addWidget(areaTitleLabel);
    layout->addStretch();

    setLayout(layout);
}
