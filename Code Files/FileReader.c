#include "FileReader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_ProjectsInfo(struct ProjectsInfo* projectsInfo)
{
    projectsInfo->size = 0;
    projectsInfo->Projects = calloc(0, sizeof(struct Project));
}

void add_ProjectInfo(struct ProjectsInfo* projectsInfo, const struct Project* project)
{
    if (projectsInfo == NULL || project == NULL)
    {
         printf("Project is nullptr\n");
        return;
    }
    
    projectsInfo->Projects = (struct Project*)realloc(&projectsInfo->Projects[0], (projectsInfo->size+1)*sizeof(struct Project));

    memcpy(&projectsInfo->Projects[projectsInfo->size], project, sizeof(struct Project));

    projectsInfo->size++;
}

void remove_NextLines(char string[])
{
    for (size_t i = 0; i < strlen(string); i++)
    {
        if (string[i] == '\n')
        {
            string[i] = '\0';
        }
    }
}

void print_ProjectData(struct Project* project)
{
    printf("Project Name: %s Project Address: %s\n", project->ProjectName, project->ProjectPath);
}