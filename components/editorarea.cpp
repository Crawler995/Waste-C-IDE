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

    setTabsClosable(true);
    setMovable(false);

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
    editors.insert(0, editor);

    this->insertTab(0, editor, tr("untitled.c"));
    this->setCurrentIndex(0);

    connect(editor->getTextEdit(), &QTextEdit::cursorPositionChanged,
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

    Editor *editor = editors[currentIndex()];

    if(editor->getFileName() == "") {
        editor->setFileName(QFileDialog::getSaveFileName(this, tr("保存C语言源文件"),
                                                        "untitled.c",
                                                        "C Source files (*.c)"));
    }

    QFile file(editor->getFileName());
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream s(&file);
        s << getCurEditorText();
    }
    file.close();

    this->setTabText(this->currentIndex(), QFileInfo(editor->getFileName()).fileName());

    editor->setIsSave(true);
}

void EditorArea::findWord()
{

}

void EditorArea::compileCurFile()
{
    Editor *editor = editors[currentIndex()];

    if(!editor->getIsSave()) {
        emit createOutputInfo("文件未保存！");
        return;
    }

    QProcess *process = new QProcess(this);
    QString exeFilePath = QString(editor->getFileName()).replace(".c", ".exe");

    QString cmd = QDir::toNativeSeparators(tr("gcc %1 -o %2\n").arg(editor->getFileName()).arg(exeFilePath));

    process->start(cmd);

    emit createOutputInfo("> " + cmd);
    editor->setIsAlreadyCompile(true);

    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        emit createOutputInfo(QString::fromUtf8(process->readAllStandardOutput()));

        process->close();
    });
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        emit createOutputError(QString::fromUtf8(process->readAllStandardError()));
        editor->setIsAlreadyCompile(false);
        process->close();
    });
    connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        emit createOutputInfo("\n终端任务结束。");
    });
}

void EditorArea::runCurFile()
{
    Editor *editor = editors[currentIndex()];

    if(!editor->getIsAlreadyCompile()) {
        emit createOutputError("文件未编译，请先进行编译再运行！\r\n");
        return;
    }

    QProcess *process = new QProcess(this);
    QString cmd = QString(editor->getFileName() + "\n").replace(".c", ".exe");
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
        emit createOutputInfo("\n终端任务结束。");
    });
}

void EditorArea::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开C语言源文件"),
                                                    "/",
                                                    "C Source files (*.c)");
    if(fileName == "") {
        return;
    }
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        createEditor();
        this->setTabText(this->currentIndex(), QFileInfo(fileName).fileName());
        getCurEditor()->setText(QString::fromUtf8(file.readAll()));
    }

    file.close();

    editors[currentIndex()]->setIsSave(true);
    editors[currentIndex()]->setFileName(fileName);
}
