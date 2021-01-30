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
    struct stat st;
    unsigned int size;
    struct tm dt;
    struct group *gr;
    struct passwd *pwd;
    unsigned long int total_size = 0;
    FILE *fp;

    if (argc != 2)
    {
        //printf("This is a simulation of \"ls -l\" command.\n");
        printf("usage: lsminusl_command <directory>\n");
        exit(EXIT_FAILURE);
    }

    //p_dir = opendir(".");
    p_dir = opendir(argv[1]);
    if (p_dir == NULL)
    {
        printf("Path doesn't exist!");
        exit(EXIT_FAILURE);
    }
    

    p_dirent = readdir(p_dir);

    fp = fopen(p_dirent->d_name, "r");
    fseek(fp, 0, 2);
    size = ftell(fp);
    fclose(fp);

    printf ("total %u\n", size);
    while(p_dirent != NULL)
    {
        // ls -l command doesn't show hidden files.
        // Hence, ignoring those files starting with '.'
        // character.
        if (p_dirent->d_name[0] != '.')
        {
            stat(p_dirent->d_name, &st);

            file_permission(st.st_mode);    // File Permission

            size = st.st_size;
            printf("%lu\t",st.st_nlink);    // File Hard Link

            pwd = getpwuid(st.st_uid);
            printf("%s\t", pwd->pw_name);

            gr = getgrgid(st.st_gid);
            printf("%s\t", gr->gr_name);

            printf("%u\t", size);

            dt = *(gmtime(&st.st_mtime));
            printf ("%s %2d ",
                    MONTH[(dt.tm_mon)%12],
                    dt.tm_mday);
            if (dt.tm_year + 1900 == CURRENT_YEAR)
                printf ("%d:%d", dt.tm_hour, dt.tm_min);
            else
                printf ("%5d", dt.tm_year + 1900);
            printf ("\t");

            printf("%s\t", p_dirent->d_name);
            printf("\n");
        }

        p_dirent = readdir(p_dir);
        total_size += size;
    }

    printf ("total %lu", total_size/1024);
    printf("\n");

    closedir(p_dir);
    return 0;
}

