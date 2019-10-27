/* An implementation of the Rabbit stream cipher
 *
 * This implementation of the Rabbit stream cipher is derived from the
 * reference ANSI C code provided in the appendix of the paper,
 * "Rabbit: A New High-Performance Stream Cipher", by Martin Boesgaard,
 * Mette Vesterager, Thomas Pedersen, Jesper Christiansen, and
 * Ove Scavenius of Cryptico A/S.
 *
 * For more information, please visit the Cryptico website at
 * http://www.cryptico.com.
 *
 * Copyright (C) 2004 Julius C. Duque
 * Copyright (C) 2003 Cryptico A/S
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the same terms as the GNU General Public License.
 *
 * The Rabbit stream cipher is the copyrighted work of Cryptico A/S,
 * and use of Rabbit may only be used for non-commercial purposes. Any
 * reproduction or redistribution of Rabbit not in accordance with
 * Cryptico's license agreement is expressly prohibited by law, and may
 * result in severe civil and criminal penalties. Violators will be
 * prosecuted to the maximum extent possible.
 */

#ifndef RABBIT_H
#define RABBIT_H

#include <QObject>
#include <QtCore/QFile>

class Rabbit : public QObject
{
    Q_OBJECT

    enum {
        SIZE_KEY        = 8,
        SIZE_MESSAGE    = 16,
        MODULO          = 16,
        SIZE_ARRAY      = 8,
        DWORD_TO_BIT    = 32,
        READ_BLOCK_SIZE = 4096
    };

    unsigned int m_x[SIZE_ARRAY];
    unsigned int m_c[SIZE_ARRAY];
    unsigned int m_carry;

    char            m_temp_data[READ_BLOCK_SIZE];
    char            m_finish_data[READ_BLOCK_SIZE];

    QFile           m_readFromFile;
    QFile           m_writeToFile;

public:
    explicit Rabbit(QObject *parent = 0);
    virtual ~Rabbit();

    void setup_key(const unsigned char *p_key, size_t key_size);
    void setup_iv_vector(const unsigned char *p_iv, size_t iv_size);
    void encrypt_message(const char *p_src, char *p_dest, size_t data_size);
    void decrypt_message(const char *p_src, char *p_dest, size_t data_size);

    void setFileEncryption(QString &fileNameEncrypt);
    void setFileDecryption(QString &fileNameDecrypt);

private:
    unsigned int rotl(unsigned int x, int rot);
    unsigned int g_func(unsigned int x);
    void next_state();

signals:
    void finished();

    void setError(QString error);
    void setValueToProgressBar(int value);

public slots:
};

#endif // RABBIT_H
