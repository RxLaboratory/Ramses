#ifndef USEREDITWIDGET_H
#define USEREDITWIDGET_H

#include <QComboBox>
#include <QToolButton>

#include "duqf-utils/utils.h"
#include "objecteditwidget.h"
#include "duqf-widgets/duqffolderdisplaywidget.h"
#include "duqf-widgets/duqfcolorselector.h"
#include "ramuser.h"

class UserEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit UserEditWidget(RamUser *user, QWidget *parent = nullptr);
    explicit UserEditWidget(QWidget *parent = nullptr);

    RamUser *user() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void changePassword();
    bool checkPasswordInput();
    void setRole(int r);
    void setColor(QColor c);
    void connectionStatusChanged(NetworkUtils::NetworkStatus status);

private:
    RamUser *m_user;

    void setupUi();
    void connectEvents();

    DuQFFolderDisplayWidget *ui_folderWidget;
    QComboBox *ui_roleBox;
    QLineEdit *ui_cpasswordEdit;
    QLineEdit *ui_npassword1Edit;
    QLineEdit *ui_npassword2Edit;
    QLabel *ui_folderLabel;
    QToolButton *ui_passwordButton;
    DuQFColorSelector *ui_colorSelector;
    QWidget *ui_passwordWidget;
    QLabel *ui_currentPasswordLabel;
};

#endif // USEREDITWIDGET_H
