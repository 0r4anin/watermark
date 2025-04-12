#include "watermark.h"

#include <QApplication>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>

bool processCommandLine(const QCoreApplication &app, WaterMark &w)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Watermark Application");
    parser.addHelpOption(); // автоматически добавляет -h --help

    // Добавляем возможные параметры
    QCommandLineOption watermarkOption(QStringList() << "w" << "watermark",
                                       "Watermark image file", "imagefile");
    QCommandLineOption xOption("x", "X coordinate position", "x", "0");
    QCommandLineOption yOption("y", "Y coordinate position", "y", "0");
    QCommandLineOption scaleOption(QStringList() << "s" << "scale",
                                   "Scale factor (1.0 = 100%)", "scale", "1.0");
    QCommandLineOption alphaOption(QStringList() << "a" << "alpha",
                                   "Transparency (0-100)", "alpha", "100");

    parser.addOption(watermarkOption);
    parser.addOption(xOption);
    parser.addOption(yOption);
    parser.addOption(scaleOption);
    parser.addOption(alphaOption);

    // Парсим аргументы
    if (!parser.parse(app.arguments())) {
        qDebug() << parser.errorText();
        parser.showHelp(1);

    }

    // Если запрошена помощь, показываем и выходим
    if (parser.isSet("help")) {
        parser.showHelp();
        return false;
    }

    // Получаем значения параметров
    QString watermarkFile;
    if (parser.isSet(watermarkOption)) {
        watermarkFile = parser.value(watermarkOption);
        qDebug() << "Watermark file:" << watermarkFile;
    } else {
        qDebug() << "Error: Watermark file is required";
        parser.showHelp(1);
        return false;
    }

    int x = parser.value(xOption).toInt();
    int y = parser.value(yOption).toInt();
    qDebug() << "Position:" << x << y;

    bool scaleOk, alphaOk;
    double scale = parser.value(scaleOption).toDouble(&scaleOk);
    int alpha = parser.value(alphaOption).toInt(&alphaOk);

    if (!scaleOk || scale <= 0) {
        qDebug() << "Error: Invalid scale value";
        parser.showHelp(1);
    }
    qDebug() << "Scale:" << scale;

    if (!alphaOk || alpha < 0 || alpha > 100) {
        qDebug() << "Error: Alpha must be between 0 and 100";
        parser.showHelp(1);
    }
    qDebug() << "Alpha:" << alpha;

    w.initialize(watermarkFile,scale,alpha);
    w.move(x,y);

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WaterMark w;
    if (processCommandLine(a,w))
    {
        w.show();
        w.preventFocus();

    }else
    {
        return 0;
    }
    // WaterMark w("C:\\Qt\\projects\\watermark\\kakashka.png",1.0,50);
    // w.show();
    // Размещаем по центру экрана
    // QScreen *screen = QGuiApplication::primaryScreen();
    // QRect screenGeometry = screen->geometry();
    // w.move(screenGeometry.width()/2 + screenGeometry.x(),
           // screenGeometry.height()/2 + screenGeometry.y());
    // w.move(100,100);
    return a.exec();
}
