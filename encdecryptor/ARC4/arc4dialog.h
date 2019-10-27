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

#ifndef ARC4DIALOG_H
#define ARC4DIALOG_H

#include <QDialog>

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QTextBrowser>
#include <QtCore/QVector>
#include <QtWidgets/QProgressBar>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileDialog>
#include <QtCore/QThread>

#include <windows.h>
#include <wincrypt.h>

#include "ARC4/arc4object.h"

namespace Ui {
class ARC4Dialog;
}

class ARC4Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ARC4Dialog(QWidget *parent = 0);
    ~ARC4Dialog();

private:
    Ui::ARC4Dialog *ui;


    enum {
        MIN_SIZE_KEY    = 40,
        MAX_SIZE_KEY    = 256,
        SIZE_BLOCK      = 4096  // size block for read\write data
    };

    char            m_temp_data[SIZE_BLOCK];
    char            m_finish_data[SIZE_BLOCK];

    QGridLayout     *m_mainGridLayout;

    QGroupBox       *m_gboxLeft;
    QGridLayout     *m_gridLayoutLeft;

    QTextEdit       *m_plainTextEdit;
    QLabel          *m_fileNameLabel;
    QLineEdit       *m_fileNameLineEdit;

    QString         m_enc_suffix;
    QString         m_dec_suffix;
    QString         m_fileName;
    QFile           m_file;

    QFile           m_fileTemp;
    QFile           m_fileEncryption;
    QFile           m_fileDecryption;

    QGroupBox       *m_gboxFile;
    QGridLayout     *m_gridLayoutFile;
    QPushButton     *m_pushbuttonEncryptFile;
    QPushButton     *m_pushbuttonDecryptFile;
    QProgressBar    *m_progressbar_file;

    QGroupBox       *m_gboxRight;
    QGridLayout     *m_gridLayoutRight;

    QGroupBox       *m_gboxKey;
    QRadioButton    *m_radioAutomaticKey;
    QRadioButton    *m_radioCustomKey;
    QVBoxLayout     *m_radioBoxKeyLayout;

    QGroupBox       *m_gboxSizeKey;
    QLabel          *m_labelSizeKey;
    QLineEdit       *m_sizeKeyLineEdit;
    QVBoxLayout     *m_vBoxLayout;

    QTextEdit       *m_genkeyTextEdit;
    QPushButton     *m_genkeyPushButton;
    QPushButton     *m_pushbuttonEncryptMessage;
    QPushButton     *m_pushbuttonDecryptMessage;

    QVector<unsigned char>   m_vector;
    AllegedRC4      *kAllegedRC4Obj;
    QThread         *kThread;


    void automatic_generate_key();
    void custom_generate_key();

protected:
    void keyPressEvent(QKeyEvent *event);


private slots:
    void pushbutton_generate_key();
    void pushbutton_encrypt_message();
    void pushbutton_decrypt_message();

    void radio_button_custom_key();
    void radio_button_automatic_key();

    void pushbutton_encrypt_file();
    void pushbutton_decrypt_file();

    void getError(QString error_str);
    void getValueToProgressBar(int value);
};

#endif // ARC4DIALOG_H
