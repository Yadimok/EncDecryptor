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



#include "matchfiles.h"
#include "ui_matchfiles.h"

#include <QtDebug>

MatchFiles::MatchFiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatchFiles)
{
    ui->setupUi(this);

    m_tabWidget = new QTabWidget();
    m_gridLayout = new QGridLayout();

    /// m_tabWidget #1
    m_boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    m_groupBox = new QGroupBox();
    m_comboBoxHashAlgo = new QComboBox();
    m_comboBoxHashAlgo->addItem("MD4");
    m_comboBoxHashAlgo->addItem("MD5");
    m_comboBoxHashAlgo->addItem("SHA1");
    m_comboBoxHashAlgo->addItem("SH224");
    m_comboBoxHashAlgo->addItem("SHA256");
    m_comboBoxHashAlgo->addItem("SHA384");
    m_comboBoxHashAlgo->addItem("SHA512");
    m_comboBoxHashAlgo->addItem("SHA3_224");
    m_comboBoxHashAlgo->addItem("SHA3_256");
    m_comboBoxHashAlgo->addItem("SHA3_384");
    m_comboBoxHashAlgo->addItem("SHA3_512");
    m_comboBoxHashAlgo->setCurrentIndex(5);
    m_pushbutton = new QPushButton("Match files.");
    m_labelHash1 = new QLabel("Test label hash 1");
    m_labelHash2 = new QLabel("Test label hash 2");
    m_labelResultHash = new QLabel("Test label hash result");
    m_labelResultHash->setFont(QFont("Courier", 12, QFont::ExtraBold));

    m_boxLayout->addWidget(m_comboBoxHashAlgo);
    m_boxLayout->setMargin(10);
    m_boxLayout->addWidget(m_pushbutton);
    m_boxLayout->setMargin(10);
    m_boxLayout->addWidget(m_labelHash1);
    m_boxLayout->setMargin(10);
    m_boxLayout->addWidget(m_labelHash2);
    m_boxLayout->setMargin(10);
    m_boxLayout->addWidget(m_labelResultHash);
    m_groupBox->setLayout(m_boxLayout);


    /// m_tabWidget #2
    m_boxLayoutRight = new QBoxLayout(QBoxLayout::TopToBottom);
    m_groupBoxRight = new QGroupBox();
    m_comboBoxHashAlgoRight = new QComboBox();
    m_comboBoxHashAlgoRight->addItem("MD4");
    m_comboBoxHashAlgoRight->addItem("MD5");
    m_comboBoxHashAlgoRight->addItem("SHA1");
    m_comboBoxHashAlgoRight->addItem("SH224");
    m_comboBoxHashAlgoRight->addItem("SHA256");
    m_comboBoxHashAlgoRight->addItem("SHA384");
    m_comboBoxHashAlgoRight->addItem("SHA512");
    m_comboBoxHashAlgoRight->addItem("SHA3_224");
    m_comboBoxHashAlgoRight->addItem("SHA3_256");
    m_comboBoxHashAlgoRight->addItem("SHA3_384");
    m_comboBoxHashAlgoRight->addItem("SHA3_512");
    m_comboBoxHashAlgoRight->setCurrentIndex(5);
    m_pushButtonRight = new QPushButton("Calculate hash file.");
    m_labelHashRight = new QLabel("Test label hash file");
    m_boxLayoutRight->addWidget(m_comboBoxHashAlgoRight);
    m_boxLayoutRight->setMargin(10);
    m_boxLayoutRight->addWidget(m_pushButtonRight);
    m_boxLayoutRight->setMargin(10);
    m_boxLayoutRight->addWidget(m_labelHashRight);
    m_groupBoxRight->setLayout(m_boxLayoutRight);


    m_tabWidget->addTab(m_groupBox, "Match files.");
    m_tabWidget->addTab(m_groupBoxRight, "Calculate hash file.");
    m_gridLayout->addWidget(m_tabWidget);

    setLayout(m_gridLayout);
    setFont(QFont("DejaVu Sans", 10));
    setModal(true);

    kMatchFilesObj = new MatchFilesObject();
    kThread = new QThread();
    kMatchFilesObj->moveToThread(kThread);

    connect(m_pushbutton, SIGNAL(clicked()), this, SLOT(pushbutton_match_files()));
    connect(m_pushButtonRight, SIGNAL(clicked()), this, SLOT(pushbutton_calculate_hash_file()));

    connect(kMatchFilesObj, SIGNAL(setError(QString)), this, SLOT(getError(QString)));
    connect(kMatchFilesObj, SIGNAL(setHashResult(QMap<QString,QByteArray>)), this,
            SLOT(getHashResult(QMap<QString,QByteArray>)));
    connect(kMatchFilesObj, SIGNAL(setHashFileResult(QMap<QString,QByteArray>)), this,
            SLOT(getHashFile(QMap<QString,QByteArray>)));

    connect(kMatchFilesObj, SIGNAL(finished()), kThread, SLOT(quit()));
    connect(kMatchFilesObj, SIGNAL(finished()), kMatchFilesObj, SLOT(deleteLater()));
    connect(kThread, SIGNAL(finished()), kThread, SLOT(deleteLater()));
    kThread->start();
}

MatchFiles::~MatchFiles()
{
    delete ui;
    delete kMatchFilesObj;
}

QString MatchFiles::isHashesMatch(const QString &source, const QString &destination) const
{
    if (0 == source.compare(destination))
        return tr("Hashes match.");
    return tr("Hashes NOT match.");
}

void MatchFiles::pushbutton_match_files()
{
    QString sourceFile = QFileDialog::getOpenFileName(0,
                                                     "Open source file for hash calculate.",
                                                     QDir::currentPath(),
                                                     "*.*");
    if (sourceFile.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("File %1 is empty.")
                       .arg(sourceFile));
        msgBox.exec();
        return;
    }

    QString destinationFile = QFileDialog::getOpenFileName(0,
                                                      "Open destination file for hash calculate.",
                                                      QDir::currentPath(),
                                                      "*.*");
    if (destinationFile.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("File %1 is empty.")
                       .arg(destinationFile));
        msgBox.exec();
        return;
    }

    ///
    kMatchFilesObj->setAlgorithm(m_comboBoxHashAlgo->currentIndex());
    kMatchFilesObj->calculateHashes(sourceFile, destinationFile);
    ///
}

void MatchFiles::getHashResult(QMap<QString, QByteArray> result)
{
    ////
//    QMapIterator<QString, QByteArray> iterator_test(result);
//    while (iterator_test.hasNext())
//    {
//        iterator_test.next();
//        qDebug() << iterator_test.key() << '\t' << iterator_test.value();
//    }
    ////

    QString s1,
            s2,
            hashValue1,
            hashValue2;

    QMapIterator<QString, QByteArray> iterator(result);

    if (iterator.hasNext())
        iterator.next();

    hashValue1 = QObject::tr(iterator.value());
    s1 = tr("Source file name: %1\n"
            "Hash value: %2\n")
            .arg(iterator.key())
            .arg(QObject::tr(iterator.value()));

    m_labelHash1->setText(s1);

    if (iterator.hasNext())
        iterator.next();

    hashValue2 = QObject::tr(iterator.value());
    s2 = tr("Destination file name: %1\n"
            "Hash value: %2\n")
            .arg(iterator.key())
            .arg(QObject::tr(iterator.value()));

    m_labelHash2->setText(s2);
    m_labelResultHash->setText(isHashesMatch(hashValue1, hashValue2));
}

void MatchFiles::getError(QString error_message)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::red);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Error message.");
    msgBox.setPalette(pal);
    msgBox.setFont(QFont("Times", 14, QFont::Bold));
    msgBox.setText(error_message);
    msgBox.exec();
}

void MatchFiles::pushbutton_calculate_hash_file()
{
    QString sourceFile = QFileDialog::getOpenFileName(0,
                                                      "Open file for calculate hash.",
                                                      QDir::currentPath(),
                                                      "*.*");
    if (sourceFile.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("File %1 is empty.")
                       .arg(sourceFile));
        msgBox.exec();
        return;
    }

    ///
    kMatchFilesObj->setAlgorithm(m_comboBoxHashAlgoRight->currentIndex());
    kMatchFilesObj->calculateHashFile(sourceFile);
    ///
}

void MatchFiles::getHashFile(QMap<QString, QByteArray> result_file)
{
    QString s1,
            hashValue1;

    QMapIterator<QString, QByteArray> iterator(result_file);

    if (iterator.hasNext())
        iterator.next();

    hashValue1 = QObject::tr(iterator.value());
    s1 = tr("Source file name: %1\n"
            "Hash value: %2\n")
            .arg(iterator.key())
            .arg(QObject::tr(iterator.value()));

    m_labelHashRight->setText(s1);
}
