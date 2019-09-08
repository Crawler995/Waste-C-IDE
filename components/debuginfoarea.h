#ifndef DEBUGINFOAREA_H
#define DEBUGINFOAREA_H

#include "actionbutton.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeView>
#include <QStandardItemModel>
#include <QVector>

class DebugInfoArea : public QWidget
{
    Q_OBJECT
public:
    explicit DebugInfoArea(QWidget *parent = 0);
    void addBreakPointLine(int line);

    ActionButton *getDebugButton() const;

    ActionButton *getAddBreakPointButton() const;

    QVector<int> getBreakPointLines() const;

private:
    QLabel *areaTitleLabel;
    QVBoxLayout *layout;

    QWidget *buttonWidget;
    QGridLayout *buttonLayout;
    ActionButton *addBreakPointButton;

    ActionButton *debugButton, *addWatchButton, *nextStepButton, *jumpButton,
        *nextSentenceButton, *stopButton, *singleStepEnterButton,
        *jumpFuncButton, *enterSentenceButton;

    QTreeView *breakPointTreeView;
    QStandardItemModel *breakPointItemModel;

    QVector<int> breakPointLines;

    void initButtons();
    void initBreakPointTreeView();
    void appendItem(const QString &name, const QString &value);
};

#endif // DEBUGINFOAREA_H
