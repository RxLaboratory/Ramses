#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QSplitter>
#include <QShowEvent>
#include <QHideEvent>

#include "duqf-widgets/duqftitlebar.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QString title = "Settings", QWidget *parent = nullptr);
    void addPage(QWidget *ui, QString title, QIcon icon=QIcon());
    void showReinitButton(bool show);
    DuQFTitleBar *titleBar();
public slots:
    void setCurrentIndex(int index);
signals:
    void reinitRequested();
    void closeRequested();
protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
private slots:
    void mainList_currentRowChanged(int currentRow);
private:
    void setupUi(QString title);
    QSplitter *m_splitter;
    QListWidget *m_mainList;
    QStackedWidget *m_mainStackWidget;
    DuQFTitleBar *m_titleBar;
};

#endif // SETTINGSWIDGET_H
