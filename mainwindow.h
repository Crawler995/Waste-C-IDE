#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void initStyle();

    QMenuBar *mainMenuBar;

    QMenu *fileMenu;
    QAction *newFileAction, *openFileAction, *saveFileAction;

    QMenu *editMenu;
    QAction *findAction, *replaceAction;

    QMenu *runMenu;
    QAction *compileAction, *runAction, *compileRunAction, *debugAction;

    QStatusBar *mainStatusBar;
    QLabel *cursorPositionLabel, *totalLineNumLabel;
};

#endif // MAINWINDOW_H
