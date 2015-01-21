#ifndef  __SQLITE_DBMANAGER_H_
#define  __SQLITE_DBMANAGER_H_

#include "sqlite3.h"

/**
 * This class maintains a single SQLite3 database and provides 
 * an interface to create and or modify that database without exposing
 * SQLite implementation.
 **/
class SQLiteDBManager
{
  private:

  /** **/
  sqlite3*   db;

  /** **/
  char*      errorMsg;

  /** **/
  int        resultCode,
             ExecSQLCommand(const char* sqlCommand);
  /** **/
  bool       CloseDBPrintFreeError();

  /**
   *
   */
  static int PrintCallback(void *notUsed, int argc, char **argv, char **colName);

  protected:

  public:

  /**
   *
   */
  SQLiteDBManager();

  /**
   *
   */
  bool   CreateNewDataBase(const char* fileName, int numTables, char** sqlTableCommands);

  /**
   *
   */
  bool   ConnectToDataBase(const char* fileName);

  /**
   *
   */
  bool   InsertIntoTable(const char* sqlCommand);

  /**
   *
   */
  bool   DeleteFromTable(const char* sqlCommand);

  /**
   *
   */
  char** QueryTable(const char* sqlCommand, int & rows, int & cols);

  /**
   *
   */
  ~SQLiteDBManager();
};

#endif //__SQLITE_DBMANAGER_H_
