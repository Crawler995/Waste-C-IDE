#ifndef WORKAREA_H
#define WORKAREA_H

#include "editorarea.h"
#include "debuginfoarea.h"
#include "runoutputarea.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>

class WorkArea : public QSplitter
{
    Q_OBJECT
public:
    explicit WorkArea(QWidget *parent = 0);

    EditorArea *getEditorArea() const;

    RunOutputArea *getRunOutputArea() const;

private:
    void initStyle();
    void setVStretchFactor(QWidget *widget, int factor);
    void setHStretchFactor(QWidget *widget, int factor);

    QSplitter *vSplitter, *hSplitter;
    DebugInfoArea *debugInfoArea;
    EditorArea *editorArea;
    RunOutputArea *runOutputArea;

    QVBoxLayout *layout;
};

#endif // WORKAREA_H
