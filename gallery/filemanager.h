#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "QVariantMap"


class FileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap fileInfo READ getInfo NOTIFY infoChanged)
public:
    explicit FileManager(QObject *parent = 0);

    QVariantMap getInfo();

    Q_INVOKABLE void changeInfo(QString fileUrl);//собирает в fileInfo информацию о файле

signals:
    void infoChanged();

private:
    QVariantMap fileInfo; //словарь с информацией о файле, который последним был послан в метод changeInfo
};
#endif // FILEMANAGER_H
