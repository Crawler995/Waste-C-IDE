#include "mainwindow.h"
#include "features/config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initWindowSize();
    initStyle();
    createMenu();
    createStatusBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWindowSize() {
    resize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void MainWindow::createMenu() {
    mainMenuBar = this->menuBar();

    fileMenu = mainMenuBar->addMenu(tr("文件"));
    newFileAction = fileMenu->addAction(tr("新建C语言源文件"));
    openFileAction = fileMenu->addAction(tr("打开C语言源文件"));
    saveFileAction = fileMenu->addAction(tr("保存"));

    editMenu = mainMenuBar->addMenu(tr("编辑"));
    findAction = editMenu->addAction(tr("查找"));
    replaceAction = editMenu->addAction(tr("替换"));

    runMenu = mainMenuBar->addMenu(tr("运行"));
    compileAction = runMenu->addAction(tr("编译"));
    runAction = runMenu->addAction(tr("运行"));
    compileRunAction = runMenu->addAction(tr("编译运行"));
}

void MainWindow::createStatusBar()
{
    mainStatusBar = this->statusBar();

    cursorPositionLabel = new QLabel(tr("0行0列"), mainStatusBar);
    totalLineNumLabel = new QLabel(tr("共99行"));

    mainStatusBar->addPermanentWidget(cursorPositionLabel, 3);
    mainStatusBar->addWidget(totalLineNumLabel, 4);
}

void MainWindow::initStyle()
{
    setStyleSheet(
        "QMainWindow{background-color: #ffffff;}"

        "QMenuBar{background-color: #e8e8e8; border:none; padding: 0px;}"
        "QMenuBar::item{background-color: #e8e8e8; padding: 3px 10px 3px 10px;}"
        "QMenuBar::item:selected{background-color: #ffffff;}"

        "QMenu{background-color: #ffffff;}"
        "QMenu::item{background-color: #ffffff; padding: 3px 30px 3px 30px;}"
        "QMenu::item:selected{background-color: #e8e8e8; color: #000000;}"
        "QMenu::icon{padding-left: 8px;}"

        "QToolBar{border: 2px solid #e8e8e8; padding: 0px;}"
        "QToolButton{padding: 3px 5px 3px 5px; background-color: #ffffff;border: none;}"
        "QToolButton:hover{background-color: #e8e8e8;}"

        "QScrollArea{background-color: #ffffff; border: 2px solid #e8e8e8;}"

        "QStatusBar{background-color: #e8e8e8;}"
        "QStatusBar::item{border: none;}"

        "QPushButton{background-color: #ffffff; border: 2px solid #e8e8e8; width: 76px; height: 25px;}"
        "QPushButton:hover{background-color: #e8e8e8;}"

        "QDialog{background-color: #ffffff;}"

        "QLineEdit{background-color: #ffffff; border: 2px solid #e8e8e8;}"

        "QProgressBar{background-color: #ffffff; border: 2px solid #e8e8e8; text-align: center;}"
        "QProgressBar:chunk{background-color: #e8e8e8;}"

        "QMessageBox{background-color: #ffffff;}"
    );
}
