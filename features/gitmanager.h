#ifndef GITMANAGER_H
#define GITMANAGER_H

#include <QObject>
#include <QProcess>
#include "../components/editorarea.h"

class GitManager : public QObject
{
    Q_OBJECT
public:
    explicit GitManager(QWidget *parent = 0);

    bool getIsInstallGit();
    void showGitMessage(const QString &info, bool isSuccessInfo);

    void gitInit();

    void gitAddCurFile();
    void gitAddAllFileInFolder();

    void checkoutCurFile();
    void checkoutAllFileInFolder();

    void commitFile();

private:
    QProcess *gitProcess;
    bool isCheckGitInstalled;
    bool isGitInstalled;

    EditorArea *editorArea;

    bool getFileNameAndDir(QString &path, QString &fileName);
    void executeGitCommand(const QString &dir, const QString &cmd);

signals:
    void createOutputInfo(const QString &info);
    void createOutputError(const QString &error);
    void reReadAllFile();
};

#endif // GITMANAGER_H
