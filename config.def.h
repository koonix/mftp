/* mftp's config */

/* specify a terminal file manager */
static const char* file_manager = "lfrun";

/* name, type, host, port, username, password */
static const Server servers[] = {
    { "phone", FTP, "192.168.1.15", "2121", "koonix", "ftp", "~/Mount/ftp", NULL },
    { "laptop", SSH, "192.168.1.30", NULL, "koonix", NULL, "~/Mount/ftp", "~.ssh/id_laptop" },
};
