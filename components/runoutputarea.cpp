#include "runoutputarea.h"
#include "features/colorboard.h"

RunOutputArea::RunOutputArea(QWidget *parent) : QWidget(parent)
{
    titleWidget = new QWidget(this);
    titleLayout = new QHBoxLayout(titleWidget);

    areaTitleLabel = new QLabel(tr("输出"), this);
    compileButton = new ActionButton(tr("编译"), titleWidget);
    runButton = new ActionButton(tr("运行"), titleWidget);
    compileRunButton = new ActionButton(tr("编译运行"), titleWidget);

    titleLayout->addWidget(areaTitleLabel);
    titleLayout->addWidget(compileButton);
    titleLayout->addWidget(runButton);
    titleLayout->addWidget(compileRunButton);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setAlignment(Qt::AlignLeft);

    layout = new QVBoxLayout(this);

    textEdit = new QTextEdit(this);

    layout->addWidget(titleWidget);
    layout->addWidget(textEdit);
    layout->setContentsMargins(0, 0, 0, 0);

    titleWidget->setStyleSheet("color: " + ColorBoard::lightGray + ";"
                                  "border-top: 2px solid " + ColorBoard::black2 + ";"
                                  "padding: 10px 0px 10px 10px;"
                                  "font-size: 16px;");
    textEdit->setStyleSheet("border: none;"
                            "background: " + ColorBoard::black3 + ";"
                            "color: " + ColorBoard::lightGray + ";");
    textEdit->setReadOnly(true);
    setLayout(layout);
}
