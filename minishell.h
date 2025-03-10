/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahouass <ahouass@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 12:33:01 by mohaben-          #+#    #+#             */
/*   Updated: 2025/03/10 17:08:02 by ahouass          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef enum e_token_type
{
	token_pipe,
	token_cmd,
	token_in,
	token_out,
	token_or,
	token_and,
	token_hrdc,
	token_appnd,
	token_paren_open,
	token_paren_close,
}	t_token_type;

typedef struct s_token_node
{
	t_token_type	type;
	char			*data;
	struct s_token_node	*next;
}	t_token_node;

typedef struct s_ast
{
	t_token_type	token;
	char			*data;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

char	*get_next_line(int fd);
size_t	ft_strlen(char *s);
int		ft_isdigit(char c);
int		ft_atoi(const char *str);
char	*ft_strdup(char *s1);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strchr(char *s, int c);
int		ft_strncmp(char *s1, char *s2, size_t n);
char	**ft_split(char *s, char c);
void	free_split(char **s);
void	p_error_cmd(char **cmd, char **paths, int exit_status);
void	p_error(char *err, int exit_status);
char	*ft_get_path(char **envp);
void	ft_exec_cmd(char *cmd, char **envp);


int	ft_strcmp(const char *s1, const char *s2);
t_token_node	*ft_tokenize(char *input);

#endif