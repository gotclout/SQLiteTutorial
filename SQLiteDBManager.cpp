#include "SQLiteDBManager.h"
#include <iostream>
#include <fstream>

#define null 0
#define SQLITE_OK 0

using namespace std;

bool SQLiteDBManager::CloseDBPrintFreeError()
{
  sqlite3_close(db);
  if(errorMsg)
  {
    cout << errorMsg << endl;
    sqlite3_free(errorMsg);
  }

  return false;
}

int SQLiteDBManager::PrintCallback(void *notUsed, int argc, char **argv, char **colName)
{
  int retVal = 0;
  int i = 0;
  for(i=0; i<argc; i++)
  {
    cout << colName[i] << argv[i] ? argv[i] : "NULL";
    }
  if(argc > 0)
  {
    cout << endl;
  }
  return retVal;
}

SQLiteDBManager::SQLiteDBManager()
{
  sqlite3* db = null;
  char*    errorMsg = null;
}

SQLiteDBManager::~SQLiteDBManager()
{
  if(db)
  {
    sqlite3_free(db);
    db = null;
  }
  if(errorMsg)
  {
    sqlite3_free(errorMsg);
    errorMsg = null;
  }
}

int SQLiteDBManager::ExecSQLCommand(const char* sqlCommand)
{
  return sqlite3_exec(db, sqlCommand, PrintCallback, null, &errorMsg);
}

bool SQLiteDBManager::CreateNewDataBase(const char* fileName, int numTables, char** sqlTableCommands)
{
  bool retVal = true;
  bool fileExists = false;
  int i = 0;
  ifstream file;

  file.open(fileName);
  if(!file)
  {
    file.close();
    fileExists = false;
  }
  else
  {
    file.close();
    fileExists = true;
    if(numTables > 0)
    {
      std::remove(fileName);
      fileExists = false;
    }
    else
    {
      cout << "A file with the name " << fileName 
         << " already exist, and you have chosen not to overwrite it "
         << " so the DB was not created." << endl;
      retVal = false;
    }
  }

  if(!retVal)
  {
    resultCode = sqlite3_open(fileName, &db);
    if( resultCode )
    {
      retVal = CloseDBPrintFreeError();
    }

    resultCode = ExecSQLCommand("PRAGMA temp_store = MEMORY;");
    if( resultCode )
    {
      sqlite3_close(db);
      if(errorMsg)
      {
        cout << errorMsg << endl;
        sqlite3_free(errorMsg);
      }
      retVal = false;
    }

    resultCode = ExecSQLCommand("PRAGMA synchronous = OFF;");
    if( resultCode )
    {
      retVal = CloseDBPrintFreeError();
    }

    if(fileExists && numTables > 0)
    {
      for(i = 0; i < numTables; i++)
      {
        resultCode = ExecSQLCommand(sqlTableCommands[i]);
        if( resultCode )
        {
          retVal = CloseDBPrintFreeError();
        }  
      }
    }
  }
  return retVal;
}

bool SQLiteDBManager::ConnectToDataBase(const char* fileName)
{
  bool retVal = true;
  ifstream file;
  file.open(fileName);
  if(!file)
  {
    retVal = false;
    file.close();
  }
  else
  {
    resultCode = sqlite3_open(fileName, &db);
    if(resultCode)
    {
      sqlite3_close(db);
      if(errorMsg)
      {
        cout << errorMsg << endl;
        sqlite3_free(errorMsg);
      }
      retVal = false;
    }
  }
  return retVal;
}

bool SQLiteDBManager::InsertIntoTable(const char* sqlCommand)
{
  bool retVal = true;
  
  resultCode = ExecSQLCommand(sqlCommand);
  if(resultCode)
  {
    retVal = CloseDBPrintFreeError();
  }

  return retVal;
}

bool SQLiteDBManager::DeleteFromTable(const char* sqlCommand)
{
  bool retVal = true;
  
  resultCode = ExecSQLCommand(sqlCommand);
  if(resultCode)
  {
    retVal = CloseDBPrintFreeError();
  }
  return retVal;
}

char** SQLiteDBManager::QueryTable(const char* sqlCommand, int & rows, int & cols)
{
  char** retVal = null;
  resultCode = sqlite3_get_table(db, sqlCommand, &retVal, &rows, &cols, &errorMsg);
  return retVal;
}
