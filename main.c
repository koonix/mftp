#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tui.h"
#include "server.h"

static int handle_key(char key);
static void print_info();
int page = 1;

int main()
{
    tui_init();

    while(1)
    {
        print_info();
        if (handle_key(getchar()))
            break;
    }

    tui_end();
}

static void print_info()
{
    cursor_home();
    clear_all();
    if (get_total_pages() > 1)
        printf("page %d/%d\n", page, get_total_pages());
    printf("q: quit\n");
    print_page(page);
}

static int handle_key(char key)
{
    if (key == 'q')
        return 1;
    else if (key == 'n' && page < get_total_pages())
        ++page;
    else if (key == 'p' && page > 1)
        --page;
    else if (key >= '1' && key <= '9')
        mount_and_open(*get_server(page, key - '0'));
    return 0;
}
