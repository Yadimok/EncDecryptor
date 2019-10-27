/*
 * Ron's Code ARC4
 *
 *
*/

#include "ARC4/arc4object.h"

AllegedRC4::AllegedRC4() : QObject()
{
    m_i = m_j = 0;
    m_tmp = 0;
    for (int i=0; i<BLOCK_SIZE; ++i)
    {
        m_box[i] = 0;
        m_key[i] = 0;
    }

    for (int i=0; i<READ_BLOCK_SIZE; i++)
        m_temp_data[i] = m_finish_data[i] = 0;

}

AllegedRC4::~AllegedRC4()
{
    m_i = 0;
    m_j = 0;
    m_tmp = 0;

    for (int i=0; i<BLOCK_SIZE; ++i)
    {
        m_box[i] = 0;
        m_key[i] = 0;
    }

    for (int i=0; i<READ_BLOCK_SIZE; i++)
        m_temp_data[i] = m_finish_data[i] = 0;

}

void AllegedRC4::setFileEncryption(QString &fileNameEncrypt, QVector<unsigned char> &vector)
{
    m_readFromFile.setFileName(fileNameEncrypt);
    if (!m_readFromFile.open(QIODevice::ReadOnly))
    {
        emit setError(tr("File %1 not open.")
                      .arg(m_readFromFile.fileName()));
        return;
    }

    m_writeToFile.setFileName(m_readFromFile.fileName() + "_enc");
    if(!m_writeToFile.open(QIODevice::Append))
    {
        emit setError(tr("File %1 not open.")
                      .arg(m_writeToFile.fileName()));
        return;
    }

    clear_arc4();
    init_arc4(vector);

    int dataSize = 0;

    const qint64 FILE_SIZE = m_readFromFile.size();
    float step = 0.0f;
    float value_ = 0.0f;

    while (!m_readFromFile.atEnd())
    {
        dataSize = m_readFromFile.read(m_temp_data, READ_BLOCK_SIZE);
        encrypt_message(m_temp_data, dataSize, m_finish_data);
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

void AllegedRC4::setFileDecryption(QString &fileNameDecrypt, QVector<unsigned char> &vector)
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

    clear_arc4();
    init_arc4(vector);

    int dataSize = 0;

    const qint64 FILE_SIZE = m_readFromFile.size();
    float step = 0.0f;
    float value_ = 0.0f;

    while (!m_readFromFile.atEnd())
    {
        dataSize = m_readFromFile.read(m_temp_data, READ_BLOCK_SIZE);
        decrypt_message(m_temp_data, dataSize, m_finish_data);
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

void AllegedRC4::init_arc4(QVector<unsigned char> vector)
{
    m_sizeKey = vector.size();
    for (int i=0; i<vector.size(); i++)
        m_key[i] = vector[i];


    int i;
    char tmp=0;

    for (i=0; i<BLOCK_SIZE; ++i)
        m_box[i] = static_cast<unsigned char>(i);
    m_i = 0;
    m_j = 0;

    int j=0;
    for (i=0; i<BLOCK_SIZE; ++i)
    {
        j = (j + m_box[i] + m_key[i % m_sizeKey]) % MODULO;

        tmp = m_box[i];
        m_box[i] = m_box[j];
        m_box[j] = tmp;
    }
}

void AllegedRC4::clear_arc4()
{
    m_i = 0;
    m_j = 0;

    for (int i=0; i<BLOCK_SIZE; ++i)
    {
        m_box[i] = 0;
        m_key[i] = 0;
    }
}


void AllegedRC4::encrypt_message(char msg[], int nbytes, char out_msg[])
{
    unsigned char j = 0;

    for (int idx=0; idx<nbytes; idx++)
    {
        m_i = (m_i + 1) % MODULO;
        m_j = (m_j + m_box[m_i]) % MODULO;

        m_tmp = m_box[m_i];
        m_box[m_i] = m_box[m_j];
        m_box[m_j] = m_tmp;

        j = m_box[(m_box[m_i] + m_box[m_j]) % MODULO];

        out_msg[idx] = msg[idx] ^ m_box[j];
    }
}

void AllegedRC4::decrypt_message(char msg[], int nbytes, char out_dec_msg[])
{

    unsigned char j = 0;

    for (int idx=0; idx<nbytes; idx++)
    {
        m_i = (m_i + 1) % MODULO;
        m_j = (m_j + m_box[m_i]) % MODULO;

        m_tmp = m_box[m_i];
        m_box[m_i] = m_box[m_j];
        m_box[m_j] = m_tmp;

        j = m_box[(m_box[m_i] + m_box[m_j]) % MODULO];

        out_dec_msg[idx] = msg[idx] ^ m_box[j];
    }
}
