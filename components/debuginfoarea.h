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
    void appendItem(const QString &name, const QString &value);

    ActionButton *getDebugButton() const;

    ActionButton *getAddBreakPointButton() const;

    QVector<int> getBreakPointLines() const;

    ActionButton *getAddWatchButton() const;

    ActionButton *getNextStepButton() const;

    ActionButton *getJumpButton() const;

    ActionButton *getNextSentenceButton() const;

    ActionButton *getStopButton() const;

    ActionButton *getSingleStepEnterButton() const;

    ActionButton *getJumpFuncButton() const;

    ActionButton *getEnterSentenceButton() const;

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

};

#endif // DEBUGINFOAREA_H
