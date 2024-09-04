#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <windows.h>
#include <shellapi.h>

#include "FlexFile.h"
#include "FileReader.h"

#define GCC_PATH "C:\\MinGW\\bin\\gcc.exe"

struct Settings
{
    char CompilerPath[MAX_PATH];
};

void compile_File(const char* projectPath, const char* filePath)
{
    char buildParameter[MAX_PATH];
    char buildFolder[MAX_PATH];

    strcpy(buildParameter, filePath);
    strcat(buildParameter, " -c ");

    strcpy(buildFolder, projectPath);
    strcat(buildFolder, "/Program Files");

    //printf("%s\n", buildParameter);
    HINSTANCE error = ShellExecute(NULL, "open", GCC_PATH, buildParameter, buildFolder, 1);
}

void compile_Project(const char* projectPath)
{
    char buildFolder[MAX_PATH];
    strcpy(buildFolder, projectPath);
    strcat(buildFolder, "/Program Files");
    struct FileStruct dirFiles = open_Directory(buildFolder, "o");

    char buildParameter[BUFSIZ];
    
    for (size_t i = 0; i < dirFiles.NumberOfFiles; i++)
    {
        strcat(buildParameter, dirFiles.FileList[i].Name);
        strcat(buildParameter, " ");
    }

    strcat(buildParameter, " -o ../Build/Project.exe");

    //printf("%s\n", buildParameter);
    HINSTANCE error = ShellExecute(NULL, "open", GCC_PATH, buildParameter, buildFolder, 1);
}

void build_Project(struct FileStruct* dirFiles, const char* projectFilePath)
{
    for (int i = 0; i < dirFiles->NumberOfFiles; i++)
    {
        compile_File(projectFilePath, dirFiles->FileList[i].Path);
    }

    Sleep(5);
    compile_Project(projectFilePath);
}

void setup_Project(char* projectName, char* projectPath)
{
    projectName[strlen(projectName)-1] = '\0';
    projectPath[strlen(projectPath)-1] = '\0';

    make_Directory(projectName, projectPath);
    char tempString[strlen(projectPath) + strlen(projectName) + 2];
    strcpy(tempString, projectPath);
    strcat(tempString, "\\");
    strcat(tempString, projectName);
    make_Directory("Build", tempString);
    make_Directory("Program Files", tempString);
    make_Directory("Code Files", tempString);
    strcat(tempString, "/code Files/Main.c");
    
    FILE* mainFile = fopen(tempString, "w");
    const char* mainFileText = "int main ()\n{\n}";
    fprintf(mainFile, "%s", mainFileText);

    fclose(mainFile);

    FILE* projectsFile = fopen("Project_List", "a");
    fprintf(projectsFile, "Name: \"%s\"\n", projectName);
    fprintf(projectsFile, "Address: \"%s\"\n", projectPath);

    fclose(projectsFile);

    printf("Project Setup Done \n");
}

void load_Settings(struct Settings* settingsData)
{
    FILE* settingsFile;
    settingsFile = fopen("Settings", "r");

    char fileBuffer[BUFSIZ] = {'\0'};
    if (settingsFile == NULL)
    {
        printf("Could not open Settings\n");
        return;
    }

    while (fgets(fileBuffer, sizeof(fileBuffer), settingsFile))
    {
        printf("Settings:\n");

        int character = '"';
        char* info = strchr(fileBuffer, character);

        if (strncmp(fileBuffer, "Compiler Path GCC:", 18) == 0)
        {
            strcpy(settingsData->CompilerPath, info);

            printf("Compiler Path: %s\n", settingsData->CompilerPath);
        }
    }

    fclose(settingsFile);
}

void load_Projects(struct ProjectsInfo* projectsInfo)
{
    if (projectsInfo == NULL)
    {
        printf("projecsts Info is nullptr\n");
        return;
    }

    FILE* projectsFile;
    projectsFile = fopen("Project_List", "r");

    char fileBuffer[BUFSIZ] = {'\0'};
    if (projectsFile == NULL)
    {
        printf("Could not open Settings\n");
        return;
    }

    struct Project tempProject;
    int projectInfoFilled = FALSE;
    int character = '"';

    printf("Projects:\n");

    while (fgets(fileBuffer, sizeof(fileBuffer), projectsFile))
    {
        char* info = strchr(fileBuffer, character);

        if (strncmp(fileBuffer, "Name:", 5) == 0)
        {   
            strcpy(tempProject.ProjectName, info);
            remove_NextLines(tempProject.ProjectName);
        }
        else if (strncmp(fileBuffer, "Address:", 8) == 0)
        {
            strcpy(tempProject.ProjectPath, info);
            remove_NextLines(tempProject.ProjectPath);
            projectInfoFilled = TRUE;
        }

        if (projectInfoFilled == TRUE)
        {
            add_ProjectInfo(projectsInfo, &tempProject);
            projectInfoFilled = FALSE;
        }
    }

    fclose(projectsFile);
}

int main(int argc, char* argv[])
{
    printf("starting up \n");

    struct Settings settingsData;
    load_Settings(&settingsData);

    struct ProjectsInfo Projects;
    init_ProjectsInfo(&Projects);
    load_Projects(&Projects);

    for (size_t i = 0; i < Projects.size; i++)
    {
        print_ProjectData(&Projects.Projects[i]);
    }

    printf("Create: Creates new Project\n");
    printf("Compile: Compiles existing Project\n");

    char UserInput[10];
    fgets(UserInput, sizeof(UserInput), stdin);
    UserInput[strlen(UserInput)-1] = '\0';

    if (strcmp(UserInput, "Create") == 0)
    {
        char ProjectPath[MAX_PATH];
        char ProjectName[16];

        printf("Project Path:");
        fgets(ProjectPath, sizeof(ProjectPath), stdin);
        printf("Project Name:");
        fgets(ProjectName, sizeof(ProjectName), stdin);

        setup_Project(ProjectName, ProjectPath);
    }
    else if (strcmp(UserInput, "Compile") == 0)
    {
        char ProjectPath[MAX_PATH];
        char ProjectCodeFilePath[MAX_PATH];

        printf("Project Path:");
        fgets(ProjectPath, sizeof(ProjectPath), stdin);

        ProjectPath[strlen(ProjectPath)-1] = '\0';
        strcpy(ProjectCodeFilePath, ProjectPath);
        strcat(ProjectCodeFilePath, "/Code Files");
        struct FileStruct dirFiles = open_Directory(ProjectCodeFilePath, "c");
        printf("number of files in directory: %d\n", dirFiles.NumberOfFiles);
        printf("number of Directories in directory: %d\n", dirFiles.NumberOfDirectories);
        print_DirectoryPaths(&dirFiles);    
        print_FilePaths(&dirFiles);

        build_Project(&dirFiles, ProjectPath);
    }

    return 1;
}