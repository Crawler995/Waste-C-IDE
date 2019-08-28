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
    void saveCurEditorToFile();

private:
    void initStyle();
    QString getCurEditorText();

    QVector<Editor*> editors;

signals:
    void cursorPositionChangedWithPos(int row, int col, int totalRowNum);
};

#endif // EDITORAREA_H
