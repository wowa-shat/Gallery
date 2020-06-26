#include "model.h"

#include <QDir>

Model::Model(QObject *parent) : QAbstractListModel(parent)
{

}

int Model::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const Item item = m_data.at(index.row());
    switch (role) {
    case NameRole:
        return QVariant(item.name);
    case UrlRole:
        return QVariant(item.url);
    }

    return QVariant();
}

QHash<int, QByteArray> Model::roleNames() const
{
    QHash<int, QByteArray> names;

    names[NameRole] = "name";
    names[UrlRole] = "url";

    return names;
}

void Model::scanFolder(QString urlFolder) //к нам приходит строка такого вида "file:///C:/Users/WOWA/Desktop/мои работы" - такой вид адреса к папке в qml
{
    QString startUrlFolder = urlFolder;
    urlFolder.replace(0,8,"");
    urlFolder = urlFolder.split("/", QString::SkipEmptyParts).join("\\");
    QDir folder(urlFolder); // "C:\\Users\\WOWA\\Desktop\\мои работы" - такой вид в c++

    if (folder.exists())
    {
        folder.setFilter(QDir::Files); //берём только файлы

        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.bmp";
        folder.setNameFilters(filters); //добавляем фильтры, чтобы выбирались только изображения

        folder.setSorting(QDir::Name);
        QFileInfoList listNames = folder.entryInfoList(); //выбираем только имена файлов

        beginResetModel();
        m_data.clear();
        for (int i = 0; i < listNames.size(); ++i)
        {
            QFileInfo fileInfo = listNames.at(i);
            m_data.append({ fileInfo.fileName(), startUrlFolder + "/" + fileInfo.fileName() });
        }
        endResetModel();
    }
}

void Model::removeItem(QString urlFile)
{
    for (int i = 0; i < m_data.size(); ++i)
    {
        if (m_data[i].url == urlFile)
        {
            beginRemoveRows(QModelIndex(), i,i);
            m_data.removeAt(i);
            endRemoveRows();
        }
    }
    urlFile.replace(0,8,"");
    urlFile = urlFile.split("/", QString::SkipEmptyParts).join("\\");
    QFile(urlFile).remove();
}

bool Model::renameFile(int index, QString newName)
{
    QString qmlUrlWithOldName = m_data[index].url; //  file: ///C:/Users/WOWA/Desktop/мои работы/test/pixel-64.png - Qml

    QStringList qmlUrlList = qmlUrlWithOldName.split("/");
    qmlUrlList.removeLast();

    qmlUrlWithOldName.replace(0,8,"");
    QStringList cppUrlList = qmlUrlWithOldName.split("/", QString::SkipEmptyParts);
    QString urlWithOldNameCpp = cppUrlList.join("\\");
    cppUrlList.removeLast();

    if (QFile::rename(urlWithOldNameCpp, cppUrlList.join("\\") + "\\" + newName))
    {
        m_data[index].name = newName;
        m_data[index].url = qmlUrlList.join("/") + "/" + newName;
        emit dataChanged(createIndex(index,0), createIndex(index,0));
        return true;
    }
    return false;
}
