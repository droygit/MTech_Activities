// --------------------------------------------------------------------------------
// Author: Debojyoti Roy
// Email : 2019ht13192@wilp.bits-pilani.ac.in
//
// This program will simulate the linux command - 
// * Unix ls-l command
// As "ls -l" command always shows the current directory,
// this program will also concern with the current directory only.
//
// Reference - 
//  1. http://manpages.ubuntu.com/manpages/trusty/man3/readdir.3.html
// --------------------------------------------------------------------------------


#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <error.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

#define CURRENT_YEAR 2021
#define SKIP    continue
//#define SKIP    printf (">> DEBUG << \t\t\t")
//#define DEBUG_PRINT printf("\n%d", __LINE__)
#define DEBUG_PRINT  


char MONTH[12][4] = { 
    "Jan\0",
    "Feb\0",
    "Mar\0",
    "Apr\0",
    "May\0",
    "Jun\0",
    "Jul\0",
    "Aug\0",
    "Sep\0",
    "Oct\0",
    "Nov\0",
    "Dec\0"
};
    

void file_permission(mode_t st_mode)
{
    printf( (S_ISDIR(st_mode)) ? "d" : "-");
    printf( (st_mode & S_IRUSR) ? "r" : "-");
    printf( (st_mode & S_IWUSR) ? "w" : "-");
    printf( (st_mode & S_IXUSR) ? "x" : "-");
    printf( (st_mode & S_IRGRP) ? "r" : "-");
    printf( (st_mode & S_IWGRP) ? "w" : "-");
    printf( (st_mode & S_IXGRP) ? "x" : "-");
    printf( (st_mode & S_IROTH) ? "r" : "-");
    printf( (st_mode & S_IWOTH) ? "w" : "-");
    printf( (st_mode & S_IXOTH) ? "x" : "-");
    printf( "\t" );
}

int main (int argc, char *argv[ ])
{
    DIR *p_dir;
    struct dirent *p_dirent;
    char buf [1024];
    struct stat st;
    unsigned int size;
    struct tm dt;
    struct group *gr;
    struct passwd *pwd;
    unsigned long int total_size = 0;
    
    DEBUG_PRINT;

    if (argc == 1)
    {
        p_dir = opendir(".");
    }
    else if (argc == 2)
    {
        p_dir = opendir(argv[1]);
    }
    else
    {
        printf("usage: lsminusl_command <directory>\n");
        exit(EXIT_FAILURE);
    }
    DEBUG_PRINT;

    if (p_dir == NULL)
    {
        printf("Path doesn't exist!");
        exit(EXIT_FAILURE);
    }
    
    DEBUG_PRINT;

    p_dirent = readdir(p_dir);

    if (p_dirent == NULL)
    {
        printf("Path doesn't exist!");
        exit(EXIT_FAILURE);
    }

    while(p_dirent != NULL)
    {
        DEBUG_PRINT;
        // ls -l command doesn't show hidden files.
        // Hence, ignoring those files starting with '.'
        // character.
        if (p_dirent->d_name[0] != '.')
        {
            DEBUG_PRINT;
            sprintf(buf, "%s/%s", argv[1], p_dirent->d_name);
            stat(buf, &st);

            file_permission(st.st_mode);    // File Permission

            size = st.st_size;
            printf("%lu\t",st.st_nlink);    // File Hard Link

            pwd = getpwuid(st.st_uid);
            printf("%s\t", pwd->pw_name);

            gr = getgrgid(st.st_gid);
            printf("%s\t", gr->gr_name);

            printf("%u\t", size);

            dt = *(gmtime(&st.st_mtime));
            printf ("%s %02d ",
                    MONTH[(dt.tm_mon)%12],
                    dt.tm_mday);
            if (dt.tm_year + 1900 == CURRENT_YEAR)
                printf ("%02d:%02d", dt.tm_hour, dt.tm_min);
            else
                printf ("%5d", dt.tm_year + 1900);
            printf ("\t");

            printf("%s\t", p_dirent->d_name);
            printf("\n");
        }

        p_dirent = readdir(p_dir);
    }

    printf("\n");

    closedir(p_dir);
    return 0;
}

