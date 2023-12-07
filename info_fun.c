#include "shell.h"

/**
 * rm_info - initializes info_t struct
 * @info: struct address
 */
void rm_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * info_setter - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void info_setter(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		change_alias(info);
		change_vars(info);
	}
}

/**
 * empty_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void empty_info(info_t *info, int all)
{
	freee(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			clean_list(&(info->env));
		if (info->history)
			clean_list(&(info->history));
		if (info->alias)
			clean_list(&(info->alias));
		freee(info->environ);
			info->environ = NULL;
		freee_two((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
