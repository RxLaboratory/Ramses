#ifndef FRAMERATEWIDGET_H
#define FRAMERATEWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include "duqf-widgets/autoselectdoublespinbox.h"

class FramerateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FramerateWidget(QWidget *parent = nullptr);

    double framerate() const;
    void setFramerate(double fr);

signals:
    void framerateChanged(double);

private slots:
    void setPreset(int p);
    void selectPreset(double fr);

private:
    void setupUi();
    void connectEvents();

    QComboBox *presetsBox;
    AutoSelectDoubleSpinBox *framerateBox;

};

#endif // FRAMERATEWIDGET_H
