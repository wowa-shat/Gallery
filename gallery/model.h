#ifndef MODEL_H
#define MODEL_H

#include <QVariant>
#include <QAbstractListModel>

struct Item
{
    QString name;
    QString url;
};

class Model : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);

    enum Roles {
        NameRole = Qt::UserRole,
        UrlRole
    };

    Q_INVOKABLE void scanFolder(QString urlFolder); //заполняет модель
    Q_INVOKABLE void removeItem(QString urlFile); //удаляет файл и соотвутствующий ему элемент модели
    Q_INVOKABLE bool renameFile(int index, QString newName);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QList<Item> m_data; //список над которым идёт обёртка в виде модели
};

#endif // MODEL_H
