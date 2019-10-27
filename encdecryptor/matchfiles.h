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


#ifndef MATCHFILES_H
#define MATCHFILES_H

#include <QDialog>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QComboBox>
#include <QtCore/QMap>
#include <QtCore/QThread>
#include <QtWidgets/QTabWidget>

#include "matchfilesobject.h"

namespace Ui {
class MatchFiles;
}

class MatchFiles : public QDialog
{
    Q_OBJECT

public:
    explicit MatchFiles(QWidget *parent = 0);
    ~MatchFiles();

private:
    Ui::MatchFiles *ui;

    MatchFilesObject    *kMatchFilesObj;
    QThread             *kThread;

    QTabWidget  *m_tabWidget;
    QGridLayout *m_gridLayout;

    ///tab #1
    QBoxLayout  *m_boxLayout;
    QGroupBox   *m_groupBox;
    QPushButton *m_pushbutton;
    QLabel      *m_labelHash1,
                *m_labelHash2,
                *m_labelResultHash;
    QComboBox   *m_comboBoxHashAlgo;

    ///tab #2
    QBoxLayout  *m_boxLayoutRight;
    QGroupBox   *m_groupBoxRight;
    QPushButton *m_pushButtonRight;
    QLabel      *m_labelHashRight;
    QComboBox   *m_comboBoxHashAlgoRight;

    QString isHashesMatch(const QString &source, const QString &destination) const;

private slots:
    void pushbutton_match_files();
    void pushbutton_calculate_hash_file();

public slots:
    void getHashResult(QMap<QString, QByteArray> result);
    void getHashFile(QMap<QString, QByteArray> result_file);
    void getError(QString error_message);

};

#endif // MATCHFILES_H
