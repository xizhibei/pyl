#include <stdio.h>
#include <sqlite3.h>

int main()
{
    sqlite3 *db = NULL;
    char* err_msg = 0;
    int rc = sqlite3_open("data.db",&db);

    if(rc)
    {
        printf("open database error\n");
        sqlite3_close(db);
        return 0;
    }
    char* sql = "select * from software";
    int nrow,ncol;
    char** result;
    sqlite3_get_table(db,sql,&result,&nrow,&ncol,&err_msg);
    if(err_msg != NULL)
        printf("%s\n",err_msg);
    printf("We get %d rows and %d col\n",nrow,ncol);

    for(int i = 0;i < nrow;i++)
    {
        for(int j = 0;j < ncol;j++)
        {
            printf("%s",result[i * nrow + j]);
        }
    }

    sqlite3_close(db);
    return 0;
}
