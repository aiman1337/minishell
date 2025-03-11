/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahouass <ahouass@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:40:09 by mohaben-          #+#    #+#             */
/*   Updated: 2025/03/11 15:49:51 by ahouass          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token(t_token_node **head, t_token_node **current, t_token_type type, char *data)
{
	t_token_node	*new_token;

	new_token = malloc(sizeof(t_token_node));
	if (!new_token)
		return ;
	new_token->type = type;
	new_token->data = ft_strdup(data);
	new_token->next = NULL;
	if (!*head)
		*head = new_token;
	else
		(*current)->next = new_token;
	*current = new_token;
}

void	ft_token_node_free(t_token_node **head)
{
	t_token_node	*next;

	if (!head || !*head)
		return ;
	next = NULL;
	while (*head)
	{
		next = (*head)->next;
		free((*head)->data);
		free(*head);
		*head = next;
	}
	free(head);
}

int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')');
}

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static void ft_handle_quotes(char *input, int *i, t_token_node **head, t_token_node **current)
{
    int start;
    char *str;
    char quote_type = input[*i];
    
    (*i)++;
    start = *i;
    
    while (input[*i] && input[*i] != quote_type)
        (*i)++;
    
    if (input[*i] == quote_type)
    {
        str = ft_substr(input, start, *i - start);
		if (quote_type == '"')
            ft_add_token(head, current, token_dquote, str);
		else
        	ft_add_token(head, current, token_cmd, str);
        free(str);
    }
    else
	{
        write(1, "Error: Unclosed quote\n", 22);
	}
}

static void	ft_handle_str(char *input, int *i, t_token_node **head, t_token_node **current)
{
	int		start;
	char	*str;

	start = *i;
	while (input[*i] && !is_operator(input[*i]) && !is_whitespace(input[*i]))
		(*i)++;
	str = ft_substr(input, start, *i - start);
	if (is_operator(input[*i]))
		(*i)--;
	ft_add_token(head, current, token_cmd, str);
	free(str);
}

t_token_node	*ft_tokenize(char *input)
{
	t_token_node	*head;
	t_token_node	*current;
	int				i;

	head = NULL;
	current = NULL;
	i = 0;
	while (input && input[i])
	{
		
			while (input[i] && is_whitespace(input[i]))
				i++;

			// // Check for quotes
        	if (input[i] == '"' || input[i] == '\'')
        	{
            	ft_handle_quotes(input, &i, &head, &current);
            	i--;
        	}
			// Check for operators
			else if (input[i] == '|' && input[i + 1] == '|')
			{
				ft_add_token(&head, &current, token_or, "||");
				i++;
			}
			else if (input[i] == '&' && input[i + 1] == '&')
			{
				ft_add_token(&head, &current, token_and, "&&");
				i++;
			}
			else if (input[i] == '<' && input[i + 1] == '<')
			{
				ft_add_token(&head, &current, token_hrdc, "<<");
				i++;
			}
			else if (input[i] == '>' && input[i + 1] == '>')
			{
				ft_add_token(&head, &current, token_appnd, ">>");
				i++;
			}
			else if (input[i] == '(')
				ft_add_token(&head, &current, token_paren_open, "(");
			else if (input[i] == ')')
				ft_add_token(&head, &current, token_paren_close, ")");
			else if (input[i] == '|')
				ft_add_token(&head, &current, token_pipe, "|");
			else if (input[i] == '<')
				ft_add_token(&head, &current, token_in, "<");
			else if (input[i] == '>')
				ft_add_token(&head, &current, token_out, ">");
			else
				ft_handle_str(input, &i, &head, &current);
			i++;
	}
	// if (error) // If error occurred, free all tokens and return NULL
    // {
    //     ft_token_node_free(&head);
    //     return NULL;
    // }
	return (head);
}
