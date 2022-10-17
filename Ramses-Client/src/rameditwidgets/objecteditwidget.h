#ifndef OBJECTEDITWIDGET_H
#define OBJECTEDITWIDGET_H

#include <QScrollArea>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QShowEvent>
#include <QRegExpValidator>

#include "duqf-widgets/duqftextedit.h"
#include "ramobject.h"

/**
 * @brief The ObjectEditWidget class is the base class of all editors for RamObjects (Shots, steps, etc)
 * It can be used either in the UI, or in a DockWidget
 */
class ObjectEditWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit ObjectEditWidget(QWidget *parent = nullptr);

    RamObject *object() const;
    void hideName(bool hide = true);

public slots:
    void setObject(RamObject *object);
    void lockShortName(bool lock = true);

protected slots:
    void setShortName();
    void setName();
    void setComment();

    void objectChanged(RamObject *o);

    void checkPath();

protected:
    virtual void reInit(RamObject *o) = 0;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

    QStringList m_dontRename;

    QVBoxLayout *ui_mainLayout;
    QGridLayout *ui_mainFormLayout;
    QLineEdit *ui_shortNameEdit;
    QLineEdit *ui_nameEdit;
    QLabel *ui_nameLabel;
    QLabel *ui_shortNameLabel;
    QLabel *ui_commentLabel;
    DuQFTextEdit *ui_commentEdit;
    QToolButton *ui_lockShortNameButton;

private slots:
    void objectRemoved(RamObject *o);
    void unlockShortName();

private:
    void setupUi();
    void connectEvents();

    RamObject *m_object;
};

#endif // OBJECTEDITWIDGET_H
