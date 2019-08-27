#ifndef EDITORAREA_H
#define EDITORAREA_H

#include "editor.h"

#include <QWidget>
#include <QTabWidget>

class EditorArea : public QTabWidget
{
    Q_OBJECT
public:
    explicit EditorArea(QWidget *parent = 0);

private:
    void initStyle();
    Editor *editor1, *editor2;
};

#endif // EDITORAREA_H
