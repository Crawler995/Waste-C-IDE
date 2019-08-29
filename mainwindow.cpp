#include "mainwindow.h"
#include "features/config.h"
#include "features/colorboard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initWindowSize();

    createMenu();
    createWorkArea();
    createStatusBar();

    initStyle();

    connectSignalAndSlot();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWindowSize() {
    window()->showMaximized();
    QApplication::setFont(QFont("Microsoft Yahei"));
    setMinimumSize(500, 500);
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

    settingMenu = mainMenuBar->addMenu(tr("设置"));
    editorSettingAction = settingMenu->addAction(tr("编辑器设置"));
}

void MainWindow::createStatusBar()
{
    mainStatusBar = this->statusBar();

    cursorPositionLabel = new QLabel(tr("0行0列"), mainStatusBar);
    totalLineNumLabel = new QLabel(tr("共0行"), mainStatusBar);

    mainStatusBar->addPermanentWidget(cursorPositionLabel, 3);
    mainStatusBar->addPermanentWidget(totalLineNumLabel, 4);
}

void MainWindow::createWorkArea()
{
    workArea = new WorkArea(this);
    this->setCentralWidget(workArea);
    resize(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void MainWindow::initStyle()
{
    setStyleSheet(
        "QMainWindow{background-color: " + ColorBoard::black3 + ";}"

        "QMenuBar{background-color: " + ColorBoard::black1 + "; color: " + ColorBoard::lightGray + ";border:none; padding: 0px; font-size: 16px;}"
        "QMenuBar::item{background-color: " + ColorBoard::black1 + "; color: " + ColorBoard::lightGray + ";padding: 3px 12px 3px 12px;}"
        "QMenuBar::item:selected{background-color: " + ColorBoard::black2 + ";}"

        "QMenu{background-color: " + ColorBoard::black1 + ";color: " + ColorBoard::lightGray + ";}"
        "QMenu::item{background-color: " + ColorBoard::black1 + "; color: " + ColorBoard::lightGray + ";padding: 3px 30px 3px 30px; font-size: 16px;}"
        "QMenu::item:selected{background-color: " + ColorBoard::black2 + ";}"
        "QMenu::icon{padding-left: 8px;}"

        "QStatusBar{background-color: " + ColorBoard::blue + ";}"
        "QStatusBar::item{border: none; padding: 0px;}"

        "QScrollBar:vertical{background:transparent; width: 24px;}"
        "QScrollBar::handle:vertical{background:" + ColorBoard::lightGray + ";"
        "border:2px solid transparent;"
        "border-radius:4px;}"
        "QScrollBar::handle:hover{background:" + ColorBoard::blue + ";}"
        "QScrollBar::sub-line:vertical{background:transparent;}"
        "QScrollBar::add-line:vertical{background:transparent;}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "background: none;}"
    );

    cursorPositionLabel->setStyleSheet("color: " + ColorBoard::white + "; padding-left: 6px; font-size: 14px;");
    totalLineNumLabel->setStyleSheet("color: " + ColorBoard::white + "; font-size: 14px;");
}

void MainWindow::connectSignalAndSlot()
{
    connect(workArea->getEditorArea(), &EditorArea::cursorPositionChangedWithPos,
            this, [=](int row, int col, int totalRowNum) {
        cursorPositionLabel->setText(tr("%1行%2列").arg(row).arg(col));
        totalLineNumLabel->setText(tr("共%1行").arg(totalRowNum));
    });

    connect(workArea->getEditorArea(), &EditorArea::createOutputInfo,
            workArea->getRunOutputArea(), &RunOutputArea::outputInfo);
    connect(workArea->getEditorArea(), &EditorArea::createOutputError,
            workArea->getRunOutputArea(), &RunOutputArea::outputError);

    connect(newFileAction, &QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->createEditor();
    });
    connect(openFileAction, &QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->openFile();
    });
    connect(saveFileAction, &QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->saveCurEditorToFile();
    });
    connect(compileAction, &QAction::triggered,
            this, [=]() {
        workArea->getRunOutputArea()->getTextEdit()->clear();
        workArea->getEditorArea()->compileCurFile();
    });
}

