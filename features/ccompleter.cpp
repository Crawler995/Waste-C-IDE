#include "ccompleter.h"
#include <QDebug>

CCompleter::CCompleter(QObject *parent) :
    QCompleter(parent)
{
    model = new QStringListModel(this);
    completeList = QStringList() << "short" << "int" << "float" << "double"
                                 << "long" << "char" << "enum" << "signed"
                                 << "struct" << "union" << "void"
                                 << "for" << "do" << "while" << "break" << "continue"
                                 << "if" << "else" << "goto"
                                 << "switch" << "case" << "default"
                                 << "return" << "auto" << "extern" << "register" << "static"
                                 << "const" << "sizeof" << "typedef"
                                 << "stdio.h" << "stdlib.h" << "string.h"
                                 << "main" << "include" << "define";

    model->setStringList(completeList);
    this->setModel(model);
}

void CCompleter::addCompleteItem(const QString &item)
{
    if(completeList.contains(item)) {
        return;
    }
    completeList.append(item);

    QStringListModel *newModel = new QStringListModel(this);
    newModel->setStringList(completeList);
    this->setModel(newModel);

    model = newModel;
}
