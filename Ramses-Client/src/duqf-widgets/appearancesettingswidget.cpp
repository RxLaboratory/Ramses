#include "appearancesettingswidget.h"

#include "duqf-app/app-style.h"

AppearanceSettingsWidget::AppearanceSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    _freezeUI = true;

    // Update UI
    ui_gridLayout->setAlignment(Qt::AlignVCenter);

    // Date format
    QDateTime d = QDateTime::fromString("2021-04-26 10:53:31", "yyyy-MM-dd hh:mm:ss");
    QString f = "yyyy-MM-dd hh:mm:ss";
    dateFormatBox->addItem(d.toString(f), f);
    f = "MM.dd.yyyy - h:mm:ss ap";
    dateFormatBox->addItem(d.toString(f), f);
    f = "dd/MM/yyyy - hh:mm:ss";
    dateFormatBox->addItem(d.toString(f), f);
    f = "ddd MMMM d yyyy 'at' h:mm:ss ap";
    dateFormatBox->addItem(d.toString(f), f);
    f = "ddd d MMMM yyyy 'at' hh:mm:ss";
    dateFormatBox->addItem(d.toString(f), f);

    // List available styles
    styleComboBox->addItem("Default",":/styles/default");
    styleComboBox->addItems(QStyleFactory::keys());
    styleComboBox->addItem("Custom...","");

    // Load settings
    settings.beginGroup("appearance");

    int toolButtonStyle = settings.value("toolButtonStyle", 2).toInt();
    toolsComboBox->setCurrentIndex(toolButtonStyle);

    QString style = settings.value("style", "Default").toString();
    styleComboBox->setCurrentText(style);

    QString font = settings.value("font", "Ubuntu").toString();
    fontComboBox->setCurrentFont(QFont(font));

    QString dateFormat = settings.value("dateFormat", "yyyy-MM-dd hh:mm:ss").toString();
    for (int i = 0; i < dateFormatBox->count(); i++)
    {
        if (dateFormatBox->itemData(i).toString() == dateFormat)
        {
            dateFormatBox->setCurrentIndex(i);
            break;
        }
    }

    _freezeUI = false;
}

void AppearanceSettingsWidget::on_toolsComboBox_currentIndexChanged(int index)
{
    if (_freezeUI) return;

    Qt::ToolButtonStyle style = Qt::ToolButtonTextUnderIcon;
    if (index == 0) style = Qt::ToolButtonIconOnly;
    else if (index == 1) style = Qt::ToolButtonTextOnly;
    else if (index == 2) style = Qt::ToolButtonTextUnderIcon;
    else if (index == 3) style = Qt::ToolButtonTextBesideIcon;

    settings.setValue("toolButtonStyle", index);

    DuUI::setToolButtonStyle(style);
}

void AppearanceSettingsWidget::on_styleComboBox_currentIndexChanged(int index)
{
    if (_freezeUI) return;

    QString cssFile = ":/styles/default";
    QString style = "Default";

    if(index == styleComboBox->count() - 1)
    {
        QString cssFileName = QFileDialog::getOpenFileName(this,"Open a stylesheet file","","CSS (*.css);;Text files (*.txt);;All files (*.*)");
        QFile checkFile(cssFileName);
        if (checkFile.exists())
        {
            cssFile = cssFileName;
        }
        else cssFile = settings.value("cssFile",cssFile).toString();
    }
    else
    {
        style = styleComboBox->currentText();
        cssFile = styleComboBox->currentData().toString();
    }

    settings.setValue("style", style);
    settings.setValue("cssFile", cssFile);

    if (cssFile != "")
    {
        DuUI::updateCSS(cssFile);
    }
    else
    {
        DuUI::updateCSS("");
        qApp->setStyle(QStyleFactory::create(style));
    }
}

void AppearanceSettingsWidget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    if (_freezeUI) return;

    settings.setValue("font", f.family());

    qApp->setFont(f);
    DuUI::updateCSS(settings.value("cssFile",":/styles/default").toString());
}

void AppearanceSettingsWidget::on_dateFormatBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    settings.setValue("dateFormat", dateFormatBox->currentData().toString());
}
