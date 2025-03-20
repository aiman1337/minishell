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
	case token_or:
        printf("OR: ");
        break;
	case token_paren_open:
        printf("Open: ");
        break;
	case token_paren_close:
        printf("Close: ");
        break;
	case token_dquote:
        printf("double quotes: ");
        break;
	case token_squote:
        printf("single quotes: ");
        break;
	case token_wildcard:
        printf("wildcard: ");
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
        printf("|%s|\n", current->data);
        current = current->next;
    }
}



int main(int ac, char **av, char **envp)
{
	char	*input;
	// int		pid;
	int		exit_status;
	// int		status;
	t_env	*env;

	(void)ac;
	(void)av;
	exit_status = 0;
	env = ft_init_env(envp);
	// signal(SIGINT, ft_handle_sigint);
	// signal(SIGQUIT, SIG_IGN);
	// rl_catch_signals = 0;
	while (1)
	{
		input = readline("minishell> ");
		// input = readline("\x1b[32mmini\x1b[32mshell> \x1b[0m\x1b[0m");
		if (!input)
    	{
        	ft_putstr_fd("exit\n", 1);
        	clear_history();
        	exit(0);
    	}

    	// Only add non-empty input to history
    	if (*input)
    	{
        	add_history(input);
    	}

    	// Process the input
    	if (!ft_strncmp(input, "exit", 4))
    	{
        	ft_exit(input, &exit_status);
    	}
		if (!ft_strncmp(input, "exit", 4))
			ft_exit(input, &exit_status);
		t_token_node *tokens = ft_tokenize(input);
		(void)tokens;
		// if (tokens)
		// 	print_tokens(tokens);
        t_ast_node *ast = build_ast(tokens);
		(void)ast;
        if (ast)
        {
            printf("AST Structure:\n");
            print_ast(ast, 0);
		}
		// printf("%s\n", ast->child->args[0]);
		// else if (!ft_strncmp(input, "echo $?", 7))
		// 	printf("%d\n", exit_status);
		// else if (!ft_strncmp(input, "echo", 4))
		// 	ft_echo(input);
		// else if (!ft_strncmp(input, "cd", 2))
		// 	ft_cd(input, env, &exit_status);
		// else if (!ft_strncmp(input, "export", 6))
		// 	ft_export(input, &env);
		// else if (!ft_strncmp(input, "unset", 5))
		// 	ft_unset(input, &env);
		// else if (!ft_strncmp(input, "env", 3))
		// 	ft_print_env(env);
		// else if (!ft_strncmp(input, "pwd", 3))
		// 	ft_pwd(env);
		// else
		// {
		// 	pid = fork();
		// 	if (pid == 0)
		// 	{
		// 		ft_exec_cmd(input, env);
		// 	}
		// 	else if (pid > 0)
		// 	{
		// 		waitpid(pid, &status, 0);
		// 		if (WIFEXITED(status))
		// 			exit_status = WEXITSTATUS(status);
		// 		else
		// 			exit_status = 1;
		// 	}
		// 	else
		// 	{
		// 		write(2, "Fork error\n", 11);
		// 		exit_status = 1;
		// 	}
		// }
		free(input);
	}
	return (0);
}
