/*
Copyright (c) 2014 Nezametdinov E. Ildus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include "salsa20object.h"

Salsa20::Salsa20(QObject *parent) : QObject(parent)
{
    for (size_t idx=0; idx < VECTOR_SIZE; idx++)
        m_vector[idx] = 0;

    for (size_t idx=0; idx < READ_BLOCK_SIZE; idx++)
        m_temp_data[idx] = m_finish_data[idx] = 0;
}

Salsa20::~Salsa20()
{
    for (size_t idx=0; idx < VECTOR_SIZE; idx++)
        m_vector[idx] = 0;

    for (size_t idx=0; idx < READ_BLOCK_SIZE; idx++)
        m_temp_data[idx] = m_finish_data[idx] = 0;
}

void Salsa20::setIVVector(const unsigned char *vector)
{
    if(vector == nullptr)
        return;

    m_vector[6] = convert(&vector[0]);
    m_vector[7] = convert(&vector[4]);
    m_vector[8] = m_vector[9] = 0;
}

void Salsa20::setKey(const unsigned char *key)
{
    static const char constants[] = "expand 32-byte k";

    if(key == nullptr)
        return;

    m_vector[0] = convert(reinterpret_cast<const unsigned char*>(&constants[0]));
    m_vector[1] = convert(&key[0]);
    m_vector[2] = convert(&key[4]);
    m_vector[3] = convert(&key[8]);
    m_vector[4] = convert(&key[12]);
    m_vector[5] = convert(reinterpret_cast<const unsigned char*>(&constants[4]));

    m_vector[6] = m_vector[7] = m_vector[8] = m_vector[9] = 0;

    m_vector[10] = convert(reinterpret_cast<const unsigned char*>(&constants[8]));
    m_vector[11] = convert(&key[16]);
    m_vector[12] = convert(&key[20]);
    m_vector[13] = convert(&key[24]);
    m_vector[14] = convert(&key[28]);
    m_vector[15] = convert(reinterpret_cast<const unsigned char*>(&constants[12]));
}

unsigned int Salsa20::rotate(unsigned int value, unsigned int numBits)
{
    return (value << numBits) | (value >> (32 - numBits));
}

void Salsa20::convert(unsigned int value, unsigned char *array)
{
    array[0] = static_cast<unsigned char>(value >> 0);
    array[1] = static_cast<unsigned char>(value >> 8);
    array[2] = static_cast<unsigned char>(value >> 16);
    array[3] = static_cast<unsigned char>(value >> 24);
}

unsigned int Salsa20::convert(const unsigned char *array)
{
    return ((static_cast<unsigned int>(array[0]) << 0)  |
            (static_cast<unsigned int>(array[1]) << 8)  |
            (static_cast<unsigned int>(array[2]) << 16) |
            (static_cast<unsigned int>(array[3]) << 24));
}

void Salsa20::generateKeyStream(unsigned char out[])
{
    unsigned int x[VECTOR_SIZE];

    for (size_t idx=0; idx < VECTOR_SIZE; idx++)
        x[idx] = m_vector[idx];

    for(size_t i = 20; i > 0; i -= 2)
    {
        x[4 ] ^= rotate(static_cast<unsigned int>(x[0 ] + x[12]),  7);
        x[8 ] ^= rotate(static_cast<unsigned int>(x[4 ] + x[0 ]),  9);
        x[12] ^= rotate(static_cast<unsigned int>(x[8 ] + x[4 ]), 13);
        x[0 ] ^= rotate(static_cast<unsigned int>(x[12] + x[8 ]), 18);
        x[9 ] ^= rotate(static_cast<unsigned int>(x[5 ] + x[1 ]),  7);
        x[13] ^= rotate(static_cast<unsigned int>(x[9 ] + x[5 ]),  9);
        x[1 ] ^= rotate(static_cast<unsigned int>(x[13] + x[9 ]), 13);
        x[5 ] ^= rotate(static_cast<unsigned int>(x[1 ] + x[13]), 18);
        x[14] ^= rotate(static_cast<unsigned int>(x[10] + x[6 ]),  7);
        x[2 ] ^= rotate(static_cast<unsigned int>(x[14] + x[10]),  9);
        x[6 ] ^= rotate(static_cast<unsigned int>(x[2 ] + x[14]), 13);
        x[10] ^= rotate(static_cast<unsigned int>(x[6 ] + x[2 ]), 18);
        x[3 ] ^= rotate(static_cast<unsigned int>(x[15] + x[11]),  7);
        x[7 ] ^= rotate(static_cast<unsigned int>(x[3 ] + x[15]),  9);
        x[11] ^= rotate(static_cast<unsigned int>(x[7 ] + x[3 ]), 13);
        x[15] ^= rotate(static_cast<unsigned int>(x[11] + x[7 ]), 18);
        x[1 ] ^= rotate(static_cast<unsigned int>(x[0 ] + x[3 ]),  7);
        x[2 ] ^= rotate(static_cast<unsigned int>(x[1 ] + x[0 ]),  9);
        x[3 ] ^= rotate(static_cast<unsigned int>(x[2 ] + x[1 ]), 13);
        x[0 ] ^= rotate(static_cast<unsigned int>(x[3 ] + x[2 ]), 18);
        x[6 ] ^= rotate(static_cast<unsigned int>(x[5 ] + x[4 ]),  7);
        x[7 ] ^= rotate(static_cast<unsigned int>(x[6 ] + x[5 ]),  9);
        x[4 ] ^= rotate(static_cast<unsigned int>(x[7 ] + x[6 ]), 13);
        x[5 ] ^= rotate(static_cast<unsigned int>(x[4 ] + x[7 ]), 18);
        x[11] ^= rotate(static_cast<unsigned int>(x[10] + x[9 ]),  7);
        x[8 ] ^= rotate(static_cast<unsigned int>(x[11] + x[10]),  9);
        x[9 ] ^= rotate(static_cast<unsigned int>(x[8 ] + x[11]), 13);
        x[10] ^= rotate(static_cast<unsigned int>(x[9 ] + x[8 ]), 18);
        x[12] ^= rotate(static_cast<unsigned int>(x[15] + x[14]),  7);
        x[13] ^= rotate(static_cast<unsigned int>(x[12] + x[15]),  9);
        x[14] ^= rotate(static_cast<unsigned int>(x[13] + x[12]), 13);
        x[15] ^= rotate(static_cast<unsigned int>(x[14] + x[13]), 18);
    }

    for(size_t i = 0; i < VECTOR_SIZE; ++i)
    {
        x[i] += m_vector[i];
        convert(x[i], &out[4 * i]);
    }

    ++m_vector[8];
    m_vector[9] += m_vector[8] == 0 ? 1 : 0;
}

void Salsa20::encrypt_message(const char *p_src, char *p_dest, size_t data_size)
{
    unsigned char keyStream[BLOCK_SIZE];
    size_t numBytesToProcess = 0;

    while (data_size != 0)
    {
        generateKeyStream(keyStream);
        numBytesToProcess = ((data_size >= BLOCK_SIZE) ? BLOCK_SIZE : data_size);

        for (size_t idx=0; idx<numBytesToProcess; idx++, data_size--)
            *(p_dest++) = keyStream[idx] ^ *(p_src++);

    }
}

void Salsa20::decrypt_message(const char *p_src, char *p_dest, size_t data_size)
{
    unsigned char keyStream[BLOCK_SIZE];
    size_t numBytesToProcess = 0;

    while (data_size != 0)
    {
        generateKeyStream(keyStream);
        numBytesToProcess = ((data_size >= BLOCK_SIZE) ? BLOCK_SIZE : data_size);

        for (size_t idx=0; idx<numBytesToProcess; idx++, data_size--)
            *(p_dest++) = keyStream[idx] ^ *(p_src++);

    }
}

void Salsa20::setFileEncryption(QString &fileNameEncrypt)
{
    m_readFromFile.setFileName(fileNameEncrypt);
    if (!m_readFromFile.open(QIODevice::ReadOnly))
    {
        emit setError(tr("FIle %1 not open.")
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

void Salsa20::setFileDecryption(QString &fileNameDecrypt)
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
