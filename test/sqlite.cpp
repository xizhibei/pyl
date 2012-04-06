#include "sqlite.h"

DB::DB()
{
    db = NULL;
    err_msg = NULL;
}
DB::DB(string dbname)
{
    DB();
    this->open(dbname);
}
DB::~DB()
{
    sqlite3_close(db);
}

string DB::errmsg()
{
    return string(err_msg);
}

int DB::errno()
{
    return rc;
}

void DB::check_err()
{
    if(rc != SQLITE_OK)
    {
        printf(err_msg);
        exit(0);
    }
}

int DB::open(string dbname)
{
    rc = sqlite3_open(dbname.c_str(),&db);
    if(rc != SQLITE_OK)
    {
        err_msg = "Error while open database!";
    }
    return rc;
}

int DB::query(string sql)
{
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);
    return rc;
}

vector<map<string,string> > DB::fetch_all(string table, string where)
{
    int nrow,ncol;
    string sql = "select * from " + table + " " + where;
    char** result;
    rc = sqlite3_get_table(db, sql.c_str(),&result, &nrow, &ncol, &err_msg);

    this->check_err();

    vector<map<string,string> > all;
    for(int i = 1;i <= nrow;i++)
    {
        map<string,string> tmp;
        for(int j = 0;j < ncol;j++)
        {
            tmp.insert(pair<string,string>(result[j],result[i * ncol + j]));
        }
        all.push_back(tmp);
    }

    sqlite3_free_table(result);

    return all;
}

map<string,string> DB::fetch_row(string table, string where)
{
    int nrow,ncol;
    string sql = "select * from " + table + " " + where;
    char** result;
    rc = sqlite3_get_table(db, sql.c_str(),&result, &nrow, &ncol, &err_msg);

    this->check_err();

    map<string,string> tmp;
    for(int i = 0;i < ncol;i++)
    {
       tmp.insert(pair<string,string>(result[i],result[ncol + i]));
    }

    sqlite3_free_table(result);

    return tmp;
}

string DB::fetch_one(string table, string where)
{
    int nrow,ncol;
    string sql = "select * from " + table + " " + where;
    char** result;
    rc = sqlite3_get_table(db, sql.c_str(),&result, &nrow, &ncol, &err_msg);

    this->check_err();

    string tmp = string(result[1]);

    sqlite3_free_table(result);

    return tmp;
}
