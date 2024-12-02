#include "io.h"

bool folderExists(const char *path)
{
  struct stat info;

  if (stat(path, &info) != 0)
    return false;
  else if (info.st_mode & S_IFDIR)
    return true;
  else
    return false;
}

void createFolderIfNotExists(const string path)
{
  if (!folderExists(path.c_str()))
  {
    cout << "Creating output folder: " << path << endl;
    mkdir(path.c_str(), 0777);
  }
}

string cleanFolderPath(string path)
{
  if (path.back() != '/')
  {
    path += '/';
  }
  return path;
}

string prepOutputFolder(string outputFolder, string experimentName)
{
  // Get the current directory.
  char *buffer = getcwd(NULL, 0);
  string dir = buffer;
  free(buffer);
  dir = dir + "/";                    // Current directory path
  string dirOut = dir + outputFolder; // Outpus folder path

  createFolderIfNotExists(dirOut);
  string subOutputFolder = cleanFolderPath(dir + outputFolder + experimentName);
  createFolderIfNotExists(subOutputFolder);
  return subOutputFolder;
}