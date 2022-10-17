#ifndef DUQFDOUBLESLIDER_H
#define DUQFDOUBLESLIDER_H

#include <QProgressBar>
#include <QMouseEvent>
#include <QLocale>
#include <QtMath>

#include <QtDebug>

class DuQFDoubleSlider : public QProgressBar
{
        Q_OBJECT
public:
    DuQFDoubleSlider(QWidget *parent = nullptr);
    double value() const;

    int decimals() const;
    void setDecimals(int d);

    double maximum() const;
    void setMaximum(double m);

    double minimum() const;
    void setMinimum(double m);

    QString prefix() const;
    void setPrefix(const QString &prefix);

    QString suffix() const;
    void setSuffix(const QString &suffix);

    void showValue(bool show);
    bool valueVisible() const;

    virtual QString text() const override;

public slots:
    void setValue(double v);

signals:
    void valueChanging(double);

private:
    double _value;
    int _decimals;
    int _multiplicator;
    QLocale _locale;
    QString _prefix;
    QString _suffix;
    bool _showValue;
protected:
    void mouseMoveEvent ( QMouseEvent * event ) override;
};

#endif // DUQFDOUBLESLIDER_H
