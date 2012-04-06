#ifndef SQLITE_H
#define SQLITE_H

#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

#define FetchType map<int,map<string,string> >

class DB
{
public:
    DB();
    DB(string dbname);
    virtual ~DB();

    int open(string dbname);
    int query(string sql);

    map<string,string> fetch_row(string table,string where = "");
    string fetch_one(string table,string where = "");
    vector<map<string,string> > fetch_all(string table,string where = "");

    string errmsg();
    int errno();
private:
    void check_err();
    int rc;
    sqlite3* db;
    char* err_msg;
};

#endif
