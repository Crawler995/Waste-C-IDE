#include "ccompleter.h"

CCompleter::CCompleter(QObject *parent) :
    QCompleter(QStringList() << "char" << "class" << "const"
                             << "double" << "enum" << "explicit"
                             << "friend" << "inline" << "int"
                             << "long" << "namespace" << "operator"
                             << "private" << "protected" << "public"
                             << "short" << "signals" << "signed"
                             << "slots" << "static" << "struct"
                             << "template" << "typedef" << "typename"
                             << "union" << "unsigned" << "virtual"
                             << "void" << "volatile" << "\\include"
                             << "ifdef" << "define" << "endif"
                             << "if" << "else" << "return",
        parent)
{

}
