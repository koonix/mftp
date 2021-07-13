#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "server.h"
#include "util.h"
#include "tui.h"

#define SERVERS_PER_PAGE 9

static int mount(Server sv);
static Server* get_server(int page, int num);
static int wait_for_child_with_animation();
static void open_filemanager();
static void unmount();

#include "config.h"
const int server_count = sizeof(servers) / sizeof(servers[0]);

void print_page(int page)
{
    if (page > get_total_pages())
        return;

    int start = SERVERS_PER_PAGE * (page - 1);
    int end   = (SERVERS_PER_PAGE * page) - 1;
    for (int i = start, j = 1; i < server_count && i <= end; i++, j++)
        printf("%d: %s\n", j, servers[i].name);
}

void mount_and_open(int page, int num)
{
    Server* sv = get_server(page, num);
    if (sv == NULL || !mount(*sv))
        return;
    tui_disable();
    open_filemanager();
    wait(NULL);
    unmount();
    wait(NULL);
    tui_init();
}

int get_total_pages()
{
    return (server_count > 0) ? (server_count - 1) / SERVERS_PER_PAGE + 1 : 1;
}

static int wait_for_child_with_animation()
{
    char* cycle[] = { ".", "..", "...", };
    int status;
    int i = 0;

    while (waitpid(-1, &status, WNOHANG) == 0) {
        printf("\033[%s", "K");
        printf("mounting%s\r", cycle[i]);
        fflush(stdout);
        i++;
        if ( i >= sizeof(cycle) / sizeof(cycle[0]) )
            i=0;
        sleep_msec(200);
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else
        return -1;
}

static int mount(Server sv)
{
    char opts[200];
    char host[200];

    sprintf(opts, "utf8,transform_symlinks,user=%s:%s", sv.user, sv.pass);
    sprintf(host, "ftp://%s:%s", sv.host, sv.port);

    if (fork() == 0) {
        execlp("timeout", "timeout", "5s", "curlftpfs", "-o", opts, host, "/home/koonix/phone", (char *) NULL);
        exit(0);
    }

    if (wait_for_child_with_animation() == 0)
        return 1;
    else
        return 0;
}

static void open_filemanager()
{
    if (fork() == 0) {
        execvp( ((char **)file_manager)[0], (char **)file_manager );
        exit(0);
    }
}

static void unmount()
{
    if (fork() == 0) {
        execlp("fusermount", "fusermount", "-u", "/home/koonix/phone", (char *) NULL);
        exit(0);
    }
}

static Server* get_server(int page, int num)
{
    int index = num - 1 + ( (page - 1) * SERVERS_PER_PAGE);
    if (index + 1 > server_count)
        return NULL;
    return &servers[index];
}
