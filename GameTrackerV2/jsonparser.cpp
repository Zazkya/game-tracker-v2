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
#include <QDir>
#include <QPixmap>

jsonparser::jsonparser(){

}

/**
 * @brief jsonparser::initParser
 * @param gameId
 * takes in gameID
 * request to getJson to get json array
 * creates map of data from json request
 * creates lists for genres and platforms
 */
void jsonparser::initParser(QString gameId){
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


/**
 * @brief jsonparser::getID
 * @return
 * uneeded
 */
QString jsonparser::getID(){

}

/**
 * @brief jsonparser::getName
 * @return name
 */
QString jsonparser::getName(){
    return map["name"];
}

/**
 * @brief jsonparser::getPlatform
 * @return
 * uneeded
 */
QString jsonparser::getPlatform(){

}

/**
 * @brief jsonparser::getDeveloper
 * @return developer
 */
QString jsonparser::getDeveloper(){
    return map["developer"];
}

/**
 * @brief jsonparser::getPublisher
 * @return publisher
 */
QString jsonparser::getPublisher(){
    return map["publisher"];
}

/**
 * @brief jsonparser::getSeries
 * @return franchises
 */
QString jsonparser::getSeries(){
    return map["franchise"];
}

/**
 * @brief jsonparser::getDeck
 * @return deck
 */
QString jsonparser::getDeck(){
    return map["deck"];
}

/**
 * @brief jsonparser::getSynopsis
 * @return description
 */
QString jsonparser::getSynopsis(){
    return map["description"];
}

/**
 * @brief jsonparser::getImage
 * @return image url
 */
QString jsonparser::getImage(){
    return map["image"];
}

/**
 * @brief jsonparser::getMap
 * @return map of all data
 */
QMap<QString, QString> jsonparser::getMap(){
    return map;
}

/**
 * @brief jsonparser::getGenre
 * @return genreList
 */
QList<QString> jsonparser::getGenre(){
    return genreList;
}

/**
 * @brief jsonparser::getPlatformList
 * @return platformList
 */
QList<QString> jsonparser::getPlatformList(){
    return platformList;
}

/**
 * @brief jsonparser::setPlatform
 * @param platform
 * sets a platform to map. used for combox setting
 */
void jsonparser::setPlatform(QString platform){
    map["platform"] = platform;
    qDebug()<<map["platform"];
}

/**
 * @brief getJson
 * @param gameID
 * @return array of json data
 * network request to API
 */
QByteArray getJson(QString gameID){
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString("https://www.giantbomb.com/api/game/" +gameID+ "/?api_key=2eb2277e5d566e4046904bde421a1c9223e09105&field_list=name,deck,developers,themes,franchises,genres,image,platforms,publishers,description%2Cname&format=json") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();
    QByteArray strReply =reply->readAll();
    return strReply;
}

/**
 * @brief parseOverview
 * @param description
 * @return overview from large article of description
 * strips of all html code identifiers
 */
QString parseOverview(QString description){
    QString overview = description.mid(0, description.indexOf("<h2>Gameplay"));
    overview.remove(QRegExp("<[^>]*>"));
    overview.remove(0, 8);
    return overview;
}

/**
 * @brief createMap
 * @param jsonObject
 * @return map of data from json data
 */
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
    if(!franchiseValue.isUndefined()){
        QJsonArray franchiseArray = franchiseValue.toArray();
        if(!franchiseArray.isEmpty()){
            QJsonValue franchises0Value = franchiseArray[0];
            QJsonObject franchises0Object = franchises0Value.toObject();
            map["franchise"] = franchises0Object["name"].toString();
        }
    }

    QJsonValue publisherValue = jsonObject.value("publishers");
    QJsonArray publisherArray = publisherValue.toArray();
    QJsonValue publisher0Value = publisherArray[0];
    QJsonObject publisher0Object = publisher0Value.toObject();
    map["publisher"] = publisher0Object["name"].toString();

    return map;

}

/**
 * @brief imageDownloader
 * @param url of image
 * @param name of game
 * donwloads image form url and creates directory and writes it
 */
void imageDownloader(QString url, QString name){
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
           QString newName = prettyString(name);
           QString myPath = QCoreApplication::applicationDirPath();
           QString folderPath = myPath + "/" + "Images" +"/"+ newName;
           QString path =folderPath + "/" + newName + ".jpg";

           if(url.contains("png",Qt::CaseInsensitive)){
               path = folderPath + "/" + newName + ".png";
           }

           QDir dir(folderPath);

           if(!QDir(folderPath).exists()){
            dir.mkpath(folderPath);
           }


           QFile file(path);
           qDebug()<<path;
           file.open(QIODevice::WriteOnly);
           file.write((responseData));
           file.close();
       }
}

/**
 * @brief imageViewer
 * @param name
 * @return pixmap of picture from file stored
 */
QPixmap imageViewer(QString name){
    QString newName = prettyString(name);
    QString myPath = QCoreApplication::applicationDirPath();
    QString folderPath = myPath + "/" + "Images" +"/"+ newName;
    QString path =folderPath + "/" + newName + ".jpg";

    if(!QFile(path).exists()){
       path = folderPath + "/" + newName + ".png";
    }

    QPixmap pixmap(path);
    QPixmap newp = pixmap.scaled(250,250,Qt::KeepAspectRatio);
    return newp;
}

/**
 * @brief prettyString
 * @param string
 * @return string without punctuation and spaces
 * used for filenames
 */
QString prettyString(QString string){
    string.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")));
    string.remove(" ");
    string.toLower();
    return string;

}


