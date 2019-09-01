#include "editor.h"
#include "features/colorboard.h"

#include <QFont>
#include <QDebug>
#include <QFontMetrics>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background: " + ColorBoard::black3 + "; border: none;"
                  "color: " + ColorBoard::normalTextLightYellow + ";"
                  "padding-left: 10px;");

    layout = new QHBoxLayout(this);
    textEdit = new QTextEdit(this);
    textEdit->setFont(QFont("Consolas", 14));
    layout->addWidget(textEdit);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    fileName = "";
    isAlreadyCompile = false;


    connect(textEdit, &QTextEdit::textChanged,
            this, [=] {
        isSave = false;
    });

    isSave = true;

    highLighter = new CPPHighLighter(textEdit->document());

    QFontMetrics metrics(textEdit->font());
    textEdit->setTabStopWidth(4 * metrics.width(' '));
}

QTextEdit *Editor::getTextEdit() const
{
    return textEdit;
}

QString Editor::getFileName() const
{
    return fileName;
}

void Editor::setFileName(const QString &value)
{
    fileName = value;
}

bool Editor::getIsSave() const
{
    return isSave;
}

void Editor::setIsSave(bool value)
{
    isSave = value;
}

bool Editor::getIsAlreadyCompile() const
{
    return isAlreadyCompile;
}

void Editor::setIsAlreadyCompile(bool value)
{
    isAlreadyCompile = value;
}
