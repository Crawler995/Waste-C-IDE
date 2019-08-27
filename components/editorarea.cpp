#include "editorarea.h"
#include "features/colorboard.h"

EditorArea::EditorArea(QWidget *parent) : QTabWidget(parent)
{
    editor1 = new Editor(this);
    this->insertTab(0, editor1, tr("test.c"));
//    editor2 = new Editor(this);
//    this->insertTab(1, editor2, tr("1.c"));

    setTabsClosable(true);
    setMovable(true);
}

void EditorArea::initStyle()
{

}
