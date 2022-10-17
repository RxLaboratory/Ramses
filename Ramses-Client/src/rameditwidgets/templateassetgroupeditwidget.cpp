#include "templateassetgroupeditwidget.h"

TemplateAssetGroupEditWidget::TemplateAssetGroupEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

TemplateAssetGroupEditWidget::TemplateAssetGroupEditWidget(RamTemplateAssetGroup *templateAssetGroup, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    setObject(templateAssetGroup);
    connectEvents();
}

RamTemplateAssetGroup *TemplateAssetGroupEditWidget::assetGroup() const
{
    return m_assetGroup;
}

void TemplateAssetGroupEditWidget::reInit(RamObject *obj)
{
    m_assetGroup = qobject_cast<RamTemplateAssetGroup*>(obj);
    if (m_assetGroup)
    {
        ui_colorSelector->setColor(m_assetGroup->color());
    }
    else
    {
        ui_colorSelector->setColor(QColor(67,67,67));
    }
}

void TemplateAssetGroupEditWidget::setColor(QColor c)
{
    if (!m_assetGroup) return;
    m_assetGroup->setColor(c);
}

void TemplateAssetGroupEditWidget::setupUi()
{
    QLabel *colorLabel = new QLabel("Color", this);
    ui_mainFormLayout->addWidget(colorLabel, 3, 0);
    ui_colorSelector = new DuQFColorSelector(this);
    ui_mainFormLayout->addWidget(ui_colorSelector, 3, 1);

    ui_mainLayout->addStretch();
}

void TemplateAssetGroupEditWidget::connectEvents()
{
    connect(ui_colorSelector, &DuQFColorSelector::colorChanged, this, &TemplateAssetGroupEditWidget::setColor);
}
