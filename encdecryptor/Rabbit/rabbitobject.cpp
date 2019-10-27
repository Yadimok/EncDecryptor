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


#include "rabbitobject.h"

Rabbit::Rabbit(QObject *parent) : QObject(parent)
{
    for (size_t idx=0; idx<SIZE_ARRAY; idx++)
        m_x[idx] = m_c[idx] = 0;
    m_carry = 0;

    for (size_t idx=0; idx<READ_BLOCK_SIZE; idx++)
        m_temp_data[idx] = m_finish_data[idx] = 0;
}

Rabbit::~Rabbit()
{
    for (size_t idx=0; idx<SIZE_ARRAY; idx++)
        m_x[idx] = m_c[idx] = 0;
    m_carry = 0;

    for (size_t idx=0; idx<READ_BLOCK_SIZE; idx++)
        m_temp_data[idx] = m_finish_data[idx] = 0;
}

unsigned int Rabbit::rotl(unsigned int x, int rot)
{
    return (x << rot) | (x >> (DWORD_TO_BIT - rot));
}

unsigned int Rabbit::g_func(unsigned int x)
{
    unsigned int a = x & 0xFFFF;
    unsigned int b = x >> 16;

    unsigned int h = ((((a * a) >> 17) + (a * b)) >> 15) + b * b;
    unsigned int l = x * x;

    return h ^ l;
}

void Rabbit::next_state()
{
    unsigned int g[SIZE_ARRAY],
                 c_old[SIZE_ARRAY],
                 i;

    for (i=0; i<SIZE_ARRAY; i++)
        c_old[i] = m_c[i];

    m_c[0] += 0x4d34d3d4 + m_carry;
    m_c[1] += 0xd34d34d3 + (m_c[0] < c_old[0]);
    m_c[2] += 0x34d34d34 + (m_c[1] < c_old[1]);
    m_c[3] += 0x4d34d34d + (m_c[2] < c_old[2]);
    m_c[4] += 0xd34d34d3 + (m_c[3] < c_old[3]);
    m_c[5] += 0x34d34d34 + (m_c[4] < c_old[4]);
    m_c[6] += 0x4d34d34d + (m_c[5] < c_old[5]);
    m_c[7] += 0xd34d34d3 + (m_c[6] < c_old[6]);
    m_carry = (m_c[7] < c_old[7]);

    for (i=0; i<SIZE_ARRAY; i++)
        g[i] = g_func(m_x[i] + m_c[i]);


    m_x[0] = g[0] + rotl(g[7], 16) + rotl(g[6], 16);
    m_x[1] = g[1] + rotl(g[0], 8) + g[7];
    m_x[2] = g[2] + rotl(g[1], 16) + rotl(g[0], 16);
    m_x[3] = g[3] + rotl(g[2], 8) + g[1];
    m_x[4] = g[4] + rotl(g[3], 16) + rotl(g[2], 16);
    m_x[5] = g[5] + rotl(g[4], 8) + g[3];
    m_x[6] = g[6] + rotl(g[5], 16) + rotl(g[4], 16);
    m_x[7] = g[7] + rotl(g[6], 8) + g[5];
}

void Rabbit::setup_key(const unsigned char *p_key, size_t key_size)
{
    unsigned int k0,
                 k1,
                 k2,
                 k3,
                 i;

    if (key_size != MODULO)
    {
        emit setError("Key size is not equal to 16 bytes.");
        return;
    }

    k0 = *(unsigned int *)(p_key + 0);
    k1 = *(unsigned int *)(p_key + 4);
    k2 = *(unsigned int *)(p_key + 8);
    k3 = *(unsigned int *)(p_key + 12);

    m_x[0] = k0;
    m_x[2] = k1;
    m_x[4] = k2;
    m_x[6] = k3;
    m_x[1] = (k3 << 16) | (k2 >> 16);
    m_x[3] = (k0 << 16) | (k3 >> 16);
    m_x[5] = (k1 << 16) | (k0 >> 16);
    m_x[7] = (k2 << 16) | (k1 >> 16);

    m_c[0] = rotl(k2, 16);
    m_c[2] = rotl(k3, 16);
    m_c[4] = rotl(k0, 16);
    m_c[6] = rotl(k1, 16);
    m_c[1] = (k0 & 0xFFFF0000) | (k1 & 0xFFFF);
    m_c[3] = (k1 & 0xFFFF0000) | (k2 & 0xFFFF);
    m_c[5] = (k2 & 0xFFFF0000) | (k3 & 0xFFFF);
    m_c[7] = (k3 & 0xFFFF0000) | (k0 & 0xFFFF);

    m_carry = 0;

    for (i=0; i<4; i++)
        next_state();

    for (i=0; i<SIZE_ARRAY; i++)
        m_c[(i+4)&0x7] ^= m_x[i];

}

void Rabbit::setup_iv_vector(const unsigned char *p_iv, size_t iv_size)
{
    unsigned int i0,
                 i1,
                 i2,
                 i3,
                 i;
    if (iv_size != 8)
    {
        emit setError("The size of the initialization vector is not equal to 8 bytes.");
        return;
    }

    i0 = *(unsigned int *)(p_iv + 0);
    i1 = *(unsigned int *)(p_iv + 4);
    i2 = (i0 >> 16) | (i1 & 0xFFFF0000);
    i3 = (i2 << 16) | (i0 & 0xFFFF);

    m_c[0] = m_c[0] ^ i0;
    m_c[1] = m_c[1] ^ i1;
    m_c[2] = m_c[2] ^ i2;
    m_c[3] = m_c[3] ^ i3;
    m_c[4] = m_c[4] ^ i0;
    m_c[5] = m_c[5] ^ i1;
    m_c[6] = m_c[6] ^ i2;
    m_c[7] = m_c[7] ^ i3;

    for (i=0; i<SIZE_ARRAY; i++)
        m_x[i] = m_x[i];
    m_carry = m_carry;

    for (i=0; i<4; i++)
        next_state();
}

void Rabbit::encrypt_message(const char *p_src, char *p_dest, size_t data_size)
{
    unsigned int i;

    if (data_size % MODULO)
    {
        emit setError("The data size should be a multiple of 16.");
        return;
    }

    for (i=0; i<data_size; i += MODULO)
    {
        *(unsigned int *)(p_dest + 0) = *(unsigned int *)(p_src + 0) ^ m_x[0] ^ (m_x[5] >> 16) ^ (m_x[3] << 16);
        *(unsigned int *)(p_dest + 4) = *(unsigned int *)(p_src + 4) ^ m_x[2] ^ (m_x[7] >> 16) ^ (m_x[5] << 16);
        *(unsigned int *)(p_dest + 8) = *(unsigned int *)(p_src + 8) ^ m_x[4] ^ (m_x[1] >> 16) ^ (m_x[7] << 16);
        *(unsigned int *)(p_dest + 12) = *(unsigned int *)(p_src + 12) ^ m_x[6] ^ (m_x[3] >> 16) ^ (m_x[1] << 16);

        p_src += MODULO;
        p_dest += MODULO;
    }
}

void Rabbit::decrypt_message(const char *p_src, char *p_dest, size_t data_size)
{
    unsigned int i;

    if (data_size % MODULO)
    {
        emit setError("The data size should be a multiple of 16.");
        return;
    }

    for (i=0; i<data_size; i += MODULO)
    {
        *(unsigned int *)(p_dest + 0) = *(unsigned int *)(p_src + 0) ^ m_x[0] ^ (m_x[5] >> 16) ^ (m_x[3] << 16);
        *(unsigned int *)(p_dest + 4) = *(unsigned int *)(p_src + 4) ^ m_x[2] ^ (m_x[7] >> 16) ^ (m_x[5] << 16);
        *(unsigned int *)(p_dest + 8) = *(unsigned int *)(p_src + 8) ^ m_x[4] ^ (m_x[1] >> 16) ^ (m_x[7] << 16);
        *(unsigned int *)(p_dest + 12) = *(unsigned int *)(p_src + 12) ^ m_x[6] ^ (m_x[3] >> 16) ^ (m_x[1] << 16);

        p_src += MODULO;
        p_dest += MODULO;
    }
}

void Rabbit::setFileEncryption(QString &fileNameEncrypt)
{
    m_readFromFile.setFileName(fileNameEncrypt);
    if (!m_readFromFile.open(QIODevice::ReadOnly))
    {
        emit setError(tr("File %1 not open.")
                      .arg(m_readFromFile.fileName()));
        return;
    }

    m_writeToFile.setFileName(m_readFromFile.fileName() + "_enc");
    if (!m_writeToFile.open(QIODevice::Append))
    {
        emit setError(tr("File %1 not open.")
                      .arg(m_writeToFile.fileName()));
        return;
    }

    const qint64 FILE_SIZE = m_readFromFile.size();

    int dataSize = 0;
    float step = 0.0f;
    float value_ = 0.0f;

    while (!m_readFromFile.atEnd())
    {
        dataSize = m_readFromFile.read(m_temp_data, READ_BLOCK_SIZE);
        encrypt_message(m_temp_data, m_finish_data, dataSize);
        m_writeToFile.write(m_finish_data, dataSize);

        //send to progress bar
        step += dataSize;
        value_ = (step / FILE_SIZE) * 100;
        emit setValueToProgressBar(static_cast<int>(value_));
    }

    if (m_readFromFile.isOpen())
        m_readFromFile.close();

    if (m_writeToFile.isOpen())
        m_writeToFile.close();

    emit finished();
}

void Rabbit::setFileDecryption(QString &fileNameDecrypt)
{
    m_readFromFile.setFileName(fileNameDecrypt);
    if (!m_readFromFile.open(QIODevice::ReadOnly))
    {
        emit setError(tr("File %1 not open.")
                      .arg(m_readFromFile.fileName()));
        return;
    }

    QString tmpFileName = m_readFromFile.fileName();
    if (tmpFileName.endsWith("_enc"))
    {
        tmpFileName.remove("_enc");
        tmpFileName += "_dec";
    }

    m_writeToFile.setFileName(tmpFileName);
    if(!m_writeToFile.open(QIODevice::Append))
    {
        emit setError(tr("File %1 not open.")
                      .arg(m_writeToFile.fileName()));
        return;
    }

    const qint64 FILE_SIZE = m_readFromFile.size();

    int dataSize = 0;
    float step = 0.0f;
    float value_ = 0.0f;

    while (!m_readFromFile.atEnd())
    {
        dataSize = m_readFromFile.read(m_temp_data, READ_BLOCK_SIZE);
        decrypt_message(m_temp_data, m_finish_data, dataSize);
        m_writeToFile.write(m_finish_data, dataSize);

        // send to progress bar
        step += dataSize;
        value_ = (step / FILE_SIZE) * 100;
        emit setValueToProgressBar(static_cast<int>(value_));
    }

    if (m_readFromFile.isOpen())
        m_readFromFile.close();

    if (m_writeToFile.isOpen())
        m_writeToFile.close();

    emit finished();
}

