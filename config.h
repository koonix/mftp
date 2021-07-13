/* mftp's config */

/* specify a terminal file manager */
static const char* file_manager[] = { "lfrun", "/home/koonix/phone", NULL };

/* name, type, host, port, username, password */
static const Server servers[] = {
    { "phone", FTP, "192.168.1.15", "2121", "koonix", "ftp" },
};
