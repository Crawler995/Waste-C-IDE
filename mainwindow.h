#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "components/workarea.h"

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

    QStatusBar *mainStatusBar;
    QLabel *cursorPositionLabel, *totalLineNumLabel;

    WorkArea *workArea;
};

#endif // MAINWINDOW_H
