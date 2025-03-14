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
        printf("%s\n", current->data);
        current = current->next;
    }
}

int main(int ac, char **av, char **envp)
{
	char	*input;

	(void)ac;
	(void)av;
	(void)envp;
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("\033[1;32mminishell> \033[0m");
		if (input && *input)
			add_history(input);
		if (!input)
		{
			write(1, "exit\n", 5);
			clear_history();
			exit(0);
		}
		t_token_node *tokens = ft_tokenize(input);
		if (tokens)
			print_tokens(tokens);
		if (!ft_strncmp(input, "exit", 4))
			exit(0);
		free(input);
	}
	return (0);
}
