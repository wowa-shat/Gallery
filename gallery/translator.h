#ifndef MYTRANSLATOR_H
#define MYTRANSLATOR_H

#include<QGuiApplication>
#include<QQmlApplicationEngine>
#include<QTranslator>

class Translator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString emptyString READ getEmptyString NOTIFY languageChanged) //свойство для биндинга переводов

public:
    Translator(QGuiApplication* app, QQmlApplicationEngine* engine);

    QString getEmptyString();

signals:
    void languageChanged();

public slots:
    void updateLanguage(int language);

private:
    QQmlApplicationEngine* mEngine;
    QGuiApplication* mApp;
    QTranslator mTranslator;
};

#endif // MYTRANSLATOR_H
