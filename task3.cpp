#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <algorithm>

using namespace std;

struct fileinfo {
    char type;
    string permissions;
    int links;
    string owner;
    string group;
    int size;
    string month;
    string date;
    string name;
    time_t time;  //for sort by the time of last data modification before print
};

char getFileType(int);
void getPermissions(char*, int);
string getMonthName(char*);

int main(int argc, char const *argv[]) {
    if(argc < 2) {
      cout << "Use key, please. Avaliable keys are: -a, -l, -la\n";
      return 0;
    }

    string key = argv[1];
    if (key != "-a" && key != "-l" && key != "-la") {
        cout << "Wrong key. Avaliable keys are: -a, -l, -la\n";
        return 0;
    }

    vector<fileinfo> files;

    struct passwd *pwd;
    struct group  *gr;
    FILE *fp;
    struct stat sb;
    struct dirent *dp;
    DIR *dir = opendir(".");   //work with current direcctory

    int type;
    fileinfo currfile;

    while ((dp = readdir(dir)) != NULL) {
        //9th colymn: name
        currfile.name = dp->d_name;

        //1st column: type
        type = dp->d_type;
        currfile.type = getFileType(type);

        pwd = getpwuid(geteuid());       //4th column: owner name
        currfile.owner = pwd->pw_name;
        gr = getgrgid(getegid());        //5th column: group name
        currfile.group = gr->gr_name;

        fp = fopen(currfile.name.c_str(), "r");     //open filefor reading
        if (fp == NULL)
            perror("Problem with file");            //file wasn`t open
        else{
            fstat(fileno(fp), &sb);

            //2nd column: permissions
            char permbuf[10];
            getPermissions(&permbuf[0], sb.st_mode);
            currfile.permissions = permbuf;

            //3rd column: links
            currfile.links = sb.st_nlink;

            //6th column: size
            currfile.size = sb.st_size;

            //7th and 8th columns: date time
            currfile.time = sb.st_mtime;   //get file of last data modification
            struct tm *tm;
            char timebuf[200];
            char monthbuf[5];
            tm = localtime(&currfile.time);  //convert time representation
            strftime(timebuf, sizeof(timebuf), "%d  %H:%M", tm);   //format: days hour:minute
            currfile.date = timebuf;
            strftime(monthbuf, sizeof(monthbuf), "%m", tm);   //get month`s number
            currfile.month = getMonthName(&monthbuf[0]);      //get month`s name

            fclose(fp);
        }
        files.push_back(currfile);
    }
    closedir(dir);

    //sort output by the time of of last data modification
    sort(files.begin(), files.end(),
      [](const fileinfo &l, const fileinfo &r) { return ( l.time > r.time ); } ); //new - first

    //Final output
    if (key == "-a")
      for(fileinfo curr : files)
        cout << curr.name << endl;

    else
      for (fileinfo curr : files)
        if ((key == "-l" && curr.links == 1) || key == "-la") //if key == "-l", don`t print files "." and ".."
          cout << curr.type << curr.permissions << ' ' << curr.links << ' ' << curr.owner << ' ' << curr.group
          << ' ' << curr.size << ' ' << curr.month << ' ' << curr.date << ' ' << curr.name << endl;

    files.clear();
    return 0;
}

char getFileType(int type) {
    switch (type) {
      case DT_REG:                 //regular file
        return '-';
      case DT_DIR:                 //directory
        return 'd';
      case DT_FIFO:                //named pipe
        return 'p';
      case DT_SOCK:                //local socket file
        return 's';
      case DT_CHR:                 //character device file
        return 'c';
      case DT_BLK:                 //block device file
        return 'b';
      case DT_LNK:                 //symbolic link
        return 'l';
    }
    return ' ';
}

void getPermissions(char* p, int permission) {
    *(p) = (permission & S_IRUSR) ? 'r' : '-';
    *(p + 1) = (permission & S_IWUSR) ? 'w' : '-';
    *(p + 2) = (permission & S_IXUSR) ? 'x' : '-';
    *(p + 3) = (permission & S_IRGRP) ? 'r' : '-';
    *(p + 4) = (permission & S_IWGRP) ? 'w' : '-';
    *(p + 5) = (permission & S_IXGRP) ? 'x' : '-';
    *(p + 6) = (permission & S_IROTH) ? 'r' : '-';
    *(p + 7) = (permission & S_IWOTH) ? 'w' : '-';
    *(p + 8) = (permission & S_IXOTH) ? 'x' : '-';
    *(p + 9) = 0;
}

string  getMonthName (char* buff) {
    vector <string> monthnames = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
    long int monthnum = atol(buff);
    return monthnames[monthnum - 1];
}
