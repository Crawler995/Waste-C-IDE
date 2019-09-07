#include "ccompleter.h"

CCompleter::CCompleter(QObject *parent) :
    QCompleter(QStringList() << "short" << "int" << "float" << "double"
                             << "long" << "char" << "enum" << "signed"
                             << "struct" << "union" << "void"
                             << "for" << "do" << "while" << "break" << "continue"
                             << "if" << "else" << "goto"
                             << "switch" << "case" << "default"
                             << "return" << "auto" << "extern" << "register" << "static"
                             << "const" << "sizeof" << "typedef"
                             << "stdio.h" << "stdlib.h" << "string.h"
                             << "main" << "include" << "define", parent)
{
}
