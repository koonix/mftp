#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "server.h"
#include "util.h"
#include "tui.h"


#define SERVERS_PER_PAGE 9


typedef enum {
    FTP,
    SSH
} Type;

typedef struct {
    const char* name;
    const Type  type;
    const char* host;
    const char* port;
    const char* user;
    const char* pass;
    const char* mountpoint;
    const char* keyfile;
} Server;


#include "config.h"
const int server_count = sizeof(servers) / sizeof(servers[0]);


static int mount(int page, int num);
static Server* get_server(int page, int num);
static void mounting_animation();
static void run_while_process_running(pid_t procpid, void (*fn)());
static void open_filemanager();
static void unmount();
static void mount_ftp(Server sv);


void print_page(int page)
{
    if (page > get_total_pages())
        return;

    int start = SERVERS_PER_PAGE * (page - 1);
    int end   = (SERVERS_PER_PAGE * page) - 1;
    for (int i = start, j = 1; i < server_count && i <= end; i++, j++)
        printf("%d: %s\n", j, servers[i].name);
}


int get_total_pages()
{
    return (server_count > 0) ? (server_count - 1) / SERVERS_PER_PAGE + 1 : 1;
}


void mount_and_open(int page, int num)
{
    if (!mount(page, num))
        return;
    open_filemanager();
    unmount();
}


static int mount(int page, int num)
{
    Server* sv = get_server(page, num);
    if (sv == NULL)
        return 1;

    switch (sv->type) {
        case FTP: mount_ftp(*sv); break;
        /* case SSH: mount_ssh(Server sv); break; */
    }

    return 0;
}


static void open_filemanager()
{
    tui_disable();
    pid_t pid = fork();
    if (pid == 0) {
        execvp( ((char **)file_manager)[0], (char **)file_manager );
        exit(0);
    }
    waitpid(pid, NULL, 0);
    tui_init();
}


static void unmount()
{
    pid_t pid = fork();
    if (pid == 0) {
        execlp("fusermount", "fusermount", "-u",
                "/home/koonix/phone", (char *) NULL);
        exit(0);
    }
    waitpid(pid, NULL, 0);
}


static void mount_ftp(Server sv)
{
    char opts[200];
    char host[200];

    sprintf(opts, "utf8,transform_symlinks,user=%s:%s", sv.user, sv.pass);
    sprintf(host, "ftp://%s:%s", sv.host, sv.port);

    pid_t pid = fork();
    if (pid == 0) {
        /* execlp("timeout", "timeout", "5s", "curlftpfs", "-o", opts, host, */
        /*         sv.mountpoint, (char *) NULL); */
        execlp("sleep", "sleep", "2", (char *)NULL);
        exit(0);
    }

    run_while_process_running(pid, mounting_animation);
    waitpid(pid, NULL, 0);
}


static void run_while_process_running(pid_t procpid, void (*fn)())
{
    pid_t fnpid = fork();
    if (fnpid == 0) {
        fn();
        exit(0);
    }

    siginfo_t info;
    waitid(P_PID, procpid, &info, WEXITED | WNOWAIT);
    kill(fnpid, SIGKILL);
    waitpid(fnpid, NULL, 0);
}


static void mounting_animation()
{
    char* cycle[] = { ".", "..", "...", };
    int num_cycles = sizeof(cycle) / sizeof(cycle[0]);
    int i = 0;

    while (1) {
        clear_line();
        printf("mounting%s\r", cycle[i]);
        fflush(stdout);
        sleep_msec(200);
        if ( ++i >= num_cycles )
            i = 0;
    }
}


static Server* get_server(int page, int num)
{
    int index = num - 1 + ( (page - 1) * SERVERS_PER_PAGE);
    if (index + 1 > server_count)
        return NULL;
    return &servers[index];
}
