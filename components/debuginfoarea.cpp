#include "debuginfoarea.h"
#include "features/colorboard.h"

#include <QList>
#include <QDebug>

DebugInfoArea::DebugInfoArea(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);

    areaTitleLabel = new QLabel(tr("调试"), this);
    areaTitleLabel->setStyleSheet("color: " + ColorBoard::lightGray + ";"
                                  "background: " + ColorBoard::black2 + ";"
                                  "font-size: 16px;");

    initButtons();
    initBreakPointTreeView();

    layout->addWidget(areaTitleLabel);
    layout->addWidget(buttonWidget);
    layout->addWidget(breakPointTreeView);
    layout->addStretch();

    setLayout(layout);

    breakPointLines.append(2);
}

void DebugInfoArea::addBreakPointLine(int line)
{
    breakPointLines.append(line);
}

ActionButton *DebugInfoArea::getDebugButton() const
{
    return debugButton;
}

ActionButton *DebugInfoArea::getAddBreakPointButton() const
{
    return addBreakPointButton;
}

QVector<int> DebugInfoArea::getBreakPointLines() const
{
    return breakPointLines;
}

void DebugInfoArea::initButtons()
{
    buttonWidget = new QWidget(this);
    buttonLayout = new QGridLayout(buttonWidget);

    addBreakPointButton = new ActionButton("添加断点", buttonWidget);

    debugButton = new ActionButton("调试", buttonWidget);
    addWatchButton = new ActionButton("添加查看", buttonWidget);
    nextStepButton = new ActionButton("下一步", buttonWidget);
    jumpButton = new ActionButton("跳过", buttonWidget);
    nextSentenceButton = new ActionButton("下一条语句", buttonWidget);
    stopButton = new ActionButton("停止执行", buttonWidget);
    singleStepEnterButton = new ActionButton("单步进入", buttonWidget);
    jumpFuncButton = new ActionButton("跳过函数", buttonWidget);
    enterSentenceButton = new ActionButton("进入语句", buttonWidget);

    buttonLayout->addWidget(addBreakPointButton, 0, 0, 1, 1);
    buttonLayout->addWidget(debugButton, 0, 1, 1, 1);
    buttonLayout->addWidget(addWatchButton, 1, 0, 1, 1);
    buttonLayout->addWidget(nextStepButton, 1, 1, 1, 1);
    buttonLayout->addWidget(jumpButton, 2, 0, 1, 1);
    buttonLayout->addWidget(nextSentenceButton, 2, 1, 1, 1);
    buttonLayout->addWidget(stopButton, 3, 0, 1, 1);
    buttonLayout->addWidget(singleStepEnterButton, 3, 1, 1, 1);
    buttonLayout->addWidget(jumpFuncButton, 4, 0, 1, 1);
    buttonLayout->addWidget(enterSentenceButton, 4, 1, 1, 1);

    buttonWidget->setLayout(buttonLayout);

    buttonWidget->setStyleSheet("font-size: 16px;");
}

void DebugInfoArea::initBreakPointTreeView()
{
    breakPointTreeView = new QTreeView(this);
    breakPointTreeView->setStyleSheet("QTreeView{font-size: 16px;"
                                      "background: transparent; outline: none;"
                                      "color: " + ColorBoard::lightGray + ";"
                                      "border: none;}"
                                      "QTreeView::item:hover{background:" + ColorBoard::black1 + ";}"
                                      "QTreeView::item:selected{border: 0px solid " + ColorBoard::blue + ";}"
                                      "QHeaderView::section{"
                                      "color:" + ColorBoard::lightGray + ";"
                                      "background:" + ColorBoard::black1 + ";"
                                      "border: none; padding: 4px; font-size: 16px;}");
    breakPointItemModel = new QStandardItemModel(breakPointTreeView);

    breakPointItemModel->setHorizontalHeaderLabels(QStringList()
                                                   << QStringLiteral("变量")
                                                   << QStringLiteral("值"));

    breakPointTreeView->setModel(breakPointItemModel);
}

void DebugInfoArea::appendItem(const QString &name, const QString &value)
{
    QList<QStandardItem*> breakPoint;
    QStandardItem *nameItem = new QStandardItem(name);
    QStandardItem *valueItem = new QStandardItem(value);
    breakPoint.append(nameItem);
    breakPoint.append(valueItem);
    breakPointItemModel->appendRow(breakPoint);
}
