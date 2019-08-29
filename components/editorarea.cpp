#include "editorarea.h"
#include "features/colorboard.h"
#include "welcomepage.h"

#include <QDebug>
#include <QTextBlock>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QProcess>

EditorArea::EditorArea(QWidget *parent) : QTabWidget(parent)
{
    WelcomePage *page = new WelcomePage(this);
    this->insertTab(0, page, "欢迎");

    filePath = "";

    setTabsClosable(true);
    setMovable(true);

    connect(this, &QTabWidget::tabCloseRequested,
            this, [=](int index) {
        this->removeTab(index);
    });
}

QTextEdit *EditorArea::getCurEditor()
{
    return editors.at(this->currentIndex())->getTextEdit();
}

void EditorArea::initStyle()
{

}

QString EditorArea::getCurEditorText()
{
    return getCurEditor()->document()->toPlainText();
}

void EditorArea::createEditor()
{
    if(qobject_cast<WelcomePage*>(this->currentWidget())) {
        this->removeTab(0);
    }

    Editor *editor = new Editor(this);
    editors.append(editor);

    this->insertTab(0, editor, tr("untitled.c"));
    this->setCurrentIndex(0);

    connect(getCurEditor(), &QTextEdit::cursorPositionChanged,
            this, [=] {
        QTextCursor cursor = getCurEditor()->textCursor();
        int col = cursor.columnNumber() + 1;
        int row = cursor.block().layout()->lineForTextPosition(col).lineNumber() +
                  cursor.block().firstLineNumber() + 1;
        int totalRowNum = getCurEditor()->document()->lineCount();
        emit cursorPositionChangedWithPos(row, col, totalRowNum);
    });
}

void EditorArea::saveCurEditorToFile()
{
    if(qobject_cast<WelcomePage*>(this->currentWidget())) {
        return;
    }

    if(filePath == "") {
        filePath = QFileDialog::getSaveFileName(this, tr("保存C语言源文件"),
                                                        "untitled.c",
                                                        "C Source files (*.c)");
    }

    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream s(&file);
        s << getCurEditorText();
    }
    file.close();

    this->setTabText(this->currentIndex(), QFileInfo(filePath).fileName());
}

void EditorArea::compileCurFile()
{
    QProcess *process = new QProcess(this);
    QString exeFilePath = QString(filePath).replace(".c", ".exe");

    QString cmd = QDir::toNativeSeparators(tr("gcc %1 -o %2\n").arg(filePath).arg(exeFilePath));

    process->start(cmd);

    emit createOutputInfo("> " + cmd);

    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        emit createOutputInfo(QString::fromUtf8(process->readAllStandardOutput()));

        process->close();
    });
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        emit createOutputError(QString::fromUtf8(process->readAllStandardError()));

        process->close();
    });
    connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        emit createOutputInfo(exitStatus == QProcess::NormalExit ? "终端任务顺利完成。"
                                                                 : "终端任务异常退出。");
    });
}
