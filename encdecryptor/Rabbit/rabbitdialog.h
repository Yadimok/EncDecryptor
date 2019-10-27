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

#ifndef RABBITDIALOG_H
#define RABBITDIALOG_H

#include <QDialog>
#include <QtGui/QKeyEvent>
#include <QtWidgets>

#include <QDebug>

#include "rabbitobject.h"

namespace Ui {
class RabbitDialog;
}

class RabbitDialog : public QDialog
{
    Q_OBJECT

    enum {
        SIZE_INIT_VECTOR    = 8,
        SIZE_GENERATED_KEY  = 16,
        SIZE_MESSAGE_MODULO = 16
    };

    bool m_isInitVectorIV;

    unsigned char m_vectorIV[SIZE_INIT_VECTOR];
    unsigned char m_key[SIZE_GENERATED_KEY];

public:
    explicit RabbitDialog(QWidget *parent = 0);
    ~RabbitDialog();

private:
    Ui::RabbitDialog *ui;

    Rabbit          *kRabbitObj;
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
    QRadioButton    *m_radioButtonWithoutVector;
    QRadioButton    *m_radioButtonWithVector;
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
    void radiobutton_without_init_vector();
    void radiobutton_with_init_vector();

    void pushbutton_generate_vector();
    void pushbutton_generate_key();

    void pushbutton_encrypt_text_message();
    void pushbutton_decrypt_text_message();

    void pushbutton_encrypt_file();
    void pushbutton_decrypt_file();

    void getError(QString error);
    void getValueToProgressBar(int value);

};

#endif // RABBITDIALOG_H
