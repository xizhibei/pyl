#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

using std::vector;
using std::string;

vector<string> filenames;

void getDirInfo(const char *pathname)
{
    DIR* dir;
    struct dirent * ent;
    int n = 0;
    dir = opendir(pathname);
    if (!dir) 
        return;
    while (ent = readdir(dir))
    {
        char currfile[200];
        if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name,"..") == 0)
            continue;
        sprintf(currfile,"%s/%s",pathname,ent->d_name);
        if(ent->d_type == 4)
            getDirInfo(currfile);
        else
        {
            //puts(currfile);
            filenames.push_back(string(currfile)); 
        }
    }  
    closedir(dir);  
}

void print_dir(const char* pathname)
{
    getDirInfo(pathname);
    for(int i = 0;i < filenames.size();i++)
        puts(filenames[i].c_str());
}

int main(int argc,char** argv)
{
    print_dir(argv[1]);
}
