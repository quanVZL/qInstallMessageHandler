/**************************************************************************************************
* https://blog.csdn.net/liang19890820/article/details/51838096
* http://blog.sina.com.cn/s/blog_a6fb6cc90101guc5.html
**************************************************************************************************/

#include <QGuiApplication>
#include <QQuickView>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QMutex>
#include <QFileInfo>

static const int MAX_LOG_FILE_SIZE = 4 * 1024 * 1024;

void writeToFile(const QByteArray &log) {
    static QMutex mutex;

    mutex.lock();
    QFile file("log.txt");
    QFileInfo info(file);

    if (file.open(QIODevice::ReadWrite | QIODevice::Append | QFile::Text)) {
        file.write(log);
        file.write("\n");
        file.flush();
        file.close();
        if (info.size() > MAX_LOG_FILE_SIZE) {
            auto data = file.readAll();
            file.resize(0);
            data = data.right(MAX_LOG_FILE_SIZE);
            file.write(data);
            file.flush();
            file.close();
        }
    }

    mutex.unlock();
}

void messageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            writeToFile(localMsg.constData());
            break;
        case QtInfoMsg:
            fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            writeToFile(localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            writeToFile(localMsg.constData());
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            writeToFile(localMsg.constData());
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            writeToFile(localMsg.constData());
            abort();

    }
}

int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);

    qInstallMessageHandler(messageOutPut);

    qDebug() << "Debug message";
    qInfo() << "qInfo message";
    qWarning() << "qWarning message";
    qCritical() << "qCritical message";
    qFatal("qFatal message");

    return app.exec();
}



//    QQuickView viewer;
//    viewer.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
//    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
//    viewer.show();
