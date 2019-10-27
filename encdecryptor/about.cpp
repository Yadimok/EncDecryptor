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


#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);


    m_label = new QLabel();
    m_label->setFont(QFont("DejaVu Sans", 12));
    m_label->setText("The EncDecryptor demonstrates how to encrypt/decrypt message or file('s). \n"
                     "Author - Yadimok, 2016. \n"
                     "Link - https://bitbucket.org/Yadimok2016/encdecryptor");
    m_vBoxLayout = new QVBoxLayout();
    m_vBoxLayout->addWidget(m_label);

    setWindowTitle("About EncDecryptor.");
    setLayout(m_vBoxLayout);
}

About::~About()
{
    delete ui;
}
