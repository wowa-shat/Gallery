#ifndef MYLANG_H
#define MYLANG_H

#include<QLocale>

class MyLang : public QObject
{
    Q_OBJECT
public:
    MyLang(){}

    enum E_Languages
    {
        RUS = QLocale::Russian,
        ENG = QLocale::English
    };

    Q_ENUM(E_Languages)
};

#endif // MYLANG_H
