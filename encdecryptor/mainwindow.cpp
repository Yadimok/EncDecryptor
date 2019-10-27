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


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("EncDecryptor, stream ciphers");

    mainBoxLayout = new QHBoxLayout();
    tabWidget = new QTabWidget();
    mainBoxLayout->addWidget(tabWidget);
    ui->centralWidget->setLayout(mainBoxLayout);

    kARC4 = new ARC4Dialog();
    kRabbit = new RabbitDialog();
    kSalsa20 = new Salsa20Dialog();

    tabWidget->addTab(kARC4, "Alleged RC4");
    tabWidget->addTab(kRabbit, "Rabbit");
    tabWidget->addTab(kSalsa20, "Salsa20");

    kAbout = new About();

    kMatchFiles = new MatchFiles();

    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete kARC4;
    delete kRabbit;
    delete kSalsa20;
    delete kAbout;
    delete kMatchFiles;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool bOk = QMessageBox::question(this, "EncDecryptor", "Quit application?", "Yes", "No", QString(), 0, 1);
    if (!bOk)
    {
        if (kAbout->isVisible())
            kAbout->close();

        if (kMatchFiles->isVisible())
            kMatchFiles->close();

        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    event->size().width();
    event->size().height();
    event->accept();
}

void MainWindow::readSettings()
{
    QSettings settings("EncDecryptor.ini", QSettings::IniFormat);
    settings.beginGroup("window");
    QPoint pos  = settings.value("pos", QPoint(300, 100)).toPoint();
    QSize size  = settings.value("size", QSize(800, 600)).toSize();
    settings.endGroup();

    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("EncDecryptor.ini", QSettings::IniFormat);
    settings.beginGroup("window");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

void MainWindow::on_actionExit_triggered()
{
//    qApp->exit();
}

void MainWindow::on_actionAbout_triggered()
{
    if (kAbout->isVisible())
        kAbout->hide();
    else
        kAbout->show();
}

void MainWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionMatchFiles_triggered()
{
    if (kMatchFiles->isVisible())
        kMatchFiles->hide();
    else
        kMatchFiles->show();
}
