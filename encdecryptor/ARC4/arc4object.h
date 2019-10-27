/*
 * Ron's Code ARC4
 *
 *
*/

#ifndef RC4_H
#define RC4_H

#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QFile>

class AllegedRC4 : public QObject
{
    Q_OBJECT

    enum {
        BLOCK_SIZE      = 256,
        MODULO          = 256,
        READ_BLOCK_SIZE = 4096
    };

    unsigned char   m_box[BLOCK_SIZE];
    int             m_i;
    int             m_j;
    unsigned char   m_key[BLOCK_SIZE];
    int             m_sizeKey;

    unsigned char   m_tmp;

    char            m_temp_data[READ_BLOCK_SIZE];
    char            m_finish_data[READ_BLOCK_SIZE];

    QFile           m_readFromFile;
    QFile           m_writeToFile;

public:
    AllegedRC4();
    virtual ~AllegedRC4();

    void clear_arc4();
    void init_arc4(QVector<unsigned char> vector);

    void encrypt_message(char in_msg[], int nbytes, char out_msg[]);
    void decrypt_message(char in_msg[], int nbytes, char out_msg[]);

    void setFileEncryption(QString &fileNameEncrypt, QVector<unsigned char> &vector);
    void setFileDecryption(QString &fileNameDecrypt, QVector<unsigned char> &vector);

signals:
    void setError(QString error);
    void setValueToProgressBar(int value);

    void finished();
};

#endif // RC4_H
