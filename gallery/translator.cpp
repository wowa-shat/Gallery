#include "translator.h"
#include "mylang.h"

Translator::Translator(QGuiApplication *app, QQmlApplicationEngine *engine)
{
    mApp = app;
    mEngine = engine;
}

QString Translator::getEmptyString()
{
    return "";
}

void Translator::updateLanguage(int language)
{
    switch(language)
    {
    case MyLang::ENG:
        mTranslator.load("gallery_ENG.qm", ":/translator");
        mApp->installTranslator(&mTranslator);
        mEngine->retranslate();
        break;

    case MyLang::RUS:
        mTranslator.load("gallery_RUS.qm", ":/translator");
        mApp->installTranslator(&mTranslator);
        mEngine->retranslate();
        break;
    }
    emit languageChanged();
}
