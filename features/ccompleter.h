#ifndef CCOMPLETER_H
#define CCOMPLETER_H

#include <QCompleter>
#include <QStringListModel>

class CCompleter : public QCompleter
{
    Q_OBJECT
public:
    explicit CCompleter(QObject *parent = 0);

    void addCompleteItem(const QString &item);

private:
    QStringListModel *model;
    QStringList completeList;
};

#endif // CCOMPLETER_H
