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


#include "matchfilesobject.h"


MatchFilesObject::MatchFilesObject(QObject *parent) : QObject(parent)
{

}

MatchFilesObject::~MatchFilesObject()
{

}

QCryptographicHash::Algorithm MatchFilesObject::getAlgorithm() const
{
    return m_hashAlgorithm;
}

void MatchFilesObject::setAlgorithm(const int value)
{
    switch (value)
    {
    case 0:
        m_hashAlgorithm = QCryptographicHash::Md4;
        break;

    case 1:
        m_hashAlgorithm = QCryptographicHash::Md5;
        break;

    case 2:
        m_hashAlgorithm = QCryptographicHash::Sha1;
        break;

    case 3:
        m_hashAlgorithm = QCryptographicHash::Sha224;
        break;

    case 4:
        m_hashAlgorithm = QCryptographicHash::Sha256;
        break;

    case 5:
        m_hashAlgorithm = QCryptographicHash::Sha384;
        break;

    case 6:
        m_hashAlgorithm = QCryptographicHash::Sha512;
        break;

    case 7:
        m_hashAlgorithm = QCryptographicHash::Sha3_224;
        break;

    case 8:
        m_hashAlgorithm = QCryptographicHash::Sha3_256;
        break;

    case 9:
        m_hashAlgorithm = QCryptographicHash::Sha3_384;
        break;

    case 10:
        m_hashAlgorithm = QCryptographicHash::Sha3_512;
        break;

    default:
        m_hashAlgorithm = QCryptographicHash::Sha384;
        break;
    }
}

void MatchFilesObject::calculateHashes(const QString &source, const QString &destination)
{
    QCryptographicHash cryptoHash(getAlgorithm());

    m_sourceFile.setFileName(source);
    if (!m_sourceFile.open(QIODevice::ReadOnly))
    {
        emit setError(tr("Source file %1 for calculation hash not open.")
                      .arg(m_sourceFile.fileName()));
        return;
    }

    while (!m_sourceFile.atEnd())
        cryptoHash.addData(m_sourceFile.read(READ_BLOCK_SIZE_FOR_HASH_DATA));

    if (m_sourceFile.isOpen())
        m_sourceFile.close();

    ///
    m_map.insert(m_sourceFile.fileName(),
                 cryptoHash.result().toHex());
    ///
    cryptoHash.reset();

    m_destinationFile.setFileName(destination);
    if (!m_destinationFile.open(QIODevice::ReadOnly))
    {
        emit setError(tr("Destination file %1 for calculation hash not open.")
                      .arg(m_destinationFile.fileName()));
        return;
    }

    while (!m_destinationFile.atEnd())
        cryptoHash.addData(m_destinationFile.read(READ_BLOCK_SIZE_FOR_HASH_DATA));

    if (m_destinationFile.isOpen())
        m_destinationFile.close();

    ///
    m_map.insert(m_destinationFile.fileName(),
                 cryptoHash.result().toHex());
    ///

    cryptoHash.reset();

    emit setHashResult(m_map);
    m_map.clear();

    emit finished();
}

void MatchFilesObject::calculateHashFile(const QString &source)
{
    QCryptographicHash cryptoHash(getAlgorithm());

    m_sourceFile.setFileName(source);
    if (!m_sourceFile.open(QIODevice::ReadOnly))
    {
        emit setError(tr("Source file %1 for calculation hash not open.")
                      .arg(m_sourceFile.fileName()));
        return;
    }

    while (!m_sourceFile.atEnd())
        cryptoHash.addData(m_sourceFile.read(READ_BLOCK_SIZE_FOR_HASH_DATA));

    if (m_sourceFile.isOpen())
        m_sourceFile.close();

    ///
    m_map.insert(m_sourceFile.fileName(),
                 cryptoHash.result().toHex());
    ///
    cryptoHash.reset();

    emit setHashFileResult(m_map);
    m_map.clear();

    emit finished();
}

