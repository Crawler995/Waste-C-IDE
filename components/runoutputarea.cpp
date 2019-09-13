#include "runoutputarea.h"
#include "features/colorboard.h"

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

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

    textEditWidget = new QWidget(this);
    textEditLayout = new QHBoxLayout(textEditWidget);
    textEdit = new QTextEdit(this);
    inputTextEdit = new QTextEdit(this);

    textEditLayout->addWidget(textEdit, 2);
    textEditLayout->addWidget(inputTextEdit, 1);
    textEditWidget->setLayout(textEditLayout);

    layout->addWidget(titleWidget);
    layout->addWidget(textEditWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    titleWidget->setStyleSheet("color: " + ColorBoard::lightGray + ";"
                                  "border-top: 2px solid " + ColorBoard::black2 + ";"
                                  "padding: 10px 0px 10px 10px;"
                                  "font-size: 16px;");
    textEdit->setStyleSheet("border: none;"
                            "background: " + ColorBoard::black3 + ";"
                            "color: " + ColorBoard::lightGray + ";"
                            "padding-left: 10px;");
    inputTextEdit->setStyleSheet("border: none;"
                                 "border-left: 2px solid " + ColorBoard::black2 + ";"
                                 "background: " + ColorBoard::black3 + ";"
                                 "color: " + ColorBoard::lightGray + ";"
                                 "padding-left: 10px;");
    textEdit->setReadOnly(true);
    inputTextEdit->setPlaceholderText("如果你的程序需要键盘输入，就在这输入8");
    inputTextEdit->installEventFilter(this);
    setLayout(layout);
}

void RunOutputArea::outputInfo(const QString &info)
{
    textEdit->moveCursor(QTextCursor::End);
    QTextCharFormat format;
    format.setForeground(QColor(ColorBoard::lightGray));
    textEdit->textCursor().insertText(info, format);
    textEdit->moveCursor(QTextCursor::End);
}

void RunOutputArea::outputError(const QString &error)
{
    textEdit->moveCursor(QTextCursor::End);
    QTextCharFormat format;
    format.setForeground(QColor(255, 0, 0, 255));
    textEdit->textCursor().insertText(error, format);
    textEdit->moveCursor(QTextCursor::End);
}

bool RunOutputArea::eventFilter(QObject *watched, QEvent *event)
{
    if(qobject_cast<QTextEdit*>(watched) == inputTextEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if(e->key() == Qt::Key_Return) {
            QTextCursor cursor = inputTextEdit->textCursor(), initCursor = cursor;
            cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
            QString curLineData = cursor.selectedText();
            inputTextEdit->setTextCursor(initCursor);

            emit userInputData(curLineData);

            return QWidget::eventFilter(watched, event);
        }

        else {
            return QWidget::eventFilter(watched, event);
        }
    }

    return QWidget::eventFilter(watched, event);;
}

ActionButton *RunOutputArea::getCompileRunButton() const
{
    return compileRunButton;
}

void RunOutputArea::setCompileRunButton(ActionButton *value)
{
    compileRunButton = value;
}

ActionButton *RunOutputArea::getRunButton() const
{
    return runButton;
}

void RunOutputArea::setRunButton(ActionButton *value)
{
    runButton = value;
}

ActionButton *RunOutputArea::getCompileButton() const
{
    return compileButton;
}

void RunOutputArea::setCompileButton(ActionButton *value)
{
    compileButton = value;
}

QTextEdit *RunOutputArea::getTextEdit() const
{
    return textEdit;
}
