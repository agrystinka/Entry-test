#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
      cout << "Please enter:\n./extract <name.img> <name_dir>" << endl;
      return -1;
    }

    if (argc > 3) {
      cout << "Please enter only:\n./extract <name.img> <name_dir>\n<name_dir> is optional" << endl;
      return -1;
    }

    //Read .img file first time to get amout of files in it
    //their path and names
    ifstream instrm(argv[1]);

    if (!instrm.is_open()) {
      perror("Cannot open .img file");
      return -1;
    }

    //Read header
    int16_t magic = 0, entnum = 0;
    instrm.read((char *)&magic, sizeof(int16_t));
    instrm.read((char *)&entnum, sizeof(int16_t));

    //Check if argv[1] is .img format file
    if (magic != -32768){
      cout << "Incorrect file format" << endl;
      return -1;
    }

    //Read fentry table
    int32_t* size = new int32_t [entnum];
    int32_t* offset = new int32_t [entnum];

    for (int i = 0; i < entnum; ++i) {
      size[i] = 0;
      instrm.read((char *)(size + i), 3);
      instrm.read((char *)(offset + i), sizeof(int32_t));
    }

    //Get file path;
    char tmp = '/';
    vector<string> files (entnum);
    if (argc == 3)                         //if user entered dir name
      for (int i = 0; i < entnum; ++i) {   //path name of each file strts with
        files[i] = argv[2];                //the name of entered director
        files[i].push_back(tmp);
      }

    for (int i = 0; i < entnum; ++i) {
      //move over files
      for (int j = offset[i] ; j < size[i] + offset[i]; ++j) {
        instrm.read(&tmp, 1);
      }
      //read file names
      do {
        instrm >>tmp;
        files[i].push_back(tmp);
      } while (tmp !=0);
      }
      instrm.close();
//-----------------------------------------------
      //Create subdirs
      for (int i = 0; i < entnum; ++i) {
        string tmppath;
        int j = 0;
        while(files[i][j]){
          if (files[i][j] =='/')
            if(mkdir(tmppath.c_str(), 0777) && errno != EEXIST){
              perror("Promlem with creating subdir");
              return -1;
            }
          tmppath[j] = files[i][j];
          tmppath[j + 1] = 0;
          j++;
        }
      }
//-----------------------------------------------
      //Read .img file second time to extract files
      instrm.open(argv[1]);
      ofstream outstrm;
      int counter = 0;
      for (int i = 0; i < entnum; ++i) {
        outstrm.open(files[i]);
        //move over info which is not the file
        while (counter < offset[i]) {
          instrm.read(&tmp, 1);
          counter++;
        }

        while (counter <  size[i] + offset[i]) {
          instrm.read(&tmp, 1);
          outstrm << tmp;
          counter++;
        }
        outstrm.close();
      }
      instrm.close();

    return 0;
}
