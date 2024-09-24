#ifndef FileReader
#define FileReader

struct Project
{
    char ProjectName[32];
    char ProjectPath[260];
};

struct ProjectsInfo
{
    int size;
    struct Project* Projects;
};

void init_ProjectsInfo(struct ProjectsInfo* projectsInfo);
void add_ProjectInfo(struct ProjectsInfo* projectsInfo, const struct Project* project);
void remove_NextLines(char string[]);
void print_ProjectData(struct Project* project);
void print_ProjectDataList(struct ProjectsInfo* projectsInfo);

#endif