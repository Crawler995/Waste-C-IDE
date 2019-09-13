#include "gitmanager.h"

#include <QDebug>
#include <QEventLoop>
#include <QMessageBox>
#include <QFileInfo>
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
