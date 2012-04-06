#include <stdio.h>
#include <sqlite3.h>

int main()
{
    sqlite3 *db = NULL;
    char* err_msg = 0;
    int rc = sqlite3_open("user.db",&db);

    if(rc != SQLITE_OK)
    {
        printf("open database error: %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    /*rc = sqlite3_exec(db, "CREATE TABLE software(sid integer, name text,start integer,end integer)", 0, 0, &err_msg);
    if(rc != SQLITE_OK)
        printf("%s\n",err_msg);
    
    rc = sqlite3_exec(db, "delete from software", 0, 0, &err_msg);
    if(rc != SQLITE_OK)
        printf("%s\n",err_msg);


    rc = sqlite3_exec(db, "insert into software values(1,'chrome',1,10)", 0, 0, &err_msg);
    if(rc != SQLITE_OK)
        printf("%s\n",err_msg);
    
    rc = sqlite3_exec(db, "insert into software values(2,'firefox',10,100)", 0, 0, &err_msg);
    if(rc != SQLITE_OK)
        printf("%s\n",err_msg);*/
    


    char* sql = "select * from software";
    int nrow,ncol;
    char** result;
    rc = sqlite3_get_table(db,sql,&result,&nrow,&ncol,&err_msg);
    if(rc != SQLITE_OK)
        printf("%s\n",err_msg);
    printf("We get %d rows and %d col\n",nrow,ncol);

    for(int i = 0;i <= nrow;i++)
    {
        for(int j = 0;j < ncol;j++)
        {
            printf("%s ",result[i * ncol + j]);
        }
        puts("");
    }
    sqlite3_free_table(result);

    sqlite3_close(db);
    return 0;
}
