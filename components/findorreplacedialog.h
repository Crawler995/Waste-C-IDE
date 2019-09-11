#ifndef FINDORREPLACEDIALOG_H
#define FINDORREPLACEDIALOG_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class FindOrReplaceDialog : public QWidget
{
    Q_OBJECT
public:
    explicit FindOrReplaceDialog(QWidget *parent = 0);

    QPushButton *getFindButton() const;

    QPushButton *getReplaceButton() const;

    QLineEdit *getInputWordEdit() const;

    QLineEdit *getTargetWordEdit() const;

    bool getIsRegexMatch();
    bool getIsCaseSensitive();
    bool getIsMinimal();

private:
    QLineEdit *inputWordEdit;
    QLineEdit *targetWordEdit;
    QCheckBox *isRegexMatchCheckBox;
    QCheckBox *isCaseSensitiveCheckBox;
    QCheckBox *isMinimalCheckBox;
    QPushButton *findButton;
    QPushButton *replaceButton;

    QGridLayout *layout;
};

#endif // FINDORREPLACEDIALOG_H
