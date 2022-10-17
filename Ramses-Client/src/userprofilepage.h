#ifndef USERPROFILEPAGE_H
#define USERPROFILEPAGE_H

#include <QErrorMessage>
#include <QMessageBox>

#include "ramses.h"
#include "usereditwidget.h"

class UserProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit UserProfilePage(QWidget *parent = nullptr);

private slots:
    void loggedIn(RamUser *user);
    void loggedOut();
private:
    void setupUi();
    UserEditWidget *profileWidget;

};

#endif // USERPROFILEPAGE_H
