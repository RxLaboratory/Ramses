#ifndef RESOLUTIONWIDGET_H
#define RESOLUTIONWIDGET_H

#include <QWidget>

#include <QVBoxLayout>
#include <QComboBox>

#include "duqf-widgets/autoselectspinbox.h"

class ResolutionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResolutionWidget(QWidget *parent = nullptr);

    int getWidth() const;
    int getHeight() const;

    void setWidth(int w);
    void setHeight(int h);

signals:
    void resolutionChanged(int,int);

private slots:
    void setPreset(int p);
    void selectPreset();

private:
    void setupUi();
    void connectEvents();

    QComboBox *presetsBox;
    AutoSelectSpinBox *widthBox;
    AutoSelectSpinBox *heightBox;

};

#endif // RESOLUTIONWIDGET_H
