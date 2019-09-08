#ifndef EDITORAREA_H
#define EDITORAREA_H

#include "editor.h"

#include <QWidget>
#include <QTabWidget>
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

    void startDebug(QVector<int> breakPointLines);

    QFont getDefaultFont() const;

private:
    void initStyle();
    QString getCurEditorText();

    QVector<Editor*> editors;
    QFont defaultFont;

signals:
    void cursorPositionChangedWithPos(int row, int col, int totalRowNum);
    void createOutputInfo(const QString &info);
    void createOutputError(const QString &error);
};

#endif // EDITORAREA_H
