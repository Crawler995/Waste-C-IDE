#include "debuginfoarea.h"
#include "features/colorboard.h"

DebugInfoArea::DebugInfoArea(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);

    areaTitleLabel = new QLabel(tr("调试"), this);
    areaTitleLabel->setStyleSheet("color: " + ColorBoard::lightGray + ";"
                                  "background: " + ColorBoard::black2 + ";"
                                  "font-size: 16px;");

    layout->addWidget(areaTitleLabel);
    layout->addStretch();

    setLayout(layout);
}
