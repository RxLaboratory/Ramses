#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QFrame>

#include "duqf-widgets/duqfloggingtextedit.h"
#include "duqf-utils/duqflogger.h"

class ConsoleWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ConsoleWidget(QWidget *parent = nullptr);
    explicit ConsoleWidget(DuQFLoggerObject *o, QWidget *parent = nullptr);

signals:

private slots:
    void levelIndexChanged(int index);
    void clear();

private:
    void setupUi(DuQFLoggerObject *o = nullptr);
    void connectEvents();

    DuQFLoggingTextEdit *ui_consoleEdit;
    QComboBox *ui_levelBox;
    QPushButton *ui_clearButton;
};

#endif // CONSOLEWIDGET_H
