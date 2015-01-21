#include <iostream>
#include "SQLiteDBManager.h"

using namespace std;

int main(int argc, char* argv[])
{
  SQLiteDBManager dbManager;
  int i = 0;

  //The only argument is the database to connect to, 
  //if there are no arguments we will create a new one.
  if(argc > 1)
  {
    if(dbManager.ConnectToDataBase(argv[1]))
    {
      cout << "Connected to " << argv[1] << " successfully." << endl;
    }
  }
  else
  {
    char* tables[2] = {"CREATE TABLE DefaultTable1(Time TIMESTAMP, ID VARCHAR(128), Name VARCHAR(128));", 
      "CREATE TABLE DefaultTable2(Time TIMESTAMP, ID VARCHAR(128), Name VARCHAR(128));"};
    if(dbManager.CreateNewDataBase("default.db", 2, tables))
    {
      cout << "Successfully created default.db." << endl;
    }
    char * dt1rows[2] = {"INSERT INTO DefaultTable1 (TimeStamp, PolicyID, Approved) VALUES (DATETIME ( 'NOW' ), 01, Row1);", 
      "INSERT INTO DefaultTable1 (TimeStamp, PolicyID, Approved) VALUES (DATETIME ( 'NOW' ), 02, Row2);"};
    char * dt2rows[2] = {"INSERT INTO DefaultTable2 (TimeStamp, PolicyID, Approved) VALUES (DATETIME ( 'NOW' ), 01, Row11);", 
      "INSERT INTO DefaultTable2 (TimeStamp, PolicyID, Approved) VALUES (DATETIME ( 'NOW' ), 02, Row22);"};

    if(dbManager.InsertIntoTable(dt1rows[0]) && dbManager.InsertIntoTable(dt1rows[1]))
    {
      cout << "Successfully added 2 rows to table1." << endl;
    }
    if(dbManager.InsertIntoTable(dt2rows[0]) && dbManager.InsertIntoTable(dt2rows[1]))
    {
      cout << "Successfully added 2 rows to table2." << endl;
    }
    char ** table1 = 0;
    int r = 0, 
        c = 0;

    table1 = dbManager.QueryTable("SELECT * FROM DefaultTable1", r, c);
    if(r > 0)
    {
      for(i = 0; i < r*c; i++)
      {
        cout << table1[i] << "\t";
        if((i+1) % c == 0)
        {
          cout << endl;
        }
      }
    }

    if(dbManager.DeleteFromTable("DELETE * FROM DefaultTable2 WHERE PolicyID is 02"))
    {
      cout << "Successfully deleted a row from table2." << endl;
    }

  }
  return 0;
}
