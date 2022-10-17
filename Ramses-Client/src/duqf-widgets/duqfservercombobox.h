#ifndef DUQFSERVERCOMBOBOX_H
#define DUQFSERVERCOMBOBOX_H

#include <QComboBox>

class DuQFServerComboBox : public QComboBox
{
    Q_OBJECT
public:
    DuQFServerComboBox(QString defaultAddress = "", QWidget *parent = nullptr);
    QString address() const;
public slots:
    void setAddress(QString newAddress);
signals:
    void addressChanged(QString);
protected:
    void focusOutEvent(QFocusEvent *e) override;
private slots:
    void textChanged(QString text);
    void indexChanged(int index);
private:
    QSettings m_settings;
    bool m_addTrailingSlash = true;
};

#endif // DUQFSERVERCOMBOBOX_H
