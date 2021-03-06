#include "dbmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDate>
#include <QCoreApplication>
#include "jsonparser.h"


/**
 * @brief DbManager::DbManager
 * Connects to db
 * @param path
 * path of db
 */
dbmanager::dbmanager(){

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString myPath = QCoreApplication::applicationDirPath();
    QString path = myPath + "/example.db";
    m_db.setDatabaseName(path);
    m_db.open();
    //TODO: add message for failed connect
}

/**
 * @brief DbManager::~DbManager
 * closes db connection
 */
dbmanager::~dbmanager(){
    if (m_db.isOpen()){
        m_db.close();
    }
}

/**
 * @brief DbManager::isOpen
 * check if db is open
 * @return
 * return answer
 */
bool dbmanager::isOpen(){
    return m_db.isOpen();
}

/**
 * @brief dbmanager::dbOpen
 * @return true if db is open
 */
bool dbmanager::dbOpen(){
   return m_db.open();
}

/**
 * @brief DbManager::createTable
 * create table
 * @return
 * return tru if table is created
 */
bool dbmanager::createTable(){
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS gameTable(id INTEGER PRIMARY KEY, name TEXT NOT NULL, platform TEXT NOT NULL, status TEXT NOT NULL, dateAdded TEXT NOT NULL, dateModified TEXT NOT NULL, notes CLOB , rating INTEGER, synopsis CLOB , developer TEXT, publisher TEXT, series TEXT, deck TEXT, image TEXT, gameid INTEGER, UNIQUE(name));");

    if(!query.exec()){
        success = false;
    }else success = true;

    return success;
}

/**
 * @brief dbmanager::createGenreTable
 * @return creates genre table
 */
bool dbmanager::createGenreTable(){
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS genreTable(genreid INTEGER PRIMARY KEY, genre TEXT, UNIQUE(genre));");

    if(!query.exec()){
        success = false;
    }else success = true;

    return success;
}

/**
 * @brief dbmanager::createLinkerTable
 * @return creates Linker Table
 */
bool dbmanager::createLinkerTable(){
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS linkerTable(linkerid INTEGER PRIMARY KEY, gameid INTEGER ,genreid INTEGER);");

    if(!query.exec()){
        success = false;
    }else success = true;

    return success;
}

/**
 * @brief DbManager::addEntry
 * add entry to db
 * @return
 * returns true if successfull
 */
bool dbmanager::addEntry(QString name, QString platform, QString developer, QString publisher, QString series, QString deck, QString synopsis, QString image, QString status ){
    bool success = false;

    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO gameTable(id, name, platform,developer,publisher,series,deck,synopsis,image,dateModified,dateAdded,status) VALUES (:id, :name,:platform,:developer,:publisher,:series,:deck,:synopsis,:image,:dateModified,:dateAdded, :status)");

    query.bindValue(":name", name);
    query.bindValue(":platform", platform);
    query.bindValue(":status", status);
    query.bindValue(":dateAdded", this->nowDate());
    query.bindValue(":dateModified", this->nowDate());
    query.bindValue(":synopsis", synopsis);
    query.bindValue(":deck", deck);
    query.bindValue(":developer", developer);
    query.bindValue(":publisher", publisher);
    query.bindValue(":series", series);
    query.bindValue(":image", image);

    if(!query.exec()){
        success = false;
    }else success = true;

    QSqlQuery query2;
    query2.prepare("SELECT id from gameTable WHERE name = (:name)");
    query2.bindValue(":name", name);
    query2.exec();
    query2.next();

    QSqlQuery query3;
    query3.prepare("UPDATE gameTable SET gameid  = (:gameid) WHERE name = (:name)");
    query3.bindValue(":gameid", query2.value(0).toInt());
    query3.bindValue(":name", name);
    query3.exec();


    return success;
}

/**
 * @brief dbmanager::addGenre
 * @param list of genres
 * @return sets genres into genre table if not duplicate and adds in links in linker table
 */
bool dbmanager::addGenre(QList<QString> list){
    bool success = false;

    for(int i =0; i < list.size(); i++ ){
        QSqlQuery query;
        query.prepare("INSERT OR IGNORE INTO genreTable(genre) VALUES(:genre);");
        query.bindValue(":genre", list[i]);
        if(!query.exec()){
            success = false;
        }else success = true;
    }
}

/**
 * @brief dbmanager::autoAddEntry
 * @param map of data fields
 * @param genreList
 * @return adds data from map and genreList to db
 */
bool dbmanager::autoAddEntry(QMap<QString, QString> map, QList<QString> genreList){
    bool success = false;
    success = addEntry(map["name"], map["platform"], map["developer"], map["publisher"], map["franchise"], map["deck"],map["description"], map["image"], "Unfinished" );

    this->addGenre(genreList);
    this->updateLinker(genreList, map["name"]);


    return success;
}

/**
 * @brief DbManager::entryExists
 * check if entry exists
 * @param name
 * name of entry
 * @return
 * return true if exists
 */
bool dbmanager::entryExists(QString &name){
    bool exists = false;
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT name FROM gameTable WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    if (checkQuery.exec()){
          if (checkQuery.next()){
              exists = true;
          }
      }
    return exists;
}

/**
 * @brief DbManager::queryAll
 * creates query for everything in db
 * @return
 * returns query
 */
QSqlQuery dbmanager::queryAll(){
    QSqlQuery query(QSqlDatabase::database("QSQLITE"));
    query.prepare("SELECT name, platform, status, dateAdded, dateModified, notes FROM gameTable");
    query.exec();
    return query;
}

/**
 * @brief DbManager::queryPlatform
 * queries platform - used for sorting
 * @return
 * returns query
 */
QSqlQuery dbmanager::queryPlatform(QString platform, QString status){

    QSqlQuery query;
    if(status == "All" && platform != "All Platforms"){
        query.prepare("SELECT name, platform, status, dateAdded, dateModified, notes FROM gameTable WHERE platform = (:platform)");
        query.bindValue(":platform", platform);
        query.exec();
    }else if (status == "All" && platform == "All Platforms") {
        query = this->queryAll();
    }else if (status != "All" && platform == "All Platforms") {
        query = this->queryStatus(status);
    }else{
        query.prepare("SELECT name, platform, status, dateAdded, dateModified, notes FROM gameTable WHERE status = (:status) AND platform = (:platform)");
        query.bindValue(":status", status);
        query.bindValue(":platform", platform);
        query.exec();
    }

    return query;
}


/**
 * @brief DbManager::queryStatus
 * queries status of game
 * @param status
 * Complete, Incomplete, Retired
 * @return
 * returns query
 */
QSqlQuery dbmanager::queryStatus(QString status){
    QSqlQuery query;
    query.prepare("SELECT name, platform, status, dateAdded, dateModified, notes FROM gameTable WHERE status = (:status) ");
    query.bindValue(":status", status);
    query.exec();
    return query;
}

QList<QString> dbmanager::getGenre(QString name){
    QList<QString> list;

    QSqlQuery query;
    query.prepare("SELECT genreTable.genre FROM genreTable INNER JOIN linkerTable ON linkerTable.genreid = genreTable.genreid WHERE linkerTable.gameid = (:gameid)");
    query.bindValue(":gameid", this->getGameID(name));
    query.exec();

    while (query.next()) {
        list.append(query.value(0).toString());
    }




    return list;
}


/**
 * @brief DbManager::myDb
 * database obgect - used for QT display
 * @return
 * returns m_db
 */
QSqlDatabase dbmanager::myDb(){
    return m_db;
}

/**
 * @brief DbManager::updateStatus
 * @param status
 * @param name
 * @return
 */
bool dbmanager::updateStatus(QString status, QString name){
    bool success = false;
    QString modified = this->nowDate();

    QSqlQuery checkQuery;
    checkQuery.prepare("UPDATE gameTable SET status = (:status), dateModified = (:modified) WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":modified", modified);
    checkQuery.bindValue(":status", status);

    if(checkQuery.exec()){
        success = true;
    }
    return success;
}

/**
 * @brief DbManager::updateNote
 * @param note
 * @param name
 * @return
 */
bool dbmanager::updateNote(QString note, QString name){
    bool success = false;
    QString modified = this->nowDate();
    QSqlQuery checkQuery;
    checkQuery.prepare("UPDATE gameTable SET notes = (:note), dateModified = (:modified) WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":modified", modified);
    checkQuery.bindValue(":note", note);

    if(checkQuery.exec()){
        success = true;

    }
    return success;
}
/**
 * @brief DbManager::updateGenre
 * @param genre
 * @param name
 * @return
 */
bool dbmanager::updateGenre(QString genre, QString name){
    bool success = false;
    QString modified = this->nowDate();
    QSqlQuery checkQuery;
    checkQuery.prepare("UPDATE gameTable SET genre = (:genre), dateModified = (:modified) WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":modified", modified);
    checkQuery.bindValue(":genre", genre);

    if(checkQuery.exec()){
        success = true;
    }
    return success;
}

/**
 * @brief DbManager::updateReleased
 * @param released
 * @param name
 * @return
 */
bool dbmanager::updateReleased(QString released, QString name){
    bool success = false;
    QString modified = this->nowDate();
    QSqlQuery checkQuery;
    checkQuery.prepare("UPDATE gameTable SET released = (:released), dateModified = (:modified) WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":modified", modified);
    checkQuery.bindValue(":released", released);

    if(checkQuery.exec()){
        success = true;
    }
    return success;

}

/**
 * @brief DbManager::updateTime
 * @param time
 * @param name
 * @return
 */
bool dbmanager::updateTime(int time, QString name){
    bool success = false;
    QString modified = this->nowDate();
    QSqlQuery checkQuery;
    checkQuery.prepare("UPDATE gameTable SET time = (:time), dateModified = (:modified) WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":modified", modified);
    checkQuery.bindValue(":time", time);

    if(checkQuery.exec()){
        success = true;
    }
    return success;

}

/**
 * @brief DbManager::updateMetadata
 * @param metadata
 * @param name
 * @return
 */
bool dbmanager::updateMetadata(int metadata, QString name){
    bool success = false;
    QString modified = this->nowDate();
    QSqlQuery checkQuery;
    checkQuery.prepare("UPDATE gameTable SET metadata = (:metadata), dateModified = (:modified) WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":modified", modified);
    checkQuery.bindValue(":metadata", metadata);

    if(checkQuery.exec()){
        success = true;
    }
    return success;

}

/**
 * @brief DbManager::updateRating
 * @param rating
 * @param name
 * @return
 */
bool dbmanager::updateRating(int rating, QString name){
    bool success = false;
    QString modified = this->nowDate();
    QSqlQuery checkQuery;
    checkQuery.prepare("UPDATE gameTable SET rating = (:rating), dateModified = (:modified) WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":modified", modified);
    checkQuery.bindValue(":rating", rating);

    if(checkQuery.exec()){
        success = true;
    }
    return success;

}

/**
 * @brief DbManager::updateSynopsis
 * @param synopsis
 * @param name
 * @return
 */
bool dbmanager::updateSynopsis(QString synopsis, QString name){
    bool success = false;
    QString modified = this->nowDate();
    QSqlQuery checkQuery;
    checkQuery.prepare("UPDATE gameTable SET synopsis = (:synopsis), dateModified = (:modified) WHERE name = (:name)");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":modified", modified);
    checkQuery.bindValue(":synopsis", synopsis);

    if(checkQuery.exec()){
        success = true;
    }
    return success;

}

/**
 * @brief DbManager::nowDate
 * getting the current date for dateModified
 * @return
 * returns current date as string
 */
QString dbmanager::nowDate(){
    QDate date;
    date = QDate::currentDate();
    QString date_string;
    date_string = date.toString("dd/MM/yyyy");
    return date_string;

}

QMap<QString, QString> dbmanager::editQuery(QString name){
    QSqlQuery query;
    query.prepare("SELECT name, platform, status, notes FROM gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    QMap<QString, QString> dbMap;



    dbMap["name"] = query.value("name").toString();
    dbMap["platform"] = query.value("platform").toString();
    dbMap["status"] = query.value("status").toString();
    dbMap["notes"] = query.value("notes").toString();



    return dbMap;
}

QList<QString> dbmanager::getUniquePlatforms(){
    QSqlQuery query;
    query.prepare("SELECT DISTINCT platform from gameTable");
    query.exec();

    platformList << "All Platforms";
    while (query.next()) {
        platformList.append(query.value(0).toString());
    }

    return platformList;
}

bool dbmanager::updateLinker(QList<QString> genreList, QString name){
    QSqlQuery query;
    QSqlQuery query2;

    for(int i = 0; i < genreList.count(); i++){
        query.prepare("SELECT genreid FROM genreTable WHERE genre = (:genre)");
        query.bindValue(":genre", genreList[i]);
        query.exec();
        query.next();

        query2.prepare("INSERT INTO linkerTable(gameid, genreid) VALUES(:gameid, :genreid)");
        query2.bindValue(":gameid", this->getGameID(name));
        query2.bindValue(":genreid", query.value(0));
        query2.exec();
    }
}

int dbmanager::getGameID(QString name){

    QSqlQuery query;
    query.prepare("SELECT gameid FROM gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();

    return query.value(0).toInt();

}

QString dbmanager::getNote(QString name){
    QSqlQuery query;
    query.prepare("SELECT notes from gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    return query.value(0).toString();
}

QString dbmanager::getSynopsis(QString name){
    QSqlQuery query;
    query.prepare("SELECT synopsis FROM gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    return query.value(0).toString();

}

QString dbmanager::getPlatform(QString name){
    QSqlQuery query;
    query.prepare("SELECT platform FROM gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();

    return query.value(0).toString();

}

QString dbmanager::getDeveloper(QString name){
    QSqlQuery query;
    query.prepare("SELECT developer FROM gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    return query.value(0).toString();
}

QString dbmanager::getPublisher(QString name){
    QSqlQuery query;
    query.prepare("SELECT publisher FROM gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    return query.value(0).toString();

}

QString dbmanager::getSeries(QString name){
    QSqlQuery query;
    query.prepare("SELECT series FROM gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    return query.value(0).toString();

}

QString dbmanager::getDeck(QString name){
    QSqlQuery query;
    query.prepare("SELECT deck FROM gameTable WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    return query.value(0).toString();
}

QString dbmanager::getName(){
    QSqlQuery query;
    query.prepare("SELECT id FROM gameTable WHERE id = (:id");
    query.bindValue(":id", 1);
    query.exec();
    query.next();
    return query.value(0).toString();
}























