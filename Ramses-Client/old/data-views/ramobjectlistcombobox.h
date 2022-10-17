#ifndef RAMOBJECTLISTCOMBOBOX_H
#define RAMOBJECTLISTCOMBOBOX_H

#include <QComboBox>
#include <QSortFilterProxyModel>
#include <QColumnView>

#include "data-models/ramobjectlist.h"

/**
 * @brief The RamObjectListComboBox class is a QComboBox which lists the objects of a RamObjectList.
 * It can be used as a filter selection, in which case it adds an "All" item in the top of the list (using the RamObjectFilterList proxy)
 */
class RamObjectListComboBox : public QComboBox
{
    Q_OBJECT
public:
    RamObjectListComboBox(QWidget *parent = nullptr);
    RamObjectListComboBox(bool isFilterBox, QWidget *parent = nullptr);
    RamObjectListComboBox(RamObjectList *list, QWidget *parent = nullptr);

    void setList(RamObjectList *list);

    RamObject *currentObject();
    QString currentUuid();
    void setObject(QString objUuid);
    void setObject(RamObject *obj);
    RamObject *object(int i);
    QString uuid(int i);

    void beginReset();
    void endReset();

    void showPopup() override;
    void hidePopup() override;

signals:
    void currentObjectChanged(RamObject*);
    void currentUuidChanged(QString);
    void objectActivated(RamObject*);
    void uuidActivated(QString);
    void popupShown();
    void popupHidden();

private slots:
    void currentObjectIndexChanged(int i);
    void objectIndexActivated(int i );

private:

    bool m_isFilterBox = false;

    RamObject *m_resettingObject = nullptr;
    bool m_resetting = false;

    void setupUi();
    void connectEvents();
};

#endif // RAMOBJECTLISTCOMBOBOX_H
