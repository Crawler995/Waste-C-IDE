#include "findorreplacedialog.h"

FindOrReplaceDialog::FindOrReplaceDialog(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout(this);

    inputWordEdit = new QLineEdit(this);
    inputWordEdit->setPlaceholderText("查找...");
    targetWordEdit = new QLineEdit(this);
    targetWordEdit->setPlaceholderText("替换为...");

    isRegexMatchCheckBox = new QCheckBox("使用正则表达式", this);
    isCaseSensitiveCheckBox = new QCheckBox("大小写敏感", this);
    isMinimalCheckBox = new QCheckBox("是否非贪婪匹配", this);

    findButton = new QPushButton("查找", this);
    replaceButton = new QPushButton("替换", this);

    layout->addWidget(inputWordEdit, 0, 0, 1, 2);
    layout->addWidget(targetWordEdit, 1, 0, 1, 2);
    layout->addWidget(isRegexMatchCheckBox, 2, 0, 1, 1);
    layout->addWidget(isCaseSensitiveCheckBox, 2, 1, 1, 1);
    layout->addWidget(isMinimalCheckBox, 3, 0, 1, 1);
    layout->addWidget(findButton, 4, 0, 1, 1);
    layout->addWidget(replaceButton, 4, 1, 1, 1);

    setLayout(layout);

    setStyleSheet("font-size: 16px;");
}

QPushButton *FindOrReplaceDialog::getFindButton() const
{
    return findButton;
}

QPushButton *FindOrReplaceDialog::getReplaceButton() const
{
    return replaceButton;
}

QLineEdit *FindOrReplaceDialog::getInputWordEdit() const
{
    return inputWordEdit;
}

QLineEdit *FindOrReplaceDialog::getTargetWordEdit() const
{
    return targetWordEdit;
}

bool FindOrReplaceDialog::getIsRegexMatch()
{
    return isRegexMatchCheckBox->isChecked();
}

bool FindOrReplaceDialog::getIsCaseSensitive()
{
    return isCaseSensitiveCheckBox->isChecked();
}

bool FindOrReplaceDialog::getIsMinimal()
{
    return isMinimalCheckBox->isChecked();
}
