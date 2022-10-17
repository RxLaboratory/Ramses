#ifndef DUQFDOUBLESPINBOX_H
#define DUQFDOUBLESPINBOX_H

#include <QStackedWidget>
#include <QHBoxLayout>
#include <QDoubleSpinBox>

#include "duqf-widgets/duqfdoubleslider.h"

class DuQFDoubleSpinBox : public QStackedWidget
{
    Q_OBJECT
public:
    DuQFDoubleSpinBox(QWidget *parent = nullptr);

    QString suffix() const;
    void setSuffix(const QString &suffix);

    QString prefix() const;
    void setPrefix(const QString &prefix);

    int decimals() const;
    void setDecimals(int d);

    int minimum() const;
    void setMinimum(double min);

    int maximum() const;
    void setMaximum(double max);

    double value() const;
    void setValue(double value);

    bool valueVisible() const;
    void showValue(bool showValue);

    DuQFDoubleSlider *slider() const;

    QDoubleSpinBox *spinBox() const;

signals:
    void valueChanged(int value);
private slots:
    void spinBox_editingFinished();
    void spinBox_valueChanged(double arg1);
    void slider_valueChanged(double arg1);
private:
    void setupUi();
    void connectEvents();
    DuQFDoubleSlider *_slider;
    QDoubleSpinBox *_spinBox;
    QPoint _originalMousePos;
protected:
    void mouseReleaseEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void focusOutEvent(QFocusEvent* event);
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // DUQFDOUBLESPINBOX_H
