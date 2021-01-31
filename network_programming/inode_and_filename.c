// --------------------------------------------------------------------------------
// Author: Debojyoti Roy
// Email : 2019ht13192@wilp.bits-pilani.ac.in
//
// This program will simulate the linux command - 
//  - List for every file in a directory, its i-node number and file name
//
// Reference - 
//  1. http://manpages.ubuntu.com/manpages/trusty/man3/readdir.3.html
// --------------------------------------------------------------------------------


#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <error.h>
#include <sys/stat.h>

int main (int argc, char *argv[ ])
{
    DIR *p_dir;
    struct dirent *p_dirent;
    struct stat st;
    char buf[1024];

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
        printf("usage: inode_and_filename  <directory>\n");
        exit(EXIT_FAILURE);
    }

    if (p_dir == NULL)
    {
        printf("Path doesn't exist!");
        exit(EXIT_FAILURE);
    }
    

    p_dirent = readdir(p_dir);

    printf ("\n+--------------------------------------------+");
    printf ("\n i-node\t\tfilename");
    printf ("\n+--------------------------------------------+\n");

    while(p_dirent != NULL)
    {
        if (p_dirent->d_name[0] != '.')
        {
            sprintf(buf, "%s/%s", argv[1], p_dirent->d_name);
            stat(buf, &st);

            printf("%li\t\t%s", st.st_ino, p_dirent->d_name);
            printf("\n");
        }

        p_dirent = readdir(p_dir);
    }

    printf("\n");

    closedir(p_dir);
    return 0;
}

