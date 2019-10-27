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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets>
#include <QtCore>

#include "matchfiles.h"
#include "about.h"

#include "ARC4/arc4dialog.h"
#include "Rabbit/rabbitdialog.h"
#include "Salsa20/salsa20dialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();

    void on_actionMatchFiles_triggered();

private:
    Ui::MainWindow *ui;

    QHBoxLayout     *mainBoxLayout;
    QTabWidget      *tabWidget;

    void readSettings();
    void writeSettings();

    //
    MatchFiles      *kMatchFiles;
    About           *kAbout;
    ARC4Dialog      *kARC4;
    RabbitDialog    *kRabbit;
    Salsa20Dialog   *kSalsa20;

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
