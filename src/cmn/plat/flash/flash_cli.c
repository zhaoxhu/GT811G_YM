#include "iros_config.h"
#include <pkgconf/hal.h>
#include <pkgconf/kernel.h>
#include <pkgconf/io_fileio.h>

#include <cyg/kernel/ktypes.h>         // base kernel types
#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros
#include <cyg/io/flash.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

#include <cyg/fileio/fileio.h>

#include <cyg/infra/testcase.h>
#include <cyg/infra/diag.h>            // HAL polled output

#include <pkgconf/fs_jffs2.h>         // Address of JFFS2


extern bool flash_cli_mode;
extern char cli_prompt[10];
extern int tolower( int c );
extern int jffs2_1_main(void);
extern int jffs2_1_1_main(void);
extern int jffs2_2_main(void);
extern int jffs2_2_main(void);
extern void imst_test_cyg_start(int);
int jffs2_3_main (int argc, char **argv);

char flash_current_dir[128];

/*==========================================================================*/

#define FS_NONE      0
#define FS_RETURN    1

#define FS_ERR_BREAK( _fn, _res ) \
{ \
    diag_printf("<FAIL>: " #_fn "() returned %ld %s\n", (unsigned long)_res, _res<0?strerror(errno):""); \
    break; \
}

#define FS_ERR_NO_RETURN( _fn, _res ) \
{ \
    diag_printf("<FAIL>: " #_fn "() returned %ld %s\n", (unsigned long)_res, _res<0?strerror(errno):""); \
}

#define FS_ERR_RETURN( _fn, _res ) \
{ \
    diag_printf("<FAIL>: " #_fn "() returned %ld %s\n", (unsigned long)_res, _res<0?strerror(errno):""); \
    return; \
}

#define FS_ERR_FAIL_RETURN( _fn, _res ) \
{ \
    diag_printf("<FAIL>: " #_fn "() returned %ld %s\n", (unsigned long)_res, _res<0?strerror(errno):""); \
    return -1; \
}


#define IOSIZE  1024

#define LONGNAME1       "long_file_name_that_should_take_up_more_than_one_directory_entry_1"
#define LONGNAME2       "long_file_name_that_should_take_up_more_than_one_directory_entry_2"

/*==========================================================================*/


#ifndef CYGPKG_LIBC_STRING
char *strcat( char *s1, const char *s2 )
{
    char *s = s1;
    while( *s1 ) s1++;
    while( (*s1++ = *s2++) != 0);
    return s;
}

#endif


/*
 * Function: listdir
 * Description: To list/print all the files and dir for a given dir
 * Input: name - name of the dir
 * Output: numgot - number of fils/dir printed
 */
static void listdir( char *name, int *numgot )
{
    int err;
    DIR *dirp;
    int num=0;
    time_t tmp_time;

    dirp = opendir( name );
    if( dirp == NULL ) FS_ERR_RETURN( opendir, -1 );

    for(;;)
    {
        struct dirent *entry = readdir( dirp );

        if( entry == NULL )
            break;
        num++;
        char fullname[PATH_MAX];
        struct stat sbuf;

        if( name[0] )
        {
            strncpy(fullname, name, PATH_MAX-2-strlen(entry->d_name));
            if( !(name[0] == '/' && name[1] == 0 ) )
                strcat(fullname, "/" );
        }
        else fullname[0] = 0;

        strcat(fullname, entry->d_name );
        fullname[PATH_MAX-1] = 0;

        err = stat( fullname, &sbuf );
        if( err < 0 )
        {
            if( errno == ENOSYS ) {
                diag_printf(" %14s",entry->d_name);
                diag_printf(" <no status available>");
            } else FS_ERR_BREAK( stat, err );
        }
        else
        {
             if (S_ISDIR(sbuf.st_mode)) {
                 diag_printf(" d ");
             } else {
                 diag_printf(" f ");
             }

             tmp_time = sbuf.st_mtime;
             diag_printf("%9ld  %-48s", (unsigned long) sbuf.st_size, entry->d_name);
         }

        diag_printf("\n");
    }

    err = closedir( dirp );
    if( err < 0 ) FS_ERR_NO_RETURN( stat, err );
    if ( numgot != NULL )
        *numgot = num;

    return;
}


/*
 * Function: createfile
 * Description: To create a file with data
 * Input: str  - pointer to the data string
 *        name - name of the file
 * Output:
 */
static void createfile( char *str, char *name )
{
    int fd;
    ssize_t wrote;
    int err;
    size_t size = strlen(str);


    err = access( name, F_OK );
    if( err < 0 && errno != EACCES ) FS_ERR_RETURN( access, err );


    fd = open( name, O_APPEND|O_CREAT|O_WRONLY );
    if( fd < 0 ) FS_ERR_RETURN( open, fd );

    while( size > 0 )
    {
        ssize_t len = size;
        if ( len > IOSIZE ) len = IOSIZE;

        wrote = write( fd, str, len );
        if( wrote != len ) FS_ERR_BREAK( write, wrote);

        size -= wrote;
        str = str + len;
    }

    err = close( fd );
    if( err < 0 ) FS_ERR_RETURN( close, err);

    return;
}


/*
 * Function:  displayfile
 * Description:  To display a given file on the console
 * Input: name - name of the file
 * Output: display the content of file on console
 */
static void displayfile( char *name )
{
    unsigned char buf[IOSIZE];
    int fd;
    ssize_t done;
    int i;
    int err;


    err = access( name, F_OK );
    if( err != 0 ) FS_ERR_RETURN( access, err);

    fd = open( name, O_RDONLY );
    if( fd < 0 ) FS_ERR_RETURN( open, fd);

    for(;;)
    {
        done = read( fd, buf, IOSIZE );
        if( done < 0 )  FS_ERR_BREAK( read, done);

        if( done == 0 ) break;

        for( i = 0; i < done; i++ ) {
            diag_printf("%c", buf[i]);
        }

    }
    diag_printf("\n");

    err = close( fd );
    if( err < 0 ) FS_ERR_RETURN( close, err);

    return;
}


/*
 * Function:  copyfile
 * Description:  To copy a file
 * Input: name2 - absolute destination name
 *        name1 - absolute given name
 * Output:
 */
static void copyfile( char *name2, char *name1 )
{

    int err;
    char buf[IOSIZE];
    int fd1, fd2;
    ssize_t done, wrote;

    err = access( name1, F_OK );
    if( err < 0 && errno != EACCES ) FS_ERR_RETURN( access, err);

    err = access( name2, F_OK );
    if( err != 0 ) FS_ERR_RETURN( access, err);

    fd1 = open( name1, O_WRONLY|O_CREAT );
    if( fd1 < 0 ) FS_ERR_RETURN( open, fd1);

    fd2 = open( name2, O_RDONLY );
    if( fd2 < 0 ) {
        close(fd1);
        FS_ERR_RETURN( open, fd2);
    }

    for(;;)
    {
        done = read( fd2, buf, IOSIZE );
        if( done < 0 || done > IOSIZE) FS_ERR_BREAK( read, done);

        if( done == 0 ) break;

        wrote = write( fd1, buf, done );
        if( wrote != done ) FS_ERR_BREAK( write, wrote);

        if( wrote != done ) break;
    }

    err = close( fd1 );
    if( err < 0 ) {
        close( fd2 );
        FS_ERR_RETURN( close, err);
     }

    err = close( fd2 );
    if( err < 0 ) FS_ERR_RETURN( close, err);

    return;
}


/*
 * Function: check_cwd
 * Description:  To check if a given path exist
 * Input: cwd - pointer to string of the path
 * Output: 0 - fail and 1 - pass
 */
int check_cwd( const char *cwd )
{
    static char cwdbuf[PATH_MAX];
    char *ret;

    ret = getcwd( cwdbuf, sizeof(cwdbuf));
    if( ret == NULL ) FS_ERR_FAIL_RETURN( getcwd, ret);

    if( strcmp( cwdbuf, cwd ) != 0 )
    {
        diag_printf( "FAIL: cwdbuf %s cwd %s\n",cwdbuf, cwd );
        return 0;
    }

    return 1;
}


/*
 * Function:  fs_mount
 * Description: To mount flash /dev/flash1 to /
 * Input:
 * Output:
 */
int fs_mount(void)
{
    int err;

    err = mount( CYGDAT_IO_FLASH_BLOCK_DEVICE_NAME_1, "/", "jffs2" );
    if( err < 0 ) FS_ERR_FAIL_RETURN( mount, err);
    diag_printf("/dev/flash1 is mounted at \"/\". JFFS2 file system.\n");

    return 0;
}


/*
 * Function: fs_umount
 * Description: To unmount /dev/flash1 to /
 * Input:
 * Output:
 */
int fs_umount(void)
{
    int err;

    err = umount( "/" );
    if( err < 0 ) FS_ERR_FAIL_RETURN( umount, err);

    return 0;
}


/*
 * Function:
 * Description:
 * Input:
 * Output:
 */
int flash_cli_cmd_init(void)
{
    int err;
    strcpy(flash_current_dir, "/");

    err = chdir( "/" );
    if( err < 0 ) FS_ERR_FAIL_RETURN( chdir, err);

    return 0;
}


/*
 * Function: flash_cli_cmd_exit
 * Description: This function takes care of proper exit from fs_cli>
 * Input:
 * Output:
 */
int flash_cli_cmd_exit(void)
{
    strcpy(flash_current_dir, "/");
    flash_cli_mode = false;
    strcpy(cli_prompt,"onu->");
    return 0;
}


/*
 * Function: flash_cli_cmd_mkdir
 * Description: Cli make dir support
 * Input: dir_name - name of the dir
 * Output: 0 - pass
 */
int flash_cli_cmd_mkdir(char *dir_name)
{
    int err;

    err = mkdir( dir_name, 0 );
    if( err < 0 ) FS_ERR_FAIL_RETURN( mkdir, err);

    return 0;
}


/*
 * Function: flash_cli_cmd_rmdir
 * Description:  FS_CLI remove dir support
 * Input: dir_name - name of the dir
 * Output: 0 - pass
 */
int flash_cli_cmd_rmdir(char *dir_name)
{
    int err;

    err = rmdir(dir_name);
    if( err < 0 ) FS_ERR_FAIL_RETURN( rmdir, err);

    return 0;
}


/*
 * Function: flash_cli_cmd_pwd
 * Description: FS_CLI current path display
 * Input:
 * Output: 0 -pass
 */
int flash_cli_cmd_pwd(void)
{
    int i = 0;
    char pwd[256];


    getcwd(pwd, 255);
    while(pwd[i]) {
        diag_printf("%c", pwd[i++]);
    }
    diag_printf("\n");

    return 0;
}


/*
 * Function: flash_cli_cmd_ls
 * Description: FS_CLI list all the file and dir for current dir
 * Input:
 * Output: 0 - pass
 */
int flash_cli_cmd_ls(void)
{

    listdir( ".", NULL);

    return 0;
}


/*
 * Function: flash_cli_cmd_cd
 * Description: FS_CLI change dir to a given path
 * Input: dir_name - dir name to change to
 * Output: 0 - pass
 */
int flash_cli_cmd_cd(char *dir_name)
{
    int err;

    err = chdir(dir_name);
    if( err < 0 ) FS_ERR_FAIL_RETURN( chdir, err );

    return 0;
}


/*
 * Function: flash_cli_cmd_cp
 * Description: FS_CLI copy a file
 * Input: file1 - given file
 *        file2 - destination file
 * Output: 0 - pass
 */
int flash_cli_cmd_cp(char *file1, char *file2)
{

    copyfile(file1, file2);

    return 0;
}


/*
 * Function: flash_cli_cmd_rm
 * Description:  FS_CLI to remove a fiven file
 * Input: file_name - file to be removed
 * Output: 0 - pass
 */
int flash_cli_cmd_rm(char *file_name)
{
    int err;

    err = unlink(file_name);
    if( err < 0 ) FS_ERR_FAIL_RETURN( unlink, err );

    return 0;
}


/*
 * Function: flash_cli_cmd_df
 * Description: Flash usage tool
 * Input:
 * Output: display the particion usage and free space
 */
int flash_cli_cmd_df(void)
{
    char root_used_fs[8];
    char root_free_fs[8];
    int fs_used, fs_free;
    int fs_bcount, fs_bsize;

    cyg_fs_getinfo("/", FS_INFO_USEDSIZE, root_used_fs, 8);
    cyg_fs_getinfo("/", FS_INFO_FREESIZE, root_free_fs, 8);
    flash_get_block_info(&fs_bsize, &fs_bcount);
    //fs_used = *(cyg_uint32*)root_used_fs;
    fs_used = (fs_bsize * fs_bcount) - *(cyg_uint32*)root_free_fs;
    fs_free = *(cyg_uint32*)root_free_fs;

    diag_printf(" Filesystem          Used    Available   Use%c     Mounted\n",'%');
    diag_printf(" /dev/flash1 ");
    diag_printf("%12d ", fs_used);
    diag_printf("%12d   ", fs_free);
    diag_printf("%3d%c", (fs_used * 100 / (fs_used + fs_free)),'%');
    diag_printf("           /\n");

    return 0;
}


/*
 * Function: flash_cli_cmd_echo
 * Description: create a file with date
 * Input: str - date string
 *        file_name - file the date to be dumped
 * Output:
 */
int flash_cli_cmd_echo(char *str, char *file_name)
{

    createfile(str, file_name);

    return 0;
}


/*
 * Function: flash_cli_cmd_cat
 * Description: FS_CLI to display the a fiven file
 * Input: file_name - given file to be displayed
 * Output:
 */
int flash_cli_cmd_cat(char *file_name)
{

    displayfile(file_name);

    return 0;
}

#if 0
/*
 * Function: flash_cli_cmd_test
 * Description: FS_CLI to exec test
 * Input:
 * Output:
 */
int flash_cli_cmd_test(int tc, int len)
{
    diag_printf("Test..............Start\n");
    switch (tc) {
    case 1: jffs2_1_main(); break;
    case 2: jffs2_2_main(); break;
    case 3: jffs2_3_main(0, NULL); break;
    case 4: imst_test_cyg_start(len); break;
    default:
        diag_printf("Invalid test case\n");
    }
    diag_printf("Test..............End\n");

    return 0;
}
#endif

/*
 * Function: flash_cli_cmd_help
 * Description: FS_CLI help function
 * Input:
 * Output:
 */
int flash_cli_cmd_help(void)
{

    diag_printf("\noperation help\n");
    diag_printf("    help              - help\n");
    diag_printf("    mkdir <dir>       - make dir\n");
    diag_printf("    rkdir <dir>       - remove dir\n");
    diag_printf("    pwd               - show the current dir struct path\n");
    diag_printf("    ls                - list files in the dir\n");
    diag_printf("    cp file1 file2    - copy file\n");
    diag_printf("    rm file           - remove file\n");
    diag_printf("    df                - free disk space\n");
    diag_printf("    echo <data> file  - to create a file ex: echo \"data...\" file_name \n");
    diag_printf("    cat  file         - to display a file \n");
    diag_printf("    exit              - exit flash cli\n\n");

    return 0;
}


/*
 * Function: flash_cli_cmd_exe
 * Description: Main function to execute the cli commands
 * Input:
 * Output:
 */
int flash_cli_cmd_exe (int argc, char **argv) {
    int i;

    for (i=0; i <= (strlen(argv[0])); i++) {
        if (argv[0][i] == '\0') break;
        argv[0][i] = tolower(argv[0][i]);
    }

    if (strcmp(argv[0],"exit") == 0) {
        flash_cli_cmd_exit();
    } else if (strcmp(argv[0],"mkdir") == 0) {
        flash_cli_cmd_mkdir(argv[1]);
    } else if (strcmp(argv[0],"rmdir") == 0) {
        flash_cli_cmd_rmdir(argv[1]);
    } else if (strcmp(argv[0],"pwd") == 0) {
        flash_cli_cmd_pwd();
    } else if (strcmp(argv[0],"ls") == 0) {
        flash_cli_cmd_ls();
    } else if (strcmp(argv[0],"cd") == 0) {
        flash_cli_cmd_cd(argv[1]);
    } else if (strcmp(argv[0],"cp") == 0) {
        flash_cli_cmd_cp(argv[1], argv[2]);
    } else if (strcmp(argv[0],"rm") == 0) {
        flash_cli_cmd_rm(argv[1]);
    } else if (strcmp(argv[0],"df") == 0) {
        flash_cli_cmd_df();
    } else if (strcmp(argv[0],"echo") == 0) {
        flash_cli_cmd_echo(argv[1], argv[2]);
    } else if (strcmp(argv[0],"cat") == 0) {
        flash_cli_cmd_cat(argv[1]);
    }
    #if 0
    else if (strcmp(argv[0],"jffs2test") == 0) {
        flash_cli_cmd_test(iros_strtol(argv[1]), 0);
    }
    else if (strcmp(argv[0],"fileiotest") == 0) {
        flash_cli_cmd_test(4, iros_strtol(argv[1]));
    }
    #endif

    else if (strcmp(argv[0],"help") == 0) {
        flash_cli_cmd_help();
    }

    return 0;
}


