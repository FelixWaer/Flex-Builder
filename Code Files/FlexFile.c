#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <dirent.h>

#include "FlexFile.h"

void init_DirectoryFiles(struct FileStruct* dirFiles)
{
    dirFiles->NumberOfFiles = 0;
    dirFiles->NumberOfDirectories = 0;
}

void add_FileToFileStruct(struct FileStruct* dirFiles, struct File* fileData)
{
    if (dirFiles->NumberOfFiles <= 0)
    {
        struct File* newFilePtr = (struct File*)malloc(sizeof(struct File));

        if (newFilePtr == NULL)
        {
            printf("Could not allocate memory for DirectoryFiles\n");
            return;
        }
        
        dirFiles->FileList = newFilePtr;
        memcpy(&dirFiles->FileList[dirFiles->NumberOfFiles], fileData, sizeof(struct File));
    }
    else
    {
        struct File* newFilePtr = (struct File*)realloc(dirFiles->FileList, sizeof(struct File)*(dirFiles->NumberOfFiles+1));

        if (newFilePtr == NULL)
        {
            printf("Could not allocate memory for DirectoryFiles\n");
            return;
        }

        dirFiles->FileList = newFilePtr;
        memcpy(&dirFiles->FileList[dirFiles->NumberOfFiles], fileData, sizeof(struct File));
    }

    dirFiles->NumberOfFiles++;
}

void add_DirectoryToFileStruct(struct FileStruct* dirFiles, struct Directory* directoryData)
{
    if (dirFiles->NumberOfDirectories <= 0)
    {
        struct Directory* newDirectoryPtr = (struct Directory*)malloc(sizeof(struct Directory));

        if (newDirectoryPtr == NULL)
        {
            printf("Could not allocate memory for Directory\n");
            return;
        }
        
        dirFiles->DirectoryList = newDirectoryPtr;
        memcpy(&dirFiles->DirectoryList[dirFiles->NumberOfDirectories], directoryData, sizeof(struct Directory));
    }
    else
    {
        struct Directory* newDirectoryPtr = (struct Directory*)realloc(dirFiles->DirectoryList, sizeof(struct Directory)*(dirFiles->NumberOfDirectories+1));

        if (newDirectoryPtr == NULL)
        {
            printf("Could not allocate memory for Directory\n");
            return;
        }

        dirFiles->DirectoryList = newDirectoryPtr;
        memcpy(&dirFiles->DirectoryList[dirFiles->NumberOfDirectories], directoryData, sizeof(struct Directory));
    }

    dirFiles->NumberOfDirectories++;
}

struct FileStruct open_Directory(char* directoryPath, const char* fileType)
{
    struct FileStruct dirFiles;
    init_DirectoryFiles(&dirFiles);

    char dirAddress[strlen(directoryPath) + 3];
    strcpy(dirAddress, directoryPath);
    strcat(dirAddress, "/*");      
    if (strcmp(fileType, "All") != 0)
    {
      strcat(dirAddress, ".");
      strcat(dirAddress, fileType);
    }


    //printf("Directory Address: %s\n", dirAddress);
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(dirAddress, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("FindFirstFile failed %d\n", (int)GetLastError());
        return dirFiles;
    }

    do
    {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            struct Directory dir;

            char dirPath[MAX_PATH];

            strcpy(dirPath, directoryPath);
            strcat(dirPath,"/");
            strcat(dirPath, FindFileData.cFileName);
            strcpy(dir.Path, dirPath);
            strcpy(dir.Name, FindFileData.cFileName);

            add_DirectoryToFileStruct(&dirFiles, &dir);
        }
        else
        {
            int fileSize = FindFileData.nFileSizeLow;
            //printf(TEXT("File: %s File size: %d \n"), FindFileData.cFileName, fileSize);

            struct File file;
            file.Size = fileSize;

            char filePath[MAX_PATH];

            strcpy(filePath, "\"");
            strcat(filePath, directoryPath);
            strcat(filePath,"/");
            strcat(filePath, FindFileData.cFileName);
            strcat(filePath, "\"");
            strcpy(file.Path, filePath);
            strcpy(file.Name, FindFileData.cFileName);

            add_FileToFileStruct(&dirFiles, &file);
        }
        
    } while (FindNextFile(hFind, &FindFileData) != 0);
    
    FindClose(hFind);

    return dirFiles;
}

void print_FilePaths(struct FileStruct* dirFiles)
{
    for (int i = 0; i < dirFiles->NumberOfFiles; i++)
    {
        printf("File: %s\n", dirFiles->FileList[i].Path);
    }
}

void print_FileNames(struct FileStruct* dirFiles)
{
    for (int i = 0; i < dirFiles->NumberOfFiles; i++)
    {
        printf("File: %s\n", dirFiles->FileList[i].Name);
    }
}

void print_DirectoryPaths(struct FileStruct* dirFiles)
{
    for (int i = 0; i < dirFiles->NumberOfDirectories; i++)
    {
        printf("Dir: %s\n", dirFiles->DirectoryList[i].Path);
    }
}

void print_DirectoryNames(struct FileStruct* dirFiles)
{
    for (int i = 0; i < dirFiles->NumberOfDirectories; i++)
    {
        printf("Dir: %s\n", dirFiles->DirectoryList[i].Name);
    }
}

void make_Directory(const char* newDirName, const char* newDirAddress)
{
    char dirAddress[strlen(newDirAddress) + 3];
    strcpy(dirAddress, newDirAddress);
    strcat(dirAddress, "/*");

    char test[strlen(newDirAddress) + strlen(newDirName) + 2];
    strcpy(test, newDirAddress);
    strcat(test, "/");
    strcat(test, newDirName);

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(dirAddress, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("FindFirstFile failed %d\n", (int)GetLastError());
        return;
    }

    while (FindNextFile(hFind, &FindFileData) != 0)
    {
        if (strcmp(FindFileData.cFileName, newDirName) == 0)
        {
            printf("Directory already exists\n");
            return ;
        }
    }
    FindClose(hFind);

    mkdir(test);
    printf("%s \n", "Created Directory");
}