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


#ifndef SALSA20DIALOG_H
#define SALSA20DIALOG_H

#include <QDialog>
#include <QtGui/QKeyEvent>
#include <QtWidgets>

#include "salsa20object.h"

namespace Ui {
class Salsa20Dialog;
}

class Salsa20Dialog : public QDialog
{
    Q_OBJECT

    enum {
        GENERATED_SIZE_IV_VECTOR  = 8,
        GENERATED_SIZE_KEY        = 32
    };

    unsigned char m_vectorIV[GENERATED_SIZE_IV_VECTOR];
    unsigned char m_key[GENERATED_SIZE_KEY];

public:
    explicit Salsa20Dialog(QWidget *parent = 0);
    ~Salsa20Dialog();

private:
    Ui::Salsa20Dialog *ui;

    Salsa20         *kSalsa20Obj;
    QThread         *kThread;


    QString         m_enc_suffix;
    QString         m_dec_suffix;
    QString         m_fileName;
    QFile           m_file;

    QFile           m_fileTemp;

    QFile           m_fileEncryption;
    QFile           m_fileDecryption;

    QGridLayout     *m_mainGridLayout;

    QGroupBox       *m_groupBoxLeft;
    QGridLayout     *m_gridLayoutLeft;
    QTextEdit       *m_plainTextEdit;
    QLabel          *m_totalCountLettersLabel;
    QLabel          *m_fileNameLabel;
    QLineEdit       *m_setFileNameLineEdit;
    QGroupBox       *m_gBoxFile;
    QGridLayout     *m_gridLayoutFile;
    QPushButton     *m_pushbuttonEncryptFile;
    QPushButton     *m_pushbuttonDecryptFile;
    QProgressBar    *m_progressbar_file;

    QGroupBox       *m_groupBoxRight;
    QGridLayout     *m_gridLayoutRight;
    QGroupBox       *m_groupBoxVector;
    QLineEdit       *m_lineEditVector;
    QPushButton     *m_pushbuttonGenerateVector;
    QVBoxLayout     *m_vBoxLayoutVector;
    QTextEdit       *m_textEditGenerateKey;
    QPushButton     *m_pushbuttonGenerateKey;
    QPushButton     *m_pushbuttonEncryptMessage;
    QPushButton     *m_pushbuttonDecryptMessage;

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void pushbutton_generate_vector();
    void pushbutton_generate_key();

    void pushbutton_encrypt_text_message();
    void pushbutton_decrypt_text_message();

    void pushbutton_encrypt_file();
    void pushbutton_decrypt_file();

    void getError(QString error);
    void getValueToProgressBar(int value);
};

#endif // SALSA20DIALOG_H
