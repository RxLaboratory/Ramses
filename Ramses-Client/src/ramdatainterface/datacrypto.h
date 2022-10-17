#ifndef DATACRYPTO_H
#define DATACRYPTO_H

#include "duqf-app/app-config.h"

class DataCrypto
{
public:
    static DataCrypto *instance();

    QString clientEncrypt(QString data);
    QString machineEncrypt(QString data);

    QString clientDecrypt(QString cypher);
    QString machineDecrypt(QString cypher);

    /**
     * @brief Generates a hash for a password
     * @param password The password to hash
     * @param salt The salt to use
     * @return The hashed password
     */
    QString generatePassHash(QString password, QString prefix = "", QString salt = CLIENT_BUILD_KEY);

protected:
    static DataCrypto* _instance;

private:
    DataCrypto();

    quint64 m_clientKey;
    quint64 m_machineKey;
};

#endif // DATACRYPTO_H
