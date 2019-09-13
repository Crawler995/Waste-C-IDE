#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "components/workarea.h"
#include "features/gitmanager.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QStatusBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    WorkArea *getWorkArea() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void initWindowSize();
    void createMenu();
    void createStatusBar();
    void createWorkArea();
    void initStyle();

    void connectSignalAndSlot();

    QMenuBar *mainMenuBar;

    QMenu *fileMenu;
    QMenu *newFileMenu;
    QAction *newFileAction, *newFileWithTempAction, *openFileAction, *saveFileAction;

    QMenu *editMenu;
    QAction *findReplaceAction, *manageCommentAction;

    QMenu *runMenu;
    QAction *compileAction, *runAction, *compileRunAction, *debugAction;

    QMenu *settingMenu;
    QAction *editorSettingAction;

    QMenu *gitMenu;
    QAction *gitInitAction;
    QMenu *addMenu;
    QAction *addCurFileAction, *addAllFileInFolderAction;
    QMenu *checkoutMenu;
    QAction *checkoutCurFileAction, *checkoutAllFileInFolderAction;
    QAction *commitAction;

    QStatusBar *mainStatusBar;
    QLabel *cursorPositionLabel, *totalLineNumLabel;

    WorkArea *workArea;

    GitManager *gitManager;
};

#endif // MAINWINDOW_H
