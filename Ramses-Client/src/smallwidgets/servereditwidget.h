#ifndef SERVEREDITWIDGET_H
#define SERVEREDITWIDGET_H

#include <QWidget>
#include <QHostInfo>
#include <QMessageBox>

class ServerEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ServerEditWidget(QWidget *parent = nullptr);

    void setAddress(QString a);
    QString address();

    void setSsl(bool s);
    bool ssl() const;

    void setUpdateFreq(int f);
    int updateFreq() const;

    void setTimeout(int t);
    int timeout() const;

signals:

private slots:
    void orderServer();

private:
    void setupUi();

    QLineEdit *ui_serverAddressEdit;
    QCheckBox *ui_sslCheckBox;
    QSpinBox *ui_updateFreqSpinBox;
    QSpinBox *ui_timeoutSpinBox;
    QPushButton *ui_orderServerButton;
};

#endif // SERVEREDITWIDGET_H
