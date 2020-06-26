#include "model.h"
#include "filemanager.h"
#include "mylang.h"
#include "translator.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Model>("Model", 1, 0, "Model");
    qmlRegisterType<MyLang>("MyLang", 1, 0, "MyLang");

    QQmlApplicationEngine engine;
    auto ctx = engine.rootContext();

    FileManager fileManager;
    ctx->setContextProperty("FileManager", QVariant::fromValue(&fileManager));

    Translator translator(&app, &engine);
    ctx->setContextProperty("trans", QVariant::fromValue(&translator));

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
