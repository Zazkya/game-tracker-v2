#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QMap>

class dbmanager
{
public:
    dbmanager();
    ~dbmanager();
    bool isOpen();
    bool dbOpen();
    bool createTable();
    bool createGenreTable();
    bool createLinkerTable();
    bool addEntry(QString name, QString platform, QString developer, QString publisher, QString series, QString deck, QString synopsis, QString image, QString status);
    bool addGenre(QList<QString> list);
    bool autoAddEntry(QMap<QString, QString> map, QList<QString> genreList);
    bool entryExists(QString &name);
    bool updateStatus(QString status, QString name);
    bool updateNote(QString note, QString name);
    bool updateGenre(QString genre, QString name);
    bool updateReleased(QString released, QString name);
    bool updateTime(int time, QString name);
    bool updateMetadata(int metadata, QString name);
    bool updateRating(int rating, QString name);
    bool updateSynopsis(QString synopsis, QString name);
    QSqlQuery queryAll();
    QSqlQuery queryPlatform(QString platform, QString status);
    QSqlQuery queryStatus(QString status);
    QList<QString> getGenre(QString name);
    QSqlDatabase myDb();
    QString nowDate();
    QMap<QString, QString> editQuery(QString name);
    QList<QString> getUniquePlatforms();
    bool updateLinker(QList<QString> genreList, QString name);
    int getGameID(QString name);
    QString getNote(QString name);

private:
    QSqlDatabase m_db;
    QList<QString> platformList;
};

extern dbmanager db;


#endif // DBMANAGER_H
