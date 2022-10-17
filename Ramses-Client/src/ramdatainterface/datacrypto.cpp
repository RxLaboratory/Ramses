#include "datacrypto.h"

#include "duqf-utils/simplecrypt.h"

DataCrypto* DataCrypto::_instance = nullptr;

DataCrypto *DataCrypto::instance()
{
    if (!_instance) _instance = new DataCrypto();
    return _instance;
}

QString DataCrypto::clientEncrypt(QString data)
{
    SimpleCrypt crypto( m_clientKey );
    return crypto.encryptToString(data);
}

QString DataCrypto::machineEncrypt(QString data)
{
    SimpleCrypt crypto( m_machineKey );
    return crypto.encryptToString(data);
}

QString DataCrypto::clientDecrypt(QString cypher)
{
    SimpleCrypt crypto( m_clientKey );

    return crypto.decryptToString(cypher);
}

QString DataCrypto::machineDecrypt(QString cypher)
{
    SimpleCrypt crypto( m_machineKey );
    return crypto.decryptToString(cypher);
}

QString DataCrypto::generatePassHash(QString password, QString prefix, QString salt)
{
    //hash password
    QString passToHash = prefix + password + salt;
    QString hashed = QCryptographicHash::hash(passToHash.toUtf8(), QCryptographicHash::Sha3_512).toHex();
    return hashed;
}


DataCrypto::DataCrypto()
{
    // Generate client key
    m_clientKey = SimpleCrypt::clientKey(CLIENT_BUILD_KEY);
    m_machineKey = SimpleCrypt::machineKey();
}
