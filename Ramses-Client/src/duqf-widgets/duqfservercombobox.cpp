#include "duqfservercombobox.h"

DuQFServerComboBox::DuQFServerComboBox(QString defaultAddress, QWidget *parent):
    QComboBox(parent)
{
    this->setEditable(true);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    this->setPlaceholderText("sub.domain.tld");
#endif

    // Get the server history
    int historySize = m_settings.beginReadArray("server/serverHistory");
    for (int i = 0; i < historySize; i++)
    {
        m_settings.setArrayIndex( i );
        this->addItem(m_settings.value("address").toString());
    }
    m_settings.endArray();

    // Set the current one
    this->setCurrentText(m_settings.value("server/address", defaultAddress).toString());

    // connect the text changed
    //connect(this, SIGNAL(currentTextChanged(QString)), this, SLOT(textChanged(QString)));
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
}

void DuQFServerComboBox::setAddress(QString newAddress)
{
    textChanged(newAddress);
}

QString DuQFServerComboBox::address() const
{
    return this->currentText();
}

void DuQFServerComboBox::focusOutEvent(QFocusEvent *e)
{
    QString text = this->currentText();
    textChanged(text);
    QComboBox::focusOutEvent(e);
}

void DuQFServerComboBox::textChanged(QString text)
{
    // don't emit when we update the field
    QSignalBlocker b(this);
    // Trim, just in case
    QString newAddress = text.trimmed();
    // Add trailing slash
    if (m_addTrailingSlash && !newAddress.endsWith("/"))
        newAddress = newAddress + "/";
    // Set new text
    this->setCurrentText(newAddress);
    // Save in settings
    m_settings.setValue("server/address", newAddress);
    b.unblock();
    // emit
    emit addressChanged(newAddress);
}

void DuQFServerComboBox::indexChanged(int index)
{
    Q_UNUSED(index)

    textChanged(this->currentText());
}

