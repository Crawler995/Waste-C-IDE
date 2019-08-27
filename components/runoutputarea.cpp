#include "runoutputarea.h"
#include "features/colorboard.h"

RunOutputArea::RunOutputArea(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);

    areaTitleLabel = new QLabel(tr("输出"), this);
    textEdit = new QTextEdit(this);

    layout->addWidget(areaTitleLabel);
    layout->addWidget(textEdit);
    layout->setContentsMargins(0, 0, 0, 0);

    areaTitleLabel->setStyleSheet("color: " + ColorBoard::lightGray + ";"
                                  "background: " + ColorBoard::black2 + ";"
                                  "padding: 10px 0px 10px 10px;");
    textEdit->setStyleSheet("border: none;"
                            "background: " + ColorBoard::black3 + ";"
                            "color: " + ColorBoard::lightGray + ";");
    setLayout(layout);
}
