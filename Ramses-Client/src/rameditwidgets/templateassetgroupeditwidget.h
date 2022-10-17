#ifndef TEMPLATEASSETGROUPEDITWIDGET_H
#define TEMPLATEASSETGROUPEDITWIDGET_H

#include "objecteditwidget.h"
#include "ramtemplateassetgroup.h"
#include "duqf-widgets/duqfcolorselector.h"

class TemplateAssetGroupEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit TemplateAssetGroupEditWidget(QWidget *parent = nullptr);
    explicit TemplateAssetGroupEditWidget(RamTemplateAssetGroup *templateAssetGroup, QWidget *parent = nullptr);

    RamTemplateAssetGroup *assetGroup() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void setColor(QColor c);

private:
    void setupUi();
    void connectEvents();

    DuQFColorSelector *ui_colorSelector;
    RamTemplateAssetGroup *m_assetGroup;
};

#endif // TEMPLATEASSETGROUPEDITWIDGET_H
