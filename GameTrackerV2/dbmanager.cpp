#include "dbmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDate>
#include <QCoreApplication>


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
 * @brief DbManager::createTable
 * create table
 * @return
 * return tru if table is created
 */
bool dbmanager::createTable(){
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS gameTable(id INTEGER PRIMARY KEY, name TEXT NOT NULL, platform TEXT NOT NULL, status TEXT NOT NULL, dateAdded TEXT NOT NULL, dateReleased TEXT NOT NULL, dateModified TEXT NOT NULL, eTime INTEGER NOT NULL, genre TEXT NOT NULL, notes CLOB NOT NULL, metadata INTEGER NOT NULL, rating INTEGER NOT NULL, synopsis CLOB NOT NULL);");

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
bool dbmanager::addEntry(){
    bool success = false;
    //TODO:Add method to add to db. Do after api config

    return success;
}

/**
 * @brief DbManager::entryExists
 * check if entry exists
 * @param name
 * name of entry
 * @return
 * return tru if exists
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
    QSqlQuery query;
    query.prepare("SELECT name, platform, genre, status, dateAdded, dateModified, eTime, rating FROM gameTable");
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
    query.prepare("SELECT name, platform, genre, status, dateAdded, dateModified FROM gameTable WHERE status = (:status) AND platform = (:platform)");
    query.bindValue(":status", status);
    query.bindValue(":platform", platform);
    query.exec();
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
    query.prepare("SELECT name, platform, genre, dateAdded, dateModified FROM gameTable WHERE status = (:status) ");
    query.bindValue(":status", status);
    query.exec();
    return query;
}

/**
 * @brief DbManager::queryGenre
 * queries genre of games - used for sorting
 * @return
 * returns query
 */
QSqlQuery dbmanager::queryGenre(QString genre, QString status){
    QSqlQuery query;
    query.prepare("SELECT name, platform, genre, status, dateAdded, dateModified FROM gameTable WHERE status = (:status) AND genre = (:genre)");
    query.bindValue(":status", status);
    query.bindValue(":genre", genre);
    query.exec();
    return query;
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
    checkQuery.prepare("UPDATE gameTable SET note = (:note), dateModified = (:modified) WHERE name = (:name)");
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
