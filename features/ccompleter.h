#ifndef CCOMPLETER_H
#define CCOMPLETER_H

#include <QCompleter>

class CCompleter : public QCompleter
{
    Q_OBJECT
public:
    explicit CCompleter(QObject *parent = 0);
};

#endif // CCOMPLETER_H
