#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */



#include "ntshell.h"
#include "ntopt.h"
#include "ntlibc.h"
#include "cmd.h"

int fd; /* File descriptor for the port */
int n;

ntshell_t ntshell;

typedef struct {
    char *command;
    char *description;
    void (*func)(int argc, char **argv);
} command_table_t;

const command_table_t cmdlist[] = {
    {"foo", "foo command", cmd_foo },
    {"bar", "bar command", cmd_bar },
    {"baz", "baz command", cmd_baz },
	{"sum", "Sum function", cmd_sum },
    {NULL, NULL, NULL}
};

int func_read(char *buf, int cnt, void *extobj);
int func_write(const char *buf, int cnt, void *extobj);
int func_cb_ntshell(const char *text, void *extobj);
int func_cb_ntopt(int argc, char **argv, void * extobj);

/**
 * Serial read function.
 */
int func_read(char *buf, int cnt, void *extobj) {
	read(fd, buf, cnt);
    return 0;
}

/**
 * Serial write function.
 */
int func_write(const char *buf, int cnt, void *extobj) {
	int n;
	n = write(fd, buf, cnt);
	if (n < 0)
		fputs("write() failed!\n", stderr);
    return 0;
}

/**
 * Callback function for ntshell module.
 */
int func_cb_ntshell(const char *text, void *extobj) {
    return ntopt_parse(text, func_cb_ntopt, NULL);
}

/**
 * Callback function for ntopt module.
 */
int func_cb_ntopt(int argc, char **argv, void * extobj) {
    if (argc == 0) {
        return -1;
    }    
    int execnt = 0;
    const command_table_t *p = &cmdlist[0];
    while (p->command != NULL) {
        if (strcmp(argv[0], p->command) == 0) {
            p->func(argc, argv);
            execnt++;
        }
        p++;
    }
    if (execnt == 0) {
        printf("Command not found.\n");
    }
    return 0;
}

/**
 * Application code.
 */
int main() {

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
fd = open("/dev/com1", O_RDWR | O_NOCTTY | O_NDELAY);
if (fd == -1)
	{
	/*
    * Could not open the port.
    */
    perror("open_port: Unable to open /dev/com1 - ");
	}
else
	{
	printf("open_port: OK\n");
    fcntl(fd, F_SETFL, 0);
	}
n = write(fd, "Start Shell\r\n", 13);
if (n < 0)
  fputs("write() failed!\n", stderr);
ntshell.func_read = func_read;
ntshell.func_write = func_write;
ntshell.func_callback = func_cb_ntshell;
ntshell_execute(&ntshell);
}
