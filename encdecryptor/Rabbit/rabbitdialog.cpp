/*
* This file is part of the EncDecryptor
*
* Copyright (C) Yadimok2016 https://bitbucket.org/Yadimok2016/encdecryptor, (C) 2016
*
* EncDecryptor is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* EncDecryptor is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with EncDecryptor. If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#include "rabbitdialog.h"
#include "ui_rabbitdialog.h"

RabbitDialog::RabbitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RabbitDialog)
{
    ui->setupUi(this);

    m_mainGridLayout = new QGridLayout();

    /// Left
    m_groupBoxLeft = new QGroupBox();
    m_gridLayoutLeft = new QGridLayout();
    m_plainTextEdit = new QTextEdit();
    m_totalCountLettersLabel = new QLabel();
    m_fileNameLabel = new QLabel("Set file name");
    m_setFileNameLineEdit = new QLineEdit();
    m_enc_suffix = "_encrypt_rabbit";
    m_dec_suffix = "_decrypt_rabbit";
    m_fileName = "";
    m_gBoxFile = new QGroupBox("Working with files");
    m_gridLayoutFile = new QGridLayout();
    m_pushbuttonEncryptFile = new QPushButton("Encrypt file");
    m_pushbuttonEncryptFile->setDisabled(true);
    m_pushbuttonDecryptFile = new QPushButton("Decrypt file");
    m_pushbuttonDecryptFile->setDisabled(true);
    m_progressbar_file = new QProgressBar();
    m_progressbar_file->setRange(0, 100);
    m_progressbar_file->setValue(0);
    m_gridLayoutFile->addWidget(m_pushbuttonEncryptFile, 0, 0);
    m_gridLayoutFile->addWidget(m_pushbuttonDecryptFile, 1, 0);
    m_gridLayoutFile->addWidget(m_progressbar_file, 2, 0);
    m_gBoxFile->setLayout(m_gridLayoutFile);
    m_gridLayoutLeft->addWidget(m_plainTextEdit, 0, 0);
    m_gridLayoutLeft->addWidget(m_totalCountLettersLabel, 1, 0);
    m_gridLayoutLeft->addWidget(m_fileNameLabel, 2, 0);
    m_gridLayoutLeft->addWidget(m_setFileNameLineEdit, 3, 0);
    m_gridLayoutLeft->addWidget(m_gBoxFile, 4, 0);
    m_groupBoxLeft->setLayout(m_gridLayoutLeft);

    /// Right
    m_groupBoxRight = new QGroupBox();
    m_gridLayoutRight = new QGridLayout();
    m_groupBoxVector = new QGroupBox();
    m_radioButtonWithoutVector = new QRadioButton("Without init vector.");
    m_radioButtonWithoutVector->setChecked(true);
    m_radioButtonWithVector = new QRadioButton("With init vector.");
    m_lineEditVector = new QLineEdit();
    m_lineEditVector->setReadOnly(true);
    m_lineEditVector->setDisabled(true);
    m_pushbuttonGenerateVector = new QPushButton("Generate vector");
    m_pushbuttonGenerateVector->setDisabled(true);
    m_vBoxLayoutVector = new QVBoxLayout();
    m_vBoxLayoutVector->addWidget(m_radioButtonWithoutVector);
    m_vBoxLayoutVector->setMargin(5);
    m_vBoxLayoutVector->addWidget(m_radioButtonWithVector);
    m_vBoxLayoutVector->setMargin(5);
    m_vBoxLayoutVector->addWidget(m_lineEditVector);
    m_vBoxLayoutVector->setMargin(5);
    m_vBoxLayoutVector->addWidget(m_pushbuttonGenerateVector);
    m_groupBoxVector->setLayout(m_vBoxLayoutVector);
    m_textEditGenerateKey = new QTextEdit();
    m_textEditGenerateKey->setReadOnly(true);
    m_pushbuttonGenerateKey = new QPushButton("Generate key");
    m_pushbuttonEncryptMessage = new QPushButton("Encrypt text message");
    m_pushbuttonEncryptMessage->setDisabled(true);
    m_pushbuttonDecryptMessage = new QPushButton("Decrypt text message");
    m_pushbuttonDecryptMessage->setDisabled(true);

    m_gridLayoutRight->addWidget(m_groupBoxVector, 0, 0);
    m_gridLayoutRight->addWidget(m_textEditGenerateKey, 1, 0);
    m_gridLayoutRight->addWidget(m_pushbuttonGenerateKey, 2, 0);
    m_gridLayoutRight->addWidget(m_pushbuttonEncryptMessage, 3, 0);
    m_gridLayoutRight->addWidget(m_pushbuttonDecryptMessage, 4, 0);
    m_groupBoxRight->setLayout(m_gridLayoutRight);

    m_mainGridLayout->addWidget(m_groupBoxLeft, 0, 0);
    m_mainGridLayout->addWidget(m_groupBoxRight, 0, 1);
    setLayout(m_mainGridLayout);

    m_isInitVectorIV = false;
    for (size_t idx=0; idx<SIZE_GENERATED_KEY; idx++)
        m_key[idx] = 0;
    for (size_t idx=0; idx<SIZE_INIT_VECTOR; idx++)
        m_vectorIV[idx] = 0;


    kRabbitObj = new Rabbit();
    kThread = new QThread();
    kRabbitObj->moveToThread(kThread);

    connect(kRabbitObj, SIGNAL(finished()), kThread, SLOT(quit()));
    connect(kRabbitObj, SIGNAL(finished()), kRabbitObj, SLOT(deleteLater()));
    connect(kThread, SIGNAL(finished()), kThread, SLOT(deleteLater()));
    kThread->start();

    connect(m_radioButtonWithoutVector, SIGNAL(clicked(bool)), this, SLOT(radiobutton_without_init_vector()));
    connect(m_radioButtonWithVector, SIGNAL(clicked(bool)), this, SLOT(radiobutton_with_init_vector()));
    connect(m_pushbuttonGenerateVector, SIGNAL(clicked(bool)), this, SLOT(pushbutton_generate_vector()));
    connect(m_pushbuttonGenerateKey, SIGNAL(clicked(bool)), this, SLOT(pushbutton_generate_key()));
    connect(m_pushbuttonEncryptMessage, SIGNAL(clicked(bool)), this, SLOT(pushbutton_encrypt_text_message()));
    connect(m_pushbuttonDecryptMessage, SIGNAL(clicked(bool)), this, SLOT(pushbutton_decrypt_text_message()));
    connect(m_pushbuttonEncryptFile, SIGNAL(clicked(bool)), this, SLOT(pushbutton_encrypt_file()));
    connect(m_pushbuttonDecryptFile, SIGNAL(clicked(bool)), this, SLOT(pushbutton_decrypt_file()));

    connect(kRabbitObj, SIGNAL(setError(QString)), this, SLOT(getError(QString)));
    connect(kRabbitObj, SIGNAL(setValueToProgressBar(int)), this,
            SLOT(getValueToProgressBar(int)));
}

RabbitDialog::~RabbitDialog()
{
    delete ui;
    delete kRabbitObj;
}

void RabbitDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        event->ignore();
        return;
    }
    QDialog::keyPressEvent(event);
}

void RabbitDialog::radiobutton_without_init_vector()
{
    m_radioButtonWithVector->setChecked(false);
    m_radioButtonWithoutVector->setChecked(true);
    m_lineEditVector->clear();
    m_lineEditVector->setDisabled(true);
    m_pushbuttonGenerateVector->setDisabled(true);

    m_isInitVectorIV = false;
}

void RabbitDialog::radiobutton_with_init_vector()
{
    m_radioButtonWithVector->setChecked(true);
    m_radioButtonWithoutVector->setChecked(false);
    m_lineEditVector->clear();
    m_lineEditVector->setDisabled(false);
    m_pushbuttonGenerateVector->setDisabled(false);

    m_isInitVectorIV = true;
}

void RabbitDialog::pushbutton_generate_vector()
{
    QByteArray ba_to_text;

#ifdef Q_OS_WIN
    HCRYPTPROV hProvider = 0;

    if (!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        return;

    DWORD dwLength = SIZE_INIT_VECTOR;
    BYTE pbBuffer[dwLength] = {0};

    if (!::CryptGenRandom(hProvider, dwLength, pbBuffer))
    {
        ::CryptReleaseContext(hProvider, 0);
        return;
    }

    for (DWORD i=0; i<dwLength; ++i)
    {
        m_vectorIV[i] = pbBuffer[i];
        ba_to_text.append(pbBuffer[i]);
    }

    if (!::CryptReleaseContext(hProvider, 0))
        return;
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    qDebug() << "Linux version not implemented.";
#endif

    m_lineEditVector->setText(ba_to_text.toHex());
}

void RabbitDialog::pushbutton_generate_key()
{
    QByteArray ba_to_text;

#ifdef Q_OS_WIN
    HCRYPTPROV hProvider = 0;

    if (!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        return;

    DWORD dwLength = SIZE_GENERATED_KEY;
    BYTE pbBuffer[dwLength] = {0};

    if (!::CryptGenRandom(hProvider, dwLength, pbBuffer))
    {
        ::CryptReleaseContext(hProvider, 0);
        return;
    }

    for (DWORD i=0; i<dwLength; ++i)
    {
        m_key[i] = pbBuffer[i];
        ba_to_text.append(pbBuffer[i]);
    }

    if (!::CryptReleaseContext(hProvider, 0))
        return;
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    qDebug() << "Linux version not implemented.";
#endif

    m_textEditGenerateKey->setText(ba_to_text.toHex());

    m_pushbuttonEncryptMessage->setDisabled(false);
    m_pushbuttonDecryptMessage->setDisabled(false);
    m_pushbuttonEncryptFile->setDisabled(false);
    m_pushbuttonDecryptFile->setDisabled(false);
}

void RabbitDialog::pushbutton_encrypt_text_message()
{
    QMessageBox msgBox;

//    if (m_textEditGenerateKey->toPlainText().isEmpty())
//    {
//        msgBox.setWindowTitle("Generate key");
//        msgBox.setText("Key is empty. Please, generate key for encrypt\\decrypt message or file('s).");
//        msgBox.exec();
//        return;
//    }

    if (m_plainTextEdit->toPlainText().isEmpty())
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText("Text is empty. Please, enter a message for encrypt\\decrypt message.");
        msgBox.exec();
        return;
    }

    if (m_setFileNameLineEdit->text().isEmpty())
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText("File name is empty. Please, set file name.");
        msgBox.exec();
        return;
    }

    const int SIZE_MESSAGE = m_plainTextEdit->toPlainText().size();

    m_totalCountLettersLabel->setText(tr("Total letters count - %1. %2")
                                      .arg(SIZE_MESSAGE)
                                      .arg((SIZE_MESSAGE % SIZE_MESSAGE_MODULO) == 0 ? "":"Must be a multiple of 16."));

    if ((SIZE_MESSAGE % SIZE_MESSAGE_MODULO) != 0)
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText("The size of the text to encrypt must be a multiple of 16.");
        msgBox.exec();
        return;
    }

    char *in_message = new char[SIZE_MESSAGE]();
    char *out_message = new char[SIZE_MESSAGE]();

    for (int idx=0; idx<SIZE_MESSAGE; idx++)
        in_message[idx] = m_plainTextEdit->toPlainText().toLatin1().at(idx);

    //
    kRabbitObj->setup_key(m_key, SIZE_GENERATED_KEY);
    if (m_isInitVectorIV)
        kRabbitObj->setup_iv_vector(m_vectorIV, SIZE_INIT_VECTOR);
    kRabbitObj->encrypt_message(in_message, out_message, SIZE_MESSAGE);
    //

    m_fileName = m_setFileNameLineEdit->text();
    m_fileName += m_enc_suffix;

    m_file.setFileName(m_fileName);
    if (!m_file.open(QIODevice::WriteOnly))
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("File %1 not open.")
                       .arg(m_file.fileName()));
        msgBox.exec();
        return;
    }
    m_file.write(out_message, SIZE_MESSAGE);

    if (m_file.isOpen())
        m_file.close();

    delete []in_message;
    delete []out_message;
}

void RabbitDialog::pushbutton_decrypt_text_message()
{
    QMessageBox msgBox;

    const size_t SIZE_MESSAGE = m_plainTextEdit->toPlainText().size();
    char *out_message = new char[SIZE_MESSAGE]();

    m_file.setFileName(m_fileName);
    if (!m_file.open(QIODevice::ReadOnly))
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("File %1 not open.")
                       .arg(m_file.fileName()));
        msgBox.exec();
        return;
    }

    const qint64 READ_SIZE = m_file.size();
    char *buffer = new char[READ_SIZE]();
    qint64 dataSize = m_file.read(buffer, READ_SIZE);

    if (m_file.isOpen())
        m_file.close();

    ///
    kRabbitObj->setup_key(m_key, SIZE_GENERATED_KEY);
    if (m_isInitVectorIV)
        kRabbitObj->setup_iv_vector(m_vectorIV, SIZE_INIT_VECTOR);
    kRabbitObj->decrypt_message(buffer, out_message, SIZE_MESSAGE);
    ///

    m_fileName.clear();
    m_fileName = m_setFileNameLineEdit->text();
    m_fileName += m_dec_suffix;

    m_file.setFileName(m_fileName);
    if (!m_file.open(QIODevice::WriteOnly))
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("File %1 not open.")
                       .arg(m_file.fileName()));
        msgBox.exec();
        return;
    }
    m_file.write(out_message, dataSize);

    if (m_file.isOpen())
        m_file.close();

    delete []buffer;
    delete []out_message;
}

void RabbitDialog::pushbutton_encrypt_file()
{
    m_progressbar_file->reset();
    QMessageBox msgBox;
    QString fileNameForEncryption = QFileDialog::getOpenFileName(this,
                                                                 tr("Open file for encryption."),
                                                                 QDir::currentPath(),
                                                                 QString());
    if (fileNameForEncryption.isEmpty())
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("File %1 is empty.")
                       .arg(fileNameForEncryption));
        msgBox.exec();
        return;
    }

    m_file.setFileName(fileNameForEncryption);
    const qint64 FILE_SIZE = m_file.size();
    if ((FILE_SIZE % SIZE_MESSAGE_MODULO) != 0)
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("File size for encryption must be a multiple of 16. The difference is %1 bytes.")
                       .arg(SIZE_MESSAGE_MODULO - (FILE_SIZE % SIZE_MESSAGE_MODULO)));
        msgBox.exec();
        return;
    }
    else
    {
        ///
        kRabbitObj->setup_key(m_key, SIZE_GENERATED_KEY);
        if (m_isInitVectorIV)
            kRabbitObj->setup_iv_vector(m_vectorIV, SIZE_INIT_VECTOR);
        kRabbitObj->setFileEncryption(fileNameForEncryption);
        ///
    }
}

void RabbitDialog::pushbutton_decrypt_file()
{
    m_progressbar_file->reset();
    QString fileNameForDecryption = QFileDialog::getOpenFileName(this,
                                                                 tr("Open file for encryption."),
                                                                 QDir::currentPath(),
                                                                 QString());
    if (fileNameForDecryption.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("File %1 is empty.")
                       .arg(fileNameForDecryption));
        msgBox.exec();
        return;
    }
    ///
    kRabbitObj->setup_key(m_key, SIZE_GENERATED_KEY);
    if (m_isInitVectorIV)
        kRabbitObj->setup_iv_vector(m_vectorIV, SIZE_INIT_VECTOR);
    kRabbitObj->setFileDecryption(fileNameForDecryption);
    ///
}

void RabbitDialog::getError(QString error)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setText(error);
    msgBox.exec();
}

void RabbitDialog::getValueToProgressBar(int value)
{
    m_progressbar_file->setValue(value);
}




