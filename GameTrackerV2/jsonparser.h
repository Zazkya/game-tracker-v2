#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QVariant>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include "dbmanager.h"

extern dbmanager db;

class jsonparser
{
public:
    jsonparser();
    void initParser(QString gameId);
    QString getID();
    QString getName();
    QString getPlatform();
    QString getDeveloper();
    QString getPublisher();
    QString getSeries();
    QString getDeck();
    QString getSynopsis();
    QString getImage();
    QMap<QString, QString>getMap();
    QList<QString>getGenre();
    QList<QString>getPlatformList();
    void setPlatform(QString platform);

private:
    QMap<QString, QString> map;
    QList<QString> genreList;
    QList<QString> platformList;

};

QByteArray getJson(QString gameID);
QString parseOverview(QString description);
QString parsePublisher(QJsonObject jsonObject);
QString parseImage(QJsonObject jsonObject);
QString parseDeveloper(QJsonObject jsonObject);
QPixmap imageViewer(QString name);
QFile imageConverter();
QMap<QString, QString> createMap(QJsonObject jsonObject);
void imageDownloader(QString url, QString name);
QString prettyString(QString string);

#endif // JSONPARSER_H
