#include "filetypeeditwidget.h"


FileTypeEditWidget::FileTypeEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

FileTypeEditWidget::FileTypeEditWidget(RamFileType *fileType, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
    setObject(fileType);
}

RamFileType *FileTypeEditWidget::fileType() const
{
    return m_fileType;
}

void FileTypeEditWidget::reInit(RamObject *o)
{
    m_fileType = qobject_cast<RamFileType*>(o);
    if (m_fileType)
    {
        ui_extensionsEdit->setText(m_fileType->extensions().join(", "));
        ui_previewableBox->setChecked(m_fileType->previewable());
    }
    else
    {
        ui_extensionsEdit->setText("");
        ui_previewableBox->setChecked(false);
    }
}

void FileTypeEditWidget::setExtensions()
{
    QStringList extensions = ui_extensionsEdit->text().split(",");
    QStringList fixedExtensions;
    for (int i = 0; i < extensions.count(); i++)
    {
        QString ext = extensions.at(i).trimmed();
        if (ext.startsWith(".")) ext = ext.remove(0,1);
        fixedExtensions << ext;
    }
    ui_extensionsEdit->setText( fixedExtensions.join(", "));

    if (m_fileType) m_fileType->setExtensions( ui_extensionsEdit->text() );
}

void FileTypeEditWidget::setPreviewable(bool p)
{
    if (!m_fileType) return;
    m_fileType->setPreviewable(p);
}

void FileTypeEditWidget::setupUi()
{
    ui_shortNameLabel->setText("Main extension");

    QLabel *extLabel = new QLabel("All extensions", this);
    ui_mainFormLayout->addWidget(extLabel, 3, 0);

    ui_extensionsEdit = new QLineEdit(this);
    ui_extensionsEdit->setPlaceholderText(".ext1, .ext2, .ext3...");
    ui_mainFormLayout->addWidget(ui_extensionsEdit, 3, 1);

    QLabel *previewableLabel = new QLabel("Previewable", this);
    ui_mainFormLayout->addWidget(previewableLabel, 4, 0);

    ui_previewableBox = new QCheckBox("This file can be used for previews\n(image, video...).", this);
    ui_mainFormLayout->addWidget(ui_previewableBox, 4, 1);

    ui_mainLayout->addStretch();
}

void FileTypeEditWidget::connectEvents()
{
    connect(ui_extensionsEdit, SIGNAL(editingFinished()), this, SLOT(setExtensions()));
    connect(ui_previewableBox, SIGNAL(clicked(bool)), this, SLOT(setPreviewable(bool)));
}
