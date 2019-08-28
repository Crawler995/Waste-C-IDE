#include "welcomepage.h"
#include "features/colorboard.h"

WelcomePage::WelcomePage(QWidget *parent) : QWidget(parent)
{
    welcomeLabel = new QLabel(tr("点击左上角\"文件\"菜单打开或创建C语言源文件"));
    welcomeLabel->setStyleSheet("color: " + ColorBoard::blue + ";"
                                "font-size: 20px;");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout = new QVBoxLayout(this);
    layout->addWidget(welcomeLabel);
    setLayout(layout);
}
