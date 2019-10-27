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


#ifndef MATCHFILESOBJECT_H
#define MATCHFILESOBJECT_H

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QCryptographicHash>

class MatchFilesObject : public QObject
{
    Q_OBJECT

    enum {
        READ_BLOCK_SIZE_FOR_HASH_DATA = 8192
    };

    QCryptographicHash::Algorithm   m_hashAlgorithm;
    QFile                           m_sourceFile;
    QFile                           m_destinationFile;
    QMap<QString, QByteArray>       m_map;
    QCryptographicHash::Algorithm getAlgorithm() const;

public:
    explicit MatchFilesObject(QObject *parent = 0);
    virtual ~MatchFilesObject();

    void setAlgorithm(const int value);
    void calculateHashes(const QString &source, const QString &destination);
    void calculateHashFile(const QString &source);

signals:
    void setHashResult(QMap<QString, QByteArray> result);
    void setHashFileResult(QMap<QString, QByteArray> result_file);
    void setError(QString error_str);

    void finished();

};

#endif // MATCHFILESOBJECT_H
