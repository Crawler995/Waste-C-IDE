#ifndef EDITORAREA_H
#define EDITORAREA_H

#include "editor.h"

#include <QWidget>
#include <QTabWidget>
#include <QProcess>
#include <QVector>

class EditorArea : public QTabWidget
{
    Q_OBJECT
public:
    explicit EditorArea(QWidget *parent = 0);

    QTextEdit* getCurEditor();
    void createEditor();
    void createEditorWithTemp();
    void saveCurEditorToFile();

    void findWord();

    void compileCurFile();
    void runCurFile();
    void compileRunCurFile();
    void openFile();

    void openSettingDialog();

    void executeGDBCommand(const QString &command);
    void startDebug();

    QFont getDefaultFont() const;

    void highLightBreakPointLine();
    void highLightCurRunLine(int line);
    void clearVarInfo();
    void appendItem(const QString &name, const QString &value);
    void addBreakPointLine(int line);
    void clearHighLightCurRunLine();
    void clearBreakPoints();

    void parseGDBOutput(const QString &output, QString &curRunLine, QVector<QPair<QString, QString> > &varInfo);

    void writeUserInputData(const QString &data);

private:
    void initStyle();
    QString getCurEditorText();

    QVector<Editor*> editors;
    QFont defaultFont;

    QProcess *debugProcess, *process;
    bool isDebuging;


signals:
    void cursorPositionChangedWithPos(int row, int col, int totalRowNum);
    void createOutputInfo(const QString &info);
    void createOutputError(const QString &error);

    void captureVarInfo(const QVector<QPair<QString, QString> > &varInfo);
    void varInfoModelUpdated(QStandardItemModel *model);
};

#endif // EDITORAREA_H
