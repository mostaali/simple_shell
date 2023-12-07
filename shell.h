#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* hshcommand.c */
int hshcommand(info_t *, char **);
int scout_builtin(info_t *);
void scout_command(info_t *);
void frk_command(info_t *);

/* path.c */
int is_command(info_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* loophshcommand.c */
int loophshcommand(char **);

/* error_fun.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* string_1.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* string_2.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* string_3.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* string_4.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* memory_functions */
char *_memset(char *, char, unsigned int);
void freee(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* func_memory.c */
int freee_two(void **);

/* functions.c */
int interactive_fun(info_t *);
int delimiter(char, char *);
int _isalpha(int);
int _atoi(char *);

/* functions_second.c */
int _erratoi(char *);
void error_pr(info_t *, char *);
int pr_d(int, int);
char *convert_number(long int, int, int);
void rm_comment(char *);

/* bultin.c */
int _myexit(info_t *);
int _mycd(info_t *);
int _myhelp(info_t *);

/* bultin_second.c */
int _myhistory(info_t *);
int _myalias(info_t *);

/* line.c module */
ssize_t find_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigin(int);

/* info_fun.c module */
void rm_info(info_t *);
void info_setter(info_t *, char **);
void empty_info(info_t *, int);

/* myenv.c module */
char *_getenv(info_t *, const char *);
int _myenv(info_t *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int addenvlist(info_t *);

/* myenv_second.c module */
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* function_io.c */
char *get_history_file(info_t *info);
int history_wr(info_t *info);
int history_re(info_t *info);
int initiatehistorylist(info_t *info, char *buf, int linecount);
int savehistory(info_t *info);

/* str_list_function.c module */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t pr_liststr(const list_t *);
int removenode__index(list_t **, unsigned int);
void clean_list(list_t **);

/* str_list_function.c module */
size_t listlen(const list_t *);
char **list_to_strings(list_t *);
size_t print_the_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t getnodeindex(list_t *, list_t *);

/* shell_chain.c */
int chain(info_t *, char *, size_t *);
void ckchain(info_t *, char *, size_t *, size_t, size_t);
int change_alias(info_t *);
int change_vars(info_t *);
int change_string(char **, char *);

#endif
