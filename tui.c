#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include "tui.h"

static void handle_signals();
static void hide_cursor();
static void show_cursor();
static void alt_screen();
static void orig_screen();
static void seq(char* s);
static void no_stdin_buffering();
static void restore_stdin_buffering();
static void tui_end_sigfn(int sig);

static struct termios told;

void tui_init() {
    handle_signals();
    no_stdin_buffering();
    alt_screen();
    hide_cursor();
}

void tui_end() {
    tui_disable();
    kill(0, SIGTERM);
}

void tui_disable() {
    show_cursor();
    orig_screen();
    restore_stdin_buffering();
}

void clear_line() {
    seq("K");
}

void clear_all() {
    seq("2J");
}

void clear_to_top() {
    seq("1J");
}

void clear_to_bottom() {
    seq("0J");
}

void cursor_home() {
    seq("H");
}

void cursor_up() {
    seq("A");
}

void cursor_down() {
    seq("B");
}

void cursor_line_column(int line, int column) {
    char s[15];
    sprintf(s, "%d;%dH", line, column);
    seq(s);
}

static void hide_cursor() {
    seq("?25l");
}

static void show_cursor() {
    seq("?25h");
}

static void alt_screen() {
    seq("?1049h");
}

static void orig_screen() {
    seq("?1049l");
}

static void seq(char* s) {
    printf("\033[%s", s);
}

static void no_stdin_buffering() {
    struct termios tnew;
    tcgetattr(STDIN_FILENO, &told);
    tnew = told;
    tnew.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tnew);
}

static void restore_stdin_buffering() {
    tcsetattr(STDIN_FILENO,TCSANOW,&told);
}

static void handle_signals() {
    signal(SIGINT, tui_end_sigfn);
    signal(SIGTERM, tui_end_sigfn);
    signal(SIGABRT, tui_end_sigfn);
}

void tui_no_handle_sigint() {
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
}

static void tui_end_sigfn(int sig) {
    tui_end();
    exit(1);
}
