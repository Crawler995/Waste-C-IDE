#include "actionbutton.h"
#include "features/colorboard.h"

ActionButton::ActionButton(const QString &text, QWidget *parent) : QPushButton(parent)
{
    this->setStyleSheet("QPushButton{color: " + ColorBoard::lightGray + "; border: 1px solid " + ColorBoard::blue + ";"
                        "padding: 6px; margin: 6px; border-radius: 2px;}"
                        "QPushButton:hover{color: " + ColorBoard::white + ";}");
    this->setText(text);
}
