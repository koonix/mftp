#ifndef PARSE_H
#define PARSE_H

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
} Server;

int get_total_pages();
void print_page(int page);
void mount_and_open(unsigned int page, unsigned int num);

#endif /* PARSE_H */
