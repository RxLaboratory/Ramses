#ifndef RAMOBJECT_H
#define RAMOBJECT_H

#include <QObject>
#include <QMap>
#include <QSettings>
#include <QStringBuilder>
#include <QMetaEnum>

#include "ramabstractobject.h"

class ObjectDockWidget;
class ObjectEditWidget;
class RamObjectModel;

class RamObject : public QObject, public RamAbstractObject
{
    Q_OBJECT
public:

    // STATIC METHODS //
    static RamObject *get(QString uuid, ObjectType type);

    // METHODS //

    /**
     * @brief RamObject constructs a new object and adds it in the database
     * @param shortName
     * @param name
     */
    RamObject(QString shortName, QString name, ObjectType type, QObject *parent = nullptr, bool isVirtual = false, bool encryptData = false);

    virtual QString filterUuid() const { return QString(); };
    virtual QStringList filterListUuids() const { return QStringList(); }
    virtual RamObject *objectForColumn(QString columnUuid) const;

    virtual bool canEdit();

    void emitDataChanged() override;

public slots:
    virtual void edit(bool s = true) { Q_UNUSED(s) };
    void reload();
    // reimplemented to disconnect signals
    virtual void remove() override;

signals:
    void dataChanged(RamObject *);
    void removed(RamObject *);
    void restored(RamObject *);

protected:
    // METHODS //

    /**
     * @brief RamObject constructs an object using its UUID and type. The UUID must exist in the database.
     * @param uuid
     * @param parent
     */
    RamObject(QString uuid, ObjectType type, QObject *parent = nullptr, bool encryptData = false);

    virtual QJsonObject reloadData() override;

    void emitRemoved() override;
    void emitRestored() override;

    /**
     * @brief createEditFrame sets the widget used to edit this object in the UI
     * @param w
     */
    QFrame *createEditFrame(ObjectEditWidget *w );
    /**
     * @brief showEdit shows the edit widget
     * @param title
     */
    void showEdit(QWidget *w = nullptr, QString title = "");

    // ATTRIBUTES //

    UserRole m_editRole = Admin;
    bool m_editable = true;

    RamObjectModel *createModel(ObjectType type, QString modelName);
    void loadModel(RamObjectModel *model, QString modelName, QJsonObject d = QJsonObject());

private slots:
    void checkData(QString uuid);
    void checkAvailability(QString uuid, bool availability);
    void saveModel();

private:
    void construct(QObject *parent = nullptr);
    ObjectDockWidget *m_dockWidget = nullptr;
    // models and their names
    QMap<RamObjectModel*, QString> m_subModels;
    bool m_loadingModels = false;
};

#endif // RAMOBJECT_H
