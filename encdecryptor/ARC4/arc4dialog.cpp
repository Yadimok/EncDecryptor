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

#include "arc4dialog.h"
#include "ui_arc4dialog.h"

ARC4Dialog::ARC4Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::ARC4Dialog)
{
    ui->setupUi(this);

    m_mainGridLayout    = new QGridLayout();

    m_gboxLeft          = new QGroupBox();
    m_gridLayoutLeft    = new QGridLayout();

    m_plainTextEdit     = new QTextEdit();
    m_fileNameLabel     = new QLabel("Set file name");
    m_fileNameLineEdit  = new QLineEdit();
    m_enc_suffix        = "_encrypt_arc4";
    m_dec_suffix        = "_decrypt_arc4";
    m_fileName          = "";

    m_gboxFile          = new QGroupBox("Working with files");
    m_gridLayoutFile    = new QGridLayout();
    m_pushbuttonEncryptFile = new QPushButton("Encrypt file");
    m_pushbuttonEncryptFile->setDisabled(true);
    m_pushbuttonDecryptFile = new QPushButton("Decrypt file");
    m_pushbuttonDecryptFile->setDisabled(true);
    m_progressbar_file  = new QProgressBar();
    m_progressbar_file->setRange(0, 100);
    m_progressbar_file->setValue(0);

    m_gridLayoutFile->addWidget(m_pushbuttonEncryptFile, 0, 0);
    m_gridLayoutFile->addWidget(m_pushbuttonDecryptFile, 1, 0);
    m_gridLayoutFile->addWidget(m_progressbar_file, 2, 0);
    m_gboxFile->setLayout(m_gridLayoutFile);

    m_gridLayoutLeft->addWidget(m_plainTextEdit, 0, 0);
    m_gridLayoutLeft->addWidget(m_fileNameLabel, 1, 0);
    m_gridLayoutLeft->addWidget(m_fileNameLineEdit, 2, 0);
    m_gridLayoutLeft->addWidget(m_gboxFile, 3, 0);
    m_gboxLeft->setLayout(m_gridLayoutLeft);

    m_gboxRight         = new QGroupBox();
    m_gridLayoutRight   = new QGridLayout();

    m_gboxKey           = new QGroupBox();
    m_radioAutomaticKey = new QRadioButton("Generate automatic key.");
    m_radioCustomKey    = new QRadioButton("Insert custom key.");
    m_radioAutomaticKey->setChecked(true);

    m_radioBoxKeyLayout = new QVBoxLayout();
    m_radioBoxKeyLayout->addWidget(m_radioAutomaticKey);
    m_radioBoxKeyLayout->setMargin(20);
    m_radioBoxKeyLayout->addWidget(m_radioCustomKey);
    m_gboxKey->setLayout(m_radioBoxKeyLayout);

    m_gboxSizeKey       = new QGroupBox();
    m_labelSizeKey      = new QLabel(tr("Size key, bits (from %1 to %2):")
                                     .arg(MIN_SIZE_KEY)
                                     .arg(MAX_SIZE_KEY));
    m_sizeKeyLineEdit   = new QLineEdit();
    QRegExp rx("[0-9]+");
    QValidator *validator = new QRegExpValidator(rx, this);
    m_sizeKeyLineEdit->setValidator(validator);

    m_vBoxLayout        = new QVBoxLayout();
    m_vBoxLayout->addWidget(m_labelSizeKey);
    m_vBoxLayout->setMargin(20);
    m_vBoxLayout->addWidget(m_sizeKeyLineEdit);
    m_gboxSizeKey->setLayout(m_vBoxLayout);

    m_genkeyTextEdit    = new QTextEdit();
    m_genkeyTextEdit->setReadOnly(true);
    m_genkeyPushButton  = new QPushButton("Generate key");
    m_pushbuttonEncryptMessage = new QPushButton("Encrypt text message");
    m_pushbuttonEncryptMessage->setDisabled(true);
    m_pushbuttonDecryptMessage = new QPushButton("Decrypt text message");
    m_pushbuttonDecryptMessage->setDisabled(true);

    m_gridLayoutRight->addWidget(m_gboxKey, 0, 0);
    m_gridLayoutRight->addWidget(m_gboxSizeKey, 1, 0);
    m_gridLayoutRight->addWidget(m_genkeyTextEdit, 2, 0);
    m_gridLayoutRight->addWidget(m_genkeyPushButton, 3, 0);
    m_gridLayoutRight->addWidget(m_pushbuttonEncryptMessage, 4, 0);
    m_gridLayoutRight->addWidget(m_pushbuttonDecryptMessage, 5, 0);
    m_gboxRight->setLayout(m_gridLayoutRight);

    m_mainGridLayout->addWidget(m_gboxLeft, 0, 0);
    m_mainGridLayout->addWidget(m_gboxRight, 0, 1);

    setLayout(m_mainGridLayout);

    ////
    kThread = new QThread();
    kAllegedRC4Obj = new AllegedRC4();
    kAllegedRC4Obj->moveToThread(kThread);

    connect(kAllegedRC4Obj, SIGNAL(setError(QString)), this, SLOT(getError(QString)));
    connect(kAllegedRC4Obj, SIGNAL(setValueToProgressBar(int)), this, SLOT(getValueToProgressBar(int)));

    connect(kAllegedRC4Obj, SIGNAL(finished()), kThread, SLOT(quit()));
    connect(kAllegedRC4Obj, SIGNAL(finished()), kAllegedRC4Obj, SLOT(deleteLater()));
    connect(kThread, SIGNAL(finished()), kThread, SLOT(deleteLater()));
    kThread->start();
    ///


    connect(m_genkeyPushButton, SIGNAL(clicked()), this, SLOT(pushbutton_generate_key()));
    connect(m_pushbuttonEncryptMessage, SIGNAL(clicked()), this, SLOT(pushbutton_encrypt_message()));
    connect(m_pushbuttonDecryptMessage, SIGNAL(clicked()), this, SLOT(pushbutton_decrypt_message()));

    connect(m_radioAutomaticKey, SIGNAL(clicked()), this, SLOT(radio_button_automatic_key()));
    connect(m_radioCustomKey, SIGNAL(clicked()), this, SLOT(radio_button_custom_key()));

    connect(m_pushbuttonEncryptFile, SIGNAL(clicked()), this, SLOT(pushbutton_encrypt_file()));
    connect(m_pushbuttonDecryptFile, SIGNAL(clicked()), this, SLOT(pushbutton_decrypt_file()));


    for (int idx=0; idx<SIZE_BLOCK; idx++)
    {
        m_temp_data[idx] = 0;
        m_finish_data[idx] = 0;
    }
}

ARC4Dialog::~ARC4Dialog()
{
    delete ui;
    delete kAllegedRC4Obj;
}

void ARC4Dialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        event->ignore();
        return;
    }
    QDialog::keyPressEvent(event);
}

void ARC4Dialog::pushbutton_generate_key()
{
    if (m_radioAutomaticKey->isChecked())
        automatic_generate_key();
    else
        custom_generate_key();
}

void ARC4Dialog::automatic_generate_key()
{
    if (!m_vector.isEmpty())
        m_vector.clear();

    QByteArray ba_to_text;
    QMessageBox msgBox;

    if (m_sizeKeyLineEdit->text().isEmpty())
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("Enter a size key from %1 to %2.")
                       .arg(MIN_SIZE_KEY)
                       .arg(MAX_SIZE_KEY));
        msgBox.exec();
        return;
    }

    if ((m_sizeKeyLineEdit->text().toInt() < MIN_SIZE_KEY) ||
            (m_sizeKeyLineEdit->text().toInt() > MAX_SIZE_KEY))
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("The size key is very small or very big. Enter a size key from %1 to %2")
                       .arg(MIN_SIZE_KEY)
                       .arg(MAX_SIZE_KEY));
        msgBox.exec();
        return;
    }

#ifdef Q_OS_WIN
    HCRYPTPROV hProvider = 0;

    if (!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        return;

    DWORD dwLength = m_sizeKeyLineEdit->text().toInt();
    BYTE pbBuffer[dwLength] = {0};

    if (!::CryptGenRandom(hProvider, dwLength, pbBuffer))
    {
        ::CryptReleaseContext(hProvider, 0);
        return;
    }

    for (DWORD i=0; i<dwLength; ++i)
    {
        m_vector.push_back(pbBuffer[i]);
        ba_to_text.append(pbBuffer[i]);
    }

    if (!::CryptReleaseContext(hProvider, 0))
        return;
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    qDebug() << "Linux version not implemented.";
#endif

    m_genkeyTextEdit->setText(ba_to_text.toHex());

    m_pushbuttonEncryptMessage->setEnabled(true);
    m_pushbuttonDecryptMessage->setEnabled(true);
    m_pushbuttonEncryptFile->setEnabled(true);
    m_pushbuttonDecryptFile->setEnabled(true);
}

void ARC4Dialog::custom_generate_key()
{
    if (!m_vector.isEmpty())
        m_vector.clear();

    QMessageBox msgBox;

    if (m_genkeyTextEdit->toPlainText().isEmpty())
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("Key is empty. Enter any key (size key from %1 to %2).")
                       .arg(MIN_SIZE_KEY)
                       .arg(MAX_SIZE_KEY));
        msgBox.exec();
        return;
    }

    const int SIZE_KEY = m_genkeyTextEdit->toPlainText().size();
    for (int i=0; i<SIZE_KEY; i++)
        m_vector.push_back(static_cast<unsigned char>(m_genkeyTextEdit->toPlainText().at(i).toLatin1()));

    m_sizeKeyLineEdit->setText(QString::number(SIZE_KEY));

    m_pushbuttonEncryptMessage->setEnabled(true);
    m_pushbuttonDecryptMessage->setEnabled(true);
    m_pushbuttonEncryptFile->setEnabled(true);
    m_pushbuttonDecryptFile->setEnabled(true);
}


void ARC4Dialog::pushbutton_encrypt_message()
{
    QMessageBox msgBox;
    if (m_plainTextEdit->toPlainText().isEmpty())
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText("Enter a message for encryption.");
        msgBox.exec();
        return;
    }

    if (m_fileNameLineEdit->text().isEmpty())
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText("Enter a file name for encryption.");
        msgBox.exec();
        return;
    }

    const int SIZE_MESSAGE = m_plainTextEdit->toPlainText().size();

    char *in_message = new char[SIZE_MESSAGE]();
    char *out_message = new char[SIZE_MESSAGE]();

    for (int idx=0; idx<SIZE_MESSAGE; idx++)
        in_message[idx] = m_plainTextEdit->toPlainText().toLatin1().at(idx);

    kAllegedRC4Obj->clear_arc4();
    kAllegedRC4Obj->init_arc4(m_vector);
    kAllegedRC4Obj->encrypt_message(in_message, SIZE_MESSAGE, out_message);

    m_fileName = m_fileNameLineEdit->text();
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

void ARC4Dialog::pushbutton_decrypt_message()
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

    kAllegedRC4Obj->clear_arc4();
    kAllegedRC4Obj->init_arc4(m_vector);
    kAllegedRC4Obj->decrypt_message(buffer, dataSize, out_message);

    m_fileName.clear();
    m_fileName = m_fileNameLineEdit->text();
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


void ARC4Dialog::radio_button_automatic_key()
{
    m_radioCustomKey->setChecked(false);
    m_genkeyTextEdit->setReadOnly(true);
}

void ARC4Dialog::radio_button_custom_key()
{
    m_radioAutomaticKey->setChecked(false);
    m_genkeyTextEdit->setReadOnly(false);
}

void ARC4Dialog::pushbutton_encrypt_file()
{
    m_progressbar_file->reset();
    QString fileNameForEncryption = QFileDialog::getOpenFileName(this,
                                                                 tr("Open file for encryption."),
                                                                 QDir::currentPath(),
                                                                 QString());
    if (fileNameForEncryption.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(tr("File %1 is empty.")
                       .arg(fileNameForEncryption));
        msgBox.exec();
        return;
    }

    kAllegedRC4Obj->setFileEncryption(fileNameForEncryption, m_vector);
}

void ARC4Dialog::pushbutton_decrypt_file()
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
    kAllegedRC4Obj->setFileDecryption(fileNameForDecryption, m_vector);
}

void ARC4Dialog::getError(QString error_str)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setText(error_str);
    msgBox.exec();
}

void ARC4Dialog::getValueToProgressBar(int value)
{
    m_progressbar_file->setValue(value);
}
