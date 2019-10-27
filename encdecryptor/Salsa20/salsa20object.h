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

#ifndef SALSA20_H
#define SALSA20_H

#include <QObject>
#include <QtCore/QFile>

class Salsa20 : public QObject
{
    Q_OBJECT

    enum {
        IV_SIZE         = 8,
        VECTOR_SIZE     = 16,
        KEY_SIZE        = 32,
        BLOCK_SIZE      = 64,
        READ_BLOCK_SIZE = 4096
    };

    unsigned int m_vector[VECTOR_SIZE];

    unsigned int rotate(unsigned int value, unsigned int numBits);
    void convert(unsigned int value, unsigned char *array);
    unsigned int convert(const unsigned char *array);
    void generateKeyStream(unsigned char out[BLOCK_SIZE]);

    char        m_temp_data[READ_BLOCK_SIZE];
    char        m_finish_data[READ_BLOCK_SIZE];

    QFile       m_readFromFile;
    QFile       m_writeToFile;

public:
    explicit Salsa20(QObject *parent = 0);
    ~Salsa20();

    void setIVVector(const unsigned char* vector);
    void setKey(const unsigned char* key);

    void encrypt_message(const char *p_src, char *p_dest, size_t data_size);
    void decrypt_message(const char *p_src, char *p_dest, size_t data_size);

    void setFileEncryption(QString &fileNameEncrypt);
    void setFileDecryption(QString &fileNameDecrypt);

signals:
    void finished();

    void setError(QString error);
    void setValueToProgressBar(int value);

public slots:
};

#endif // SALSA20_H
