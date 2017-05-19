#include "jsonparser.h"
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
#include <QFile>

jsonparser::jsonparser(QString gameId){
    QByteArray jsonArray = getJson(gameId);
    QJsonDocument doc(QJsonDocument::fromJson(jsonArray));
    QJsonObject json = doc.object();
    QJsonValue value = json.value("results");
    QJsonObject resultsObject = value.toObject();
    map = createMap(resultsObject);


    QJsonValue genreValue = resultsObject.value("genres");
    QJsonArray tempArray = genreValue.toArray();
    for(int i = 0; i < tempArray.count(); i++){
       QJsonValue tempGenreValue = tempArray[i];
       QJsonObject tempGenreObject = tempGenreValue.toObject();
       genreList.append(tempGenreObject["name"].toString());
    }



    QJsonValue platformValue = resultsObject.value("platforms");
    QJsonArray tempPlatformArray = platformValue.toArray();
    for(int i = 0; i < tempPlatformArray.count(); i++){
        QJsonValue tempPlatformValue = tempPlatformArray[i];
        QJsonObject tempPlatformObject = tempPlatformValue.toObject();
        platformList.append(tempPlatformObject["name"].toString());
    }


}



QString jsonparser::getID()
{

}

QString jsonparser::getName()
{
    return map["name"];
}

QString jsonparser::getPlatform()
{

}

QString jsonparser::getDeveloper()
{
    return map["developer"];
}

QString jsonparser::getPublisher()
{
    return map["publisher"];
}

QString jsonparser::getSeries()
{
    return map["franchise"];
}

QString jsonparser::getDeck()
{
    return map["deck"];
}

QString jsonparser::getSynopsis()
{
    return map["description"];
}

QString jsonparser::getImage()
{
    return map["image"];
}

QByteArray getJson(QString gameID){
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString("https://www.giantbomb.com/api/game/3030-4725/?api_key=2eb2277e5d566e4046904bde421a1c9223e09105&field_list=name,deck,developers,themes,franchises,genres,image,platforms,publishers,description%2Cname&format=json") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();
    QByteArray strReply =reply->readAll();
    return strReply;
}



QString parseOverview(QString description){
    QString overview = description.mid(0, description.indexOf("<h2>Gameplay"));
    overview.remove(QRegExp("<[^>]*>"));
    return overview;
}

QMap<QString, QString> createMap(QJsonObject jsonObject){
    QMap<QString, QString> map;
    map["name"] = jsonObject["name"].toString();
    map["deck"] = jsonObject["deck"].toString();
    map["description"] = parseOverview(jsonObject["description"].toString());



    QJsonValue imageValue = jsonObject.value("image");
    QJsonObject imageObject = imageValue.toObject();
    map["image"] = imageObject["small_url"].toString();

    QJsonValue developerValue = jsonObject.value("developers");
    QJsonArray array = developerValue.toArray();
    QJsonValue developer0Value = array[0];
    QJsonObject developer0Object = developer0Value.toObject();
    map["developer"] = developer0Object["name"].toString();

    QJsonValue franchiseValue = jsonObject.value("franchises");
    QJsonArray franchiseArray = franchiseValue.toArray();
    QJsonValue franchises0Value = franchiseArray[0];
    QJsonObject franchises0Object = franchises0Value.toObject();
    map["franchise"] = franchises0Object["name"].toString();

    QJsonValue publisherValue = jsonObject.value("publishers");
    QJsonArray publisherArray = publisherValue.toArray();
    QJsonValue publisher0Value = publisherArray[0];
    QJsonObject publisher0Object = publisher0Value.toObject();
    map["publisher"] = publisher0Object["name"].toString();
    //TODO: Add other fields.

    return map;

}

void imageDownloader(QString url){
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( (QUrl(QString(url))));
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();
    if(reply->error() != QNetworkReply::NoError)
     {
         qDebug()<<"ERROR!!!!!!!!!!!!!!";
     }
    else
       {
           QByteArray responseData = reply->readAll();

           QString myPath = QCoreApplication::applicationDirPath();
           QString path = myPath + "/example.png";

           QFile file(path);
           file.open(QIODevice::WriteOnly);
           file.write((responseData));
           file.close();
           qDebug()<<"DONE!!!!!!!!!!!";
       }

}




