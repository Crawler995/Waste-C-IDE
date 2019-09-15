#include "mainwindow.h"
#include "features/config.h"
#include "features/colorboard.h"
#include <QInputDialog>
#include <QVector>
#include <QPair>
#include <QStandardItemModel>
#include <QMimeData>
#include <QUrl>
#include <QDebug>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initWindowSize();

    createMenu();
    createWorkArea();
    createStatusBar();

    initStyle();

    gitManager = new GitManager(this);

    connectSignalAndSlot();

    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWindowSize() {
    window()->showMaximized();
    QApplication::setFont(QFont("Microsoft Yahei"));
    setMinimumSize(900, 700);
}

void MainWindow::createMenu() {
    mainMenuBar = this->menuBar();

    fileMenu = mainMenuBar->addMenu(tr("文件"));
    newFileMenu = fileMenu->addMenu(tr("新建"));
    newFileAction = newFileMenu->addAction(tr("新建空白C语言源文件"));
    newFileAction->setShortcut(QKeySequence(QLatin1String("Ctrl+N")));
    newFileWithTempAction = newFileMenu->addAction(tr("新建带main函数的C语言源文件"));
    openFileAction = fileMenu->addAction(tr("打开C语言源文件"));
    openFileAction->setShortcut(QKeySequence(QLatin1String("Ctrl+O")));
    saveFileAction = fileMenu->addAction(tr("保存"));
    saveFileAction->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));

    editMenu = mainMenuBar->addMenu(tr("编辑"));
    findReplaceAction = editMenu->addAction(tr("查找/替换"));
    findReplaceAction->setShortcut(QKeySequence(QLatin1String("Ctrl+F")));
    manageCommentAction = editMenu->addAction(tr("显示/隐藏注释"));
    manageCommentAction->setShortcut(QKeySequence(QLatin1String("Ctrl+H")));

    runMenu = mainMenuBar->addMenu(tr("运行"));
    compileAction = runMenu->addAction(tr("编译"));
    runAction = runMenu->addAction(tr("运行"));
    compileRunAction = runMenu->addAction(tr("编译运行"));
    compileRunAction->setShortcut(QKeySequence(QLatin1String("Ctrl+R")));

    settingMenu = mainMenuBar->addMenu(tr("设置"));
    editorSettingAction = settingMenu->addAction(tr("编辑器设置"));

    gitMenu = mainMenuBar->addMenu(tr("Git管理"));
    gitInitAction = gitMenu->addAction(tr("init"));
    addMenu = gitMenu->addMenu(tr("add..."));
    addCurFileAction = addMenu->addAction(tr("add <this file>"));
    addAllFileInFolderAction = addMenu->addAction(tr("add <all file in this folder>"));
    checkoutMenu = gitMenu->addMenu(tr("checkout -- ...（撤销暂存区修改）"));
    checkoutCurFileAction = checkoutMenu->addAction(tr("checkout -- <this file>"));
    checkoutAllFileInFolderAction = checkoutMenu->addAction(tr("checkout -- <all file in this folder>"));
    commitAction = gitMenu->addAction(tr("commit"));
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
    connect(newFileWithTempAction, &QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->createEditorWithTemp();
    });
    connect(openFileAction, &QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->openFile();
    });
    connect(saveFileAction, &QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->saveCurEditorToFile();
    });

    connect(findReplaceAction, &QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->findWord();
    });
    connect(manageCommentAction, QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->manageComment();
    });

    connect(compileAction, &QAction::triggered,
            this, [=]() {
        workArea->getRunOutputArea()->getTextEdit()->clear();
        workArea->getEditorArea()->compileCurFile();
    });
    connect(runAction, &QAction::triggered,
            this, [=]() {
        workArea->getRunOutputArea()->getTextEdit()->clear();
        workArea->getEditorArea()->runCurFile();
    });
    connect(compileRunAction, &QAction::triggered,
            this, [=]() {
        workArea->getRunOutputArea()->getTextEdit()->clear();
        workArea->getEditorArea()->compileRunCurFile();
    });
    connect(workArea->getRunOutputArea()->getCompileButton(), &QPushButton::clicked,
            this, [=]() {
        workArea->getRunOutputArea()->getTextEdit()->clear();
        workArea->getEditorArea()->compileCurFile();
    });
    connect(workArea->getRunOutputArea()->getRunButton(), &QPushButton::clicked,
            this, [=]() {
        workArea->getRunOutputArea()->getTextEdit()->clear();
        workArea->getEditorArea()->runCurFile();
    });
    connect(workArea->getRunOutputArea()->getCompileRunButton(), &QPushButton::clicked,
            this, [=]() {
        workArea->getRunOutputArea()->getTextEdit()->clear();
        workArea->getEditorArea()->compileRunCurFile();
    });

    connect(editorSettingAction, &QAction::triggered,
            this, [=]() {
        workArea->getEditorArea()->openSettingDialog();
    });

    // debug
    connect(workArea->getDebugInfoArea()->getAddBreakPointButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }

        QTextCursor cursor = workArea->getEditorArea()->getCurEditor()->textCursor();
        int col = cursor.columnNumber() + 1;
        int row = cursor.block().layout()->lineForTextPosition(col).lineNumber() +
                  cursor.block().firstLineNumber() + 1;
        workArea->getEditorArea()->addBreakPointLine(row);

        workArea->getEditorArea()->highLightBreakPointLine();
    });
    connect(workArea->getDebugInfoArea()->getDebugButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->startDebug();
    });
    connect(workArea->getDebugInfoArea()->getAddWatchButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        QInputDialog dialog;
        dialog.setWindowTitle("添加查看");
        dialog.setLabelText("输入变量名");
        if(dialog.exec() == QInputDialog::Accepted) {
            workArea->getEditorArea()->executeGDBCommand("display " + dialog.textValue());
            workArea->getEditorArea()->appendItem(dialog.textValue(), "");
        }
    });
    connect(workArea->getDebugInfoArea()->getNextStepButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->executeGDBCommand("next");
    });
    connect(workArea->getDebugInfoArea()->getJumpButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->executeGDBCommand("continue");
    });
    connect(workArea->getDebugInfoArea()->getNextSentenceButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->executeGDBCommand("nexti");
    });
    connect(workArea->getDebugInfoArea()->getStopButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->executeGDBCommand("kill");
        workArea->getEditorArea()->executeGDBCommand("y");
        workArea->getEditorArea()->executeGDBCommand("quit");
        workArea->getEditorArea()->clearHighLightCurRunLine();
        workArea->getEditorArea()->clearBreakPoints();
        workArea->getEditorArea()->clearVarInfo();
    });
    connect(workArea->getDebugInfoArea()->getSingleStepEnterButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->executeGDBCommand("step");
    });
    connect(workArea->getDebugInfoArea()->getJumpFuncButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->executeGDBCommand("finish");
    });
    connect(workArea->getDebugInfoArea()->getEnterSentenceButton(), &QPushButton::clicked,
            this, [=]() {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->executeGDBCommand("stepi");
    });

    connect(workArea->getRunOutputArea(), &RunOutputArea::userInputData,
            this, [=](const QString &data) {
        if(!workArea->getEditorArea()->confirmOperationWithOpenFile()) {
            return;
        }
        workArea->getEditorArea()->writeUserInputData(data);
    });

    connect(workArea->getEditorArea(), &EditorArea::varInfoModelUpdated,
            this, [=](QStandardItemModel *model) {
        workArea->getDebugInfoArea()->setModel(model);
    });

    connect(gitManager, &GitManager::createOutputInfo,
            workArea->getRunOutputArea(), &RunOutputArea::outputInfo);
    connect(gitManager, &GitManager::createOutputError,
            workArea->getRunOutputArea(), &RunOutputArea::outputError);

    connect(gitInitAction, &QAction::triggered,
            this, [=]() {
        gitManager->gitInit();
    });
    connect(addCurFileAction, &QAction::triggered,
            this, [=]() {
        gitManager->gitAddCurFile();
    });
    connect(addAllFileInFolderAction, &QAction::triggered,
            this, [=]() {
        gitManager->gitAddAllFileInFolder();
    });
    connect(checkoutCurFileAction, &QAction::triggered,
            this, [=]() {
        gitManager->checkoutCurFile();
    });
    connect(checkoutAllFileInFolderAction, &QAction::triggered,
            this, [=]() {
        gitManager->checkoutAllFileInFolder();
    });
    connect(gitManager, &GitManager::reReadAllFile,
            this, [=]() {
        workArea->getEditorArea()->reReadAllFile();
    });
    connect(commitAction, &QAction::triggered,
            this, [=]() {
        gitManager->commitFile();
    });
}

WorkArea *MainWindow::getWorkArea() const
{
    return workArea;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    foreach(QUrl url, urls) {
        QString fileName = url.toLocalFile();
        if(QFileInfo(fileName).completeSuffix() == "c") {
            workArea->getEditorArea()->openFile(fileName);
        }
    }
}

