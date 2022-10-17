#ifndef FILETYPEEDITWIDGET_H
#define FILETYPEEDITWIDGET_H

#include <QCheckBox>

#include "objecteditwidget.h"
#include "ramfiletype.h"

class FileTypeEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit FileTypeEditWidget(QWidget *parent = nullptr);
    explicit FileTypeEditWidget(RamFileType *ag, QWidget *parent = nullptr);

    RamFileType *fileType() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void setExtensions();
    void setPreviewable(bool p);

private:
    RamFileType *m_fileType;

    void setupUi();
    void connectEvents();

    QLineEdit *ui_extensionsEdit;
    QCheckBox *ui_previewableBox;
};

#endif // FILETYPEEDITWIDGET_H
