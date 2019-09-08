#include "editorarea.h"
#include "features/colorboard.h"
#include "welcomepage.h"
#include "findorreplacedialog.h"

#include <QDebug>
#include <QTextBlock>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QFontDialog>
#include <QMessageBox>

EditorArea::EditorArea(QWidget *parent) : QTabWidget(parent)
{
    defaultFont = QFont("consolas", 14);

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

QFont EditorArea::getDefaultFont() const
{
    return defaultFont;
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

void EditorArea::createEditorWithTemp()
{
    createEditor();
    editors[this->currentIndex()]->getTextEdit()->insertPlainText("#include <stdio.h>");
}

void EditorArea::saveCurEditorToFile()
{
    if(this->count() == 0 || qobject_cast<WelcomePage*>(this->currentWidget())) {
        QMessageBox box;
        box.setIcon(QMessageBox::Critical);
        box.setText("当前无打开文件！");
        box.exec();
        return;
    }

    Editor *editor = editors[currentIndex()];

    if(editor->getFileName() == "") {
        QString saveFileName = QFileDialog::getSaveFileName(this, tr("保存C语言源文件"),
                                                            "untitled.c",
                                                            "C Source files (*.c)");
        if(saveFileName == "") {
            return;
        }
        editor->setFileName(saveFileName);
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
    if(this->count() == 0 || qobject_cast<WelcomePage*>(this->currentWidget())) {
        QMessageBox box;
        box.setIcon(QMessageBox::Critical);
        box.setText("当前无打开文件！");
        box.exec();
        return;
    }

    FindOrReplaceDialog *dialog = new FindOrReplaceDialog();

    connect(dialog->getFindButton(), &QPushButton::clicked,
            this, [=]() {
        editors[this->currentIndex()]->findWordAndHighLight(dialog->getInputWordEdit()->text(),
                                                            dialog->getIsRegexMatch(),
                                                            dialog->getIsCaseSensitive());
    });
    connect(dialog->getReplaceButton(), &QPushButton::clicked,
            this, [=]() {
        editors[this->currentIndex()]->replaceWordAndHighLight(dialog->getInputWordEdit()->text(),
                                                               dialog->getTargetWordEdit()->text(),
                                                               dialog->getIsRegexMatch(),
                                                               dialog->getIsCaseSensitive());
    });

    connect(dialog, &QWidget::destroyed,
            this, [=]() {
        editors[this->currentIndex()]->clearHighLightOfFoundWord();
    });

    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
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

void EditorArea::compileRunCurFile()
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
        emit createOutputInfo("\n终端任务结束。\n");

        runCurFile();
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

void EditorArea::openSettingDialog()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, defaultFont, this);

    if (ok) {
        foreach(Editor *editor, editors) {
            editor->getTextEdit()->setFont(font);
        }
        defaultFont = font;
    }
}

void EditorArea::startDebug(QVector<int> breakPointLines)
{
    qDebug() << "debug";
    Editor *editor = editors[currentIndex()];

    if(!editor->getIsSave()) {
        emit createOutputInfo("文件未保存！\n");
        return;
    }

    QProcess *process = new QProcess(this);
    QString exeFilePath = QString(editor->getFileName()).replace(".c", ".exe");

    QString cmd = QDir::toNativeSeparators(tr("gcc %1 -g -o %2\n").arg(editor->getFileName()).arg(exeFilePath));

    process->start(cmd);

    emit createOutputInfo("> " + cmd);
    editor->setIsAlreadyCompile(true);

    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        emit createOutputInfo(QString::fromUtf8(process->readAllStandardOutput()));
    });
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        emit createOutputError(QString::fromUtf8(process->readAllStandardError()));
        editor->setIsAlreadyCompile(false);
    });
    connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        process->close();

        QProcess *debugProcess = new QProcess(this);
        debugProcess->start(QString("gdb %1").arg(exeFilePath));

        connect(debugProcess, &QProcess::readyReadStandardOutput, this, [=]() {
            emit createOutputInfo(QString::fromUtf8(debugProcess->readAllStandardOutput()));
        });

        foreach (int line, breakPointLines) {
            debugProcess->write(QString("break %1\n").arg(line).toStdString().c_str());
        }

        debugProcess->write("run\n");
    });
}
