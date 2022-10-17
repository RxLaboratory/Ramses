#ifndef ASSETGROUPEDITWIDGET_H
#define ASSETGROUPEDITWIDGET_H

#include "objecteditwidget.h"
#include "objectlistwidget.h"
#include "ramassetgroup.h"
#include "duqf-widgets/duqffolderdisplaywidget.h"
#include "duqf-widgets/duqfcolorselector.h"

/**
 * @brief The AssetGroupEditWidget class is used to edit AssetGroups and can be shown either in the main UI or in the Dock
 */
class AssetGroupEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit AssetGroupEditWidget(QWidget *parent = nullptr);
    explicit AssetGroupEditWidget(RamAssetGroup *ag, QWidget *parent = nullptr);

    RamAssetGroup *assetGroup() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void setColor(QColor c);
    void createAsset();

private:
    RamAssetGroup *m_assetGroup;

    void setupUi();
    void connectEvents();

    DuQFColorSelector *ui_colorSelector;
    DuQFFolderDisplayWidget *ui_folderWidget;
    ObjectListWidget *ui_assetsList;
};

#endif // ASSETGROUPEDITWIDGET_H
