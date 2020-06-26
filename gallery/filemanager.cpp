#include "filemanager.h"

#include "QFileInfo"
#include "QDateTime"

FileManager::FileManager(QObject *parent)
{

}

QVariantMap FileManager::getInfo()
{
    return fileInfo;
}

void FileManager::changeInfo(QString fileUrl)//к нам приходит строка такого вида "file:///C:/Users/WOWA/Desktop/мои работы/deadline.png" - такой вид адреса к папке в qml
{
    QString startUrlFile = fileUrl;
    fileUrl.replace(0,8,"");
    fileUrl = fileUrl.split("/", QString::SkipEmptyParts).join("\\");
    QFileInfo file(fileUrl); // "C:\Users\WOWA\Desktop\мои работы\deadline.png" - такой вид в c++

    fileInfo["name"] = file.fileName();
    qint64 nSize = file.size();
    qint64 i = 0;
    for (; nSize > 1023; nSize /= 1024, ++i) { }
    fileInfo["size"] = QString().setNum(nSize) + " " + "BKMGT"[i];
    fileInfo["birth"] = file.birthTime().toString();
    fileInfo["mod"] = file.lastModified().toString();
    emit infoChanged();
}
