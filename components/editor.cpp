#include "editor.h"
#include "features/colorboard.h"

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background: " + ColorBoard::black3 + "; border: none;"
                  "color: " + ColorBoard::lightGray + ";"
                  "font-size: 24px;"
                  "padding-left: 10px;");

    layout = new QHBoxLayout(this);
    textEdit = new QTextEdit(this);
    layout->addWidget(textEdit);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}
