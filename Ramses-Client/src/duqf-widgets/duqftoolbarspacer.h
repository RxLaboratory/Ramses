#ifndef TOOLBARSPACER_H
#define TOOLBARSPACER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSpacerItem>

class DuQFToolBarSpacer : public QWidget
{
public:
    explicit DuQFToolBarSpacer(QWidget *parent = nullptr);

private:
    void setupUi();
};


#endif // TOOLBARSPACER_H
