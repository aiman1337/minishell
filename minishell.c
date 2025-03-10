#include "minishell.h"

void	ft_handle_sigint(int sig)
{
 	(void)sig;
	write(1, "\nminishell> ", 12);
}

void	ft_clear_screen()
{
	write(1, "\033[H\033[2J", 7);
	write(1, "\nminishell> ", 12);
}


void print_token_type(t_token_type type)
{
    switch (type)
    {
    case token_cmd:
        printf("Command: ");
        break;
    case token_pipe:
        printf("Pipe: ");
        break;
    case token_in:
        printf("Redirect In: ");
        break;
    case token_out:
        printf("Redirect Out: ");
        break;
    case token_hrdc:
        printf("Here Document: ");
        break;
    case token_appnd:
        printf("Append: ");
        break;
    default:
        printf("Unknown token type: ");
        break;
    }
}

void print_tokens(t_token_node *tokens)
{
    t_token_node *current = tokens;

    while (current != NULL)
    {
        print_token_type(current->type);
        printf("%s\n", current->data);
        current = current->next;
    }
}

static void	print_token_error(char token)
{
	write(1, "minishell: syntax error near unexpected token `", 47);
	write(1, &token, 1);
	write(1, "'\n", 2);
}

int	ft_is_syntax_error(char *input, int *i)
{
	char	current;
	int 	j;

	while (input[*i] && (input[*i] == ' ' || input[*i] == '\t'))
		(*i)++;
	if (input[*i] == '|' && (*i == 0 || input[*i-1] == '|' || (input[*i-1] == ' ' && input[*i-2] == '|')))
	{
		print_token_error('|');
		return (1);
	}
	if (input[*i] == '<' || input[*i] == '>')
	{
		current = input[*i];
		if (input[*i + 1] == current)
			(*i)++;
		j = *i + 1;
		while (input[j] && (input[j] == ' ' || input[j] == '\t'))
			j++;
		if (!input[j] || input[j] == '|' || input[j] == '<' || input[j] == '>')
		{
			print_token_error(input[j] ? input[j] : current);
			return (1);
		}
	}
	return (0);
}

static void	ft_hanlde_input_error(char *input, int *last_exit_status)
{
	int	i;

	i = 0;
	if (!input || input[0] == '\0')
		return ;
	while (input[i])
	{
		if (ft_is_syntax_error(input, &i))
		{
			*last_exit_status = 258;
			return ;
		}
		i++;
	}
}

int main(int ac, char **av, char **envp)
{
	char	*input;
	int		pid;
	int		last_exit_status;
	int		status;

	(void)ac;
	(void)av;
	last_exit_status = 0;
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
	while (1)
	{
		input = readline("minishell> ");
		ft_hanlde_input_error(input, &last_exit_status);
		if (input && *input)
			add_history(input);
		if (!input)
		{
			write(1, "exit\n", 5);
			clear_history();
			exit(0);
		}
		t_token_node *tokens = ft_tokenize(input);
		print_tokens(tokens);
		if (!ft_strncmp(input, "exit", 4))
		{
			char	**splited_cmd = ft_split(input, ' ');
			
			if (splited_cmd[1])
			{
				free(input);
				write(1, "exit\n", 5);
				write(1, "minishell: exit: too many arguments\n", 36);
				last_exit_status = 1;
				continue ;
			}
			else
			{
				free(input);
				write(1, "exit\n", 5);
				clear_history();
				if (!splited_cmd[1])
					exit(0);
				else if (splited_cmd[1] && *splited_cmd[1] == '-' && ft_isdigit(*(splited_cmd[1] + 1)))
					exit(ft_atoi(splited_cmd[1]));
				else
				{
					write(1, "minishell: exit: xx: numeric argument required", 46);
					exit(255);
				}
			}
		}
		if (!ft_strncmp(input, "echo $", 6))
		{
			if (*(input + 6) == '?')
			{
				printf("%d\n", last_exit_status);
				free(input);
				continue;
			}
			if (getenv(input + 6))
				printf("%s\n", getenv(input +6));
			else
				write(1, "\n", 1);
			continue ;
		}
		if (input[0] == '\f' && input[1] == '\0')
		{
			ft_clear_screen();
			free(input);
			continue;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue;
		}
		pid = fork();
		if (pid == 0)
		{
			ft_exec_cmd(input, envp);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                last_exit_status = WEXITSTATUS(status);
			else
                last_exit_status = 1;
		}
		else
		{
			write(2, "Fork error\n", 11);
			last_exit_status = 1;
		}
		free(input);
	}
	return (0);
}
