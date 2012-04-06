#include "sqlite.h"
#include <iostream>
using namespace std;

int main()
{
    DB db;
    db.open("user.db");
    vector<map<string,string> > result = db.fetch_all("software");
    map <string,string> ::iterator p; 
    for(int i = 0;i < result.size();i++)
    {
        for(p = result[i].begin();p != result[i].end();p++)
            cout << p->first << " " << p->second << endl;
    }
}
