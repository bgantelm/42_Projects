/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcherchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/07 17:05:56 by hcherchi          #+#    #+#             */
/*   Updated: 2016/01/22 18:50:59 by bgantelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H
# include "libft.h"
# include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <curses.h>
# include <term.h>
# define BUFFSIZE 4096
# undef tab

typedef struct          s_line
{
    char                *line;
    struct s_line       *prev;
    struct s_line       *next;
}                       t_line;

typedef struct          s_output
{
    int                 fdfile;
    char                *file;
    int                 fd;
    char                *type;
    struct s_output     *next;
}                       t_output;

typedef struct          s_input
{
    char                *file;
    char                *type;
    struct s_input      *next;
}                       t_input;

typedef struct          s_command
{
    char                *path;
    char                **args;
    int                 *piperight;
    t_output            *output;
    t_input             *input;
    int                 close_stand;
    int                 close[2];
    int                 fdredir[2];
    struct s_command    *next;
}                       t_command;

void                    choose_command(t_command *list, char ***env);
void                    fill_close_stand(t_command *new, char *subcmdr);
int                     opt_env(int *i, int *i_opt, char ***env, char **args);
int                     opt_env2(int *i, char ***env, char **args);
void                    real_command(t_command *list, char ***env, char ***tempenv);
void                    run_env(t_command *list, int i, char **tempenv);
int                     ft_env(char ***env, char **args);
char                    **newtab();
char                    **cuttab(char **tab, int i);
void                    ft_sig_int(int sig);
void                    smaller_than_prompt(t_line **l_line, size_t *position, int len_prompt);
void                    prompt_up(t_line **l_line, size_t *position, int len_prompt);
void	                mainloop(char *buffer, size_t *pos, t_line **l_line, size_t len_prompt);
void	                m2(char *buf, size_t *pos, t_line **l_line, t_line **hist, char **save);
void                    smaller_than_prompt(t_line **l_line, size_t *position, int len_prompt);
void                    else_down(t_line **l_line, size_t *position);
void                    send(size_t *pos, char ***env, t_line **hist, t_line **l_line, int *l_p);
void                    dredir_left(t_command *list);
void	                execute(char **env, char **av, char *path);
char	                *strchange(char *str, char c, char *str2);
void                    lvlup(char ***myenv);
int                     error3(char *line);
void                    fill_input(t_command *new, char *subcmdr);
void                    fill_output(t_command *new, char *subcmdr);
char                    *find_name(char *subcmdr, int i);
int                     find_fd(char *subcmdr, int i);
char                    *find_type(char *subcmdr, int i);
int                     error2(char *line);
int                     error_command(char *line);
int                     redir_error(char *line);
int                     syntax_error(char *line);
int                     error1(char *line, char c);
int		                find_complement(int c);
int                     find_open_close(char *line, int ret);
void                    keyboard_ctrl_d(char **line, size_t *position);
void                    keyboard_line_down(t_line **l_line, size_t *position, int len_prompt);
void                    keyboard_line_up(t_line **l_line, size_t *position, int len_prompt);
char                    *ft_lstjoin(t_line *lst);
void                    add_endline(t_line **final_line, char *line);
char                    *del_backslash(char *str);
void                    keyboard_copy(char *line, char **save, size_t position);
void                    keyboard_cut(char **line, char **save, size_t *position);
void                    keyboard_paste(char *save, char **line, size_t *position);
void                    keybord_copy(char *line, char **save, size_t position);
void                    print_historic(t_line *historic);
void                    keyboard_right_word(char **line, size_t *position);
void                    keyboard_left_word(char **line, size_t *position);
void                    keyboard_home(size_t *position);
void	                keyboard_end(char **line, size_t *position);
void                    enter(size_t *pos, char ***env, t_line **his, t_line **l_line, int *l_p);
char			        *del_char_to_str(char *src, unsigned int index);
void	                keyboard_delete(char **line, size_t *position);
void	                keyboard_ctrlc(char **line, size_t *position, char **env);
void                    ft_test(char **myenv, int len_prompt);
void	                keyboard_right(char **line, size_t *position);
void	                keyboard_print(char **buffer, char **line, size_t *position);
char			        *add_char_to_str(char *src, char c, unsigned int index);
void	                keyboard_right(char **line, size_t *position);
void		            ft_sig_int(int sig);
int				        ft_outc(int c);
void	                keyboard_left(size_t *position);
void                    get_command_line(char **env);
void                    prompt(char *home);
char                    *get_elem(char **env, char *elem);
t_command               *init_list(char *paths, char *command);
void                    add_command(t_command **list, char *subcommand, char *paths);
void                    fill_path(t_command *new, char *subcommand, char *paths);
int                     find_stderrout(char *subcommand);
void                    endofpipe(t_command *list);
char                    *add_path(char *p, char *n, char c);
char                    *get_firstword(char *str);
void                    select_path(char *paths, char *elem, char **good_path);
char                    *get_elem(char **env, char *elem);
void                    print_list(t_command *list);
void                    split_redir(char *subcommand, char **subcmdl, char **subcmdr);
void                    ft_puttab(char **tab);
void                    fill_struct(t_command *new, char *subcommand, char *paths);
void                    run_commands(t_command *list, char ***env);
void                    ft_puttabnum(int *tab);
int                     check_redir(char *subcmdr);
int                     check_block(char *str);
void                    add_input(t_input **alist, char *type, char *file);
void                    print_input(t_input *input);
void	                add_output(t_output **output, char *type, char *file, int fd);
void                    fill_in_out(t_command *new, char *subcmdl);
int                     is_type(char *split);
void                    print_output(t_output *output);
void                    print_input(t_input *input);
void                    redir_left(t_command *list);
void                    redir_right(t_command *list);
void                    father_part(t_command *list);
void                    son_part(t_command *list, char **env);
void                    set_commands(char *command_line, char *paths, char ***env);
void                    ft_cd(char ***myenv, char **args);
char                    *cdsplit(char *split, char *direction);
void                    cd_error(char *newpath, char **direction);
char                    *del_path(char *s);
int                     tablen(char **tab);
char                    **add_tab(char **tab, char *elem);
void                    ft_setenv(char ***myenv, char *av1, char *av2, char **av);
void                    tabdel(char ***tab);
char                    *get_name(char *line);
char	                **tab_dup(char **env);
char                    **remove_tab(char **tab, char *elem);
void                    ft_unsetenv(char ***myenv, char *av1, char **av);
void                    fill_close(t_command *new, char *subcmdr);
void                    fill_fdredir(t_command *new, char *subcmdr);
int                     raw_term_mode(void);
void                    default_terminal_mode(void);
void                    add_line(t_line **historic, char *line);
void                    keyboard_up(char **line, size_t *position, t_line **historic);
void                    list_begin(t_line **historic);
void                    keyboard_down(char **line, size_t *position, t_line **historic);

#endif
