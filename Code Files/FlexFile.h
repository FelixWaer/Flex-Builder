#ifndef FlexFile
#define FlexFile

struct File
{
    int Size;
    char Path[MAX_PATH];
    char Name[24];
};

struct Directory
{
    char Path[MAX_PATH];
    char Name[24];
};

struct FileStruct
{
    int NumberOfFiles;
    int NumberOfDirectories;
    struct File* FileList;
    struct Directory* DirectoryList;
};

struct FileStruct open_Directory(char* directoryPath, const char* fileType);

void print_FilePaths(struct FileStruct* dirFiles);
void print_FileNames(struct FileStruct* dirFiles);
void print_DirectoryPaths(struct FileStruct* dirFiles);
void print_DirectoryNames(struct FileStruct* dirFiles);

void make_Directory(const char* newDirName, const char* newDirAddress);

#endif