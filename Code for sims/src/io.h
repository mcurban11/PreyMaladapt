#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
// #include <filesystem>

using namespace std;

bool folderExists(const char *path);

void createFolderIfNotExists(const string path);

string cleanFolderPath(string path);

string getOutputFilePath(string dir, string name);

string prepOutputFolder(string outputFolder, string experimentName);