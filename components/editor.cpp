#include "editor.h"
#include "features/colorboard.h"

#include <QFont>
#include <QDebug>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background: " + ColorBoard::black3 + "; border: none;"
                  "color: " + ColorBoard::lightGray + ";"
                  "padding-left: 10px;");

    layout = new QHBoxLayout(this);
    textEdit = new QTextEdit(this);
    textEdit->setFont(QFont("Consolas", 14));
    layout->addWidget(textEdit);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

QTextEdit *Editor::getTextEdit() const
{
    return textEdit;
}
