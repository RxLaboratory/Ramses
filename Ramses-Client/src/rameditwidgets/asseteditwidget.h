#ifndef ASSETEDITWIDGET_H
#define ASSETEDITWIDGET_H

#include "objecteditwidget.h"
#include "ramasset.h"
#include "duqf-widgets/duqffolderdisplaywidget.h"
#include "ramobjectcombobox.h"

/**
 * @brief The AssetEditWidget class is the Widget used to edit an Asset, including its Status History
 */
class AssetEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    AssetEditWidget(QWidget *parent = nullptr);
    AssetEditWidget(RamAsset *asset, QWidget *parent = nullptr);

    RamAsset *asset() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void setTags();
    void setAssetGroup(RamObject *ag);

private:
    RamAsset *m_asset = nullptr;

    void setupUi();
    void connectEvents();

    DuQFFolderDisplayWidget *ui_folderWidget;
    QLineEdit *ui_tagsEdit;
    RamObjectComboBox *ui_assetGroupBox;
};

#endif // ASSETEDITWIDGET_H
