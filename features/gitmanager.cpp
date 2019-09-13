#include "gitmanager.h"

#include <QDebug>
#include <QEventLoop>
#include <QMessageBox>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include "../mainwindow.h"

GitManager::GitManager(QWidget *parent) : QObject(parent)
{
    isCheckGitInstalled = false;

    editorArea = qobject_cast<MainWindow*>(this->parent())->getWorkArea()->getEditorArea();
}

bool GitManager::getIsInstallGit()
{
    if(isCheckGitInstalled) {
        return isGitInstalled;
    }
    QEventLoop loop;
    gitProcess = new QProcess(this);
    connect(gitProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            &loop, &QEventLoop::quit);

    gitProcess->start("git --version");

    connect(gitProcess, &QProcess::readyReadStandardOutput, this, [&]() {
        QString output = QString::fromUtf8(gitProcess->readAllStandardOutput());
        isGitInstalled = output.contains("git version");
        gitProcess->close();
    });
    connect(gitProcess, &QProcess::readyReadStandardError, this, [&]() {
        isGitInstalled = false;
        gitProcess->close();
    });

    loop.exec();
    isCheckGitInstalled = true;

    return isGitInstalled;
}

void GitManager::showGitMessage(const QString &info, bool isSuccessInfo)
{
    QMessageBox box;
    box.setIcon(isSuccessInfo ? QMessageBox::Information : QMessageBox::Critical);
    box.setText(info);
    box.exec();
}

void GitManager::gitInit()
{
    if(!getIsInstallGit()) {
        showGitMessage("本机未安装Git！", false);
        return;
    }

    QString folderPath, fileName;
    if(!getFileNameAndDir(folderPath, fileName)) {
        return;
    }

    executeGitCommand(folderPath, "git init\n");
}

void GitManager::gitAddCurFile()
{
    if(!getIsInstallGit()) {
        showGitMessage("本机未安装Git！", false);
        return;
    }

    QString folderPath, fileName;
    if(!getFileNameAndDir(folderPath, fileName)) {
        return;
    }

    executeGitCommand(folderPath, QString("git add %1\n").arg(fileName));
}

void GitManager::gitAddAllFileInFolder()
{
    if(!getIsInstallGit()) {
        showGitMessage("本机未安装Git！", false);
        return;
    }

    QString folderPath, fileName;
    if(!getFileNameAndDir(folderPath, fileName)) {
        return;
    }

    executeGitCommand(folderPath, QString("git add .\n"));
}

void GitManager::checkoutCurFile()
{
    QString folderPath, fileName;
    if(!getFileNameAndDir(folderPath, fileName)) {
        return;
    }

    if(!getIsInstallGit()) {
        showGitMessage("本机未安装Git！", false);
        return;
    }
    QString dir = folderPath;
    QString cmd = QString("git checkout -- %1\n").arg(fileName);

    gitProcess = new QProcess(this);
    gitProcess->setWorkingDirectory(dir);
    gitProcess->start(cmd);

    emit createOutputInfo(QString("Git command > %1").arg(cmd));
    connect(gitProcess, &QProcess::readyReadStandardOutput, this, [=]() {
        emit createOutputInfo(QString::fromUtf8(gitProcess->readAllStandardOutput()));
    });
    connect(gitProcess, &QProcess::readyReadStandardError, this, [&]() {
        emit createOutputError(QString::fromUtf8(gitProcess->readAllStandardError()));
    });
    connect(gitProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        emit createOutputInfo("Git任务结束。\n\n");
        emit reReadAllFile();
    });
}

void GitManager::checkoutAllFileInFolder()
{
    QString folderPath, fileName;
    if(!getFileNameAndDir(folderPath, fileName)) {
        return;
    }

    if(!getIsInstallGit()) {
        showGitMessage("本机未安装Git！", false);
        return;
    }
    QString dir = folderPath;
    QString cmd = QString("git checkout -- .\n");

    gitProcess = new QProcess(this);
    gitProcess->setWorkingDirectory(dir);
    gitProcess->start(cmd);

    emit createOutputInfo(QString("Git command > %1").arg(cmd));
    connect(gitProcess, &QProcess::readyReadStandardOutput, this, [=]() {
        emit createOutputInfo(QString::fromUtf8(gitProcess->readAllStandardOutput()));
    });
    connect(gitProcess, &QProcess::readyReadStandardError, this, [&]() {
        emit createOutputError(QString::fromUtf8(gitProcess->readAllStandardError()));
    });
    connect(gitProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        emit createOutputInfo("Git任务结束。\n\n");
        emit reReadAllFile();
    });
}

void GitManager::commitFile()
{
    if(!getIsInstallGit()) {
        showGitMessage("本机未安装Git！", false);
        return;
    }

    QString folderPath, fileName;
    if(!getFileNameAndDir(folderPath, fileName)) {
        return;
    }


    QWidget *dialog = new QWidget();
    dialog->setWindowTitle("git commit");
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QComboBox *box = new QComboBox(dialog);
    box->addItems(QStringList() << "feat: A new feature"
                                << "fix: A bug fix"
                                << "build: Changes that affect the build system or external dependencies"
                                << "ci: Changes to our CI configuration files and scripts"
                                << "docs: Documentation only changes"
                                << "perf: A code change that improves performance"
                                << "refactor: A code change that neither fixes a bug nor adds a feature"
                                << "style: Changes that do not affect the meaning of the code"
                                << "test: Adding missing tests or correcting existing tests");

    QLineEdit *commitLineEdit = new QLineEdit(dialog);
    QPushButton *commitButton = new QPushButton("Commit", dialog);
    layout->addWidget(box);
    layout->addWidget(commitLineEdit);
    layout->addWidget(commitButton);
    dialog->setLayout(layout);

    connect(commitButton, &QPushButton::clicked,
            this, [=]() {
        QRegExp regx("^([a-z]+):");
        regx.setMinimal(true);
        const QString commitType = box->currentText();
        regx.indexIn(commitType);

        const QString commitMessage = QString("%1: %2").arg(regx.capturedTexts()[1]).arg(commitLineEdit->text());
        dialog->close();

        executeGitCommand(folderPath, QString("git commit -m \"%1\"\n").arg(commitMessage));
    });

    dialog->show();
}

bool GitManager::getFileNameAndDir(QString &path, QString &fileName)
{
    const QString curFileFullName = editorArea->getCurFileFullName();
    if(curFileFullName == "") {
        return false;
    }

    path = QFileInfo(curFileFullName).absolutePath();
    fileName = QFileInfo(curFileFullName).fileName();

    return true;
}

void GitManager::executeGitCommand(const QString &dir, const QString &cmd)
{
    if(!getIsInstallGit()) {
        showGitMessage("本机未安装Git！", false);
        return;
    }

    gitProcess = new QProcess(this);
    gitProcess->setWorkingDirectory(dir);
    gitProcess->start(cmd);

    emit createOutputInfo(QString("Git command > %1").arg(cmd));
    connect(gitProcess, &QProcess::readyReadStandardOutput, this, [=]() {
        emit createOutputInfo(QString::fromUtf8(gitProcess->readAllStandardOutput()));
    });
    connect(gitProcess, &QProcess::readyReadStandardError, this, [&]() {
        emit createOutputError(QString::fromUtf8(gitProcess->readAllStandardError()));
    });
    connect(gitProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        emit createOutputInfo("Git任务结束。\n\n");
    });
}
