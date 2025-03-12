/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahouass <ahouass@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:40:09 by mohaben-          #+#    #+#             */
/*   Updated: 2025/03/12 16:51:32 by ahouass          ###   ########.fr       */
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
}

int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == '&');
}

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quotes(char c)
{
	return (c == '"' || c == '\'');
}

int	is_parentesis(char c)
{
	return (c == '(' || c == ')');
}

static void ft_handle_quotes(char *input, int *i, t_token_node **head, t_token_node **current, int *error)
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
        	ft_add_token(head, current, token_squote, str);
        free(str);
    }
    else
	{
        write(2, "Error: Unclosed quote\n", 22);
		*error = 1;
	}
}

static void	ft_handle_str(char *input, int *i, t_token_node **head, t_token_node **current)
{
	int		start;
	char	*str;

	start = *i;
	while (input[*i] && !is_operator(input[*i]) && !is_whitespace(input[*i]) && !is_quotes(input[*i]) && !is_parentesis(input[*i]))
		(*i)++;
	str = ft_substr(input, start, *i - start);
	if (is_operator(input[*i]) || is_quotes(input[*i]) || is_parentesis(input[*i]))
		(*i)--;
	ft_add_token(head, current, token_cmd, str);
	free(str);
}

t_token_node	*ft_token_last(t_token_node *list)
{
	if (!list)
		return (NULL);
	while(list->next)
	{
		list = list->next;
	}
	return (list);
}

void	ft_token_syntax_error(t_token_node *list, int *error)
{
	t_token_node	*tmp = list;
	int paren_count = 0;

	if (!list)
		return ;
	if (is_operator(*(list->data)) && ft_strcmp(list->data, "<") && ft_strcmp(list->data, "<<"))
	{
		write(2, "syntax error near unexpected token `", 37);
		write(2, list->data, ft_strlen(list->data));
		write(2, "'\n", 2);
		*error = 1;
		return ;
	}
	else
	{
		while (tmp->next)
		{
			if (is_operator(*(tmp->data)) && is_operator(*(tmp->next->data)))
			{
				write(2, "syntax error near unexpected token `", 37);
				write(2, tmp->next->data, ft_strlen(tmp->next->data));
				write(2, "'\n", 2);
				*error = 1;
				return ;
			}
			tmp = tmp->next;
		}
	}
	tmp = list;
	while (tmp)
	{
    	if (tmp->type == token_paren_open)
        	paren_count++;
    	else if (tmp->type == token_paren_close)
    	{
        	paren_count--;
        	if (paren_count < 0)
        	{
            	write(2, "syntax error: unbalanced parentheses\n", 37);
            	*error = 1;
            	return;
        	}
    	}
    	tmp = tmp->next;
	}
	if (paren_count > 0)
	{
    	write(2, "syntax error: unbalanced parentheses\n", 37);
    	*error = 1;
	}
	if (is_operator(*(ft_token_last(list)->data)))
	{
		write(2, "syntax error near unexpected token `", 37);
		write(2, ft_token_last(list)->data, ft_strlen(ft_token_last(list)->data));
		write(2, "'\n", 2);
		*error = 1;
	}
}

t_token_node	*ft_tokenize(char *input)
{
	t_token_node	*head;
	t_token_node	*current;
	int				i;
	int				error;

	i = 0;
	error = 0;
	head = NULL;
	current = NULL;
	while (input && input[i])
	{
		
			while (input[i] && is_whitespace(input[i]))
				i++;

        	if (input[i] == '"' || input[i] == '\'')
        	{
            	ft_handle_quotes(input, &i, &head, &current, &error);
				if (error)
					break;
        	}
			else if (input[i] == '|' && input[i + 1] == '|')
			{
				ft_add_token(&head, &current, token_or, "||");
				i++;
			}
			else if (input[i] == '&' && input[i + 1] == '&')
			{
				ft_add_token(&head, &current, token_and_and, "&&");
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
			else if (input[i] == '&')
				ft_add_token(&head, &current, token_and, "&");
			else if (input[i])
				ft_handle_str(input, &i, &head, &current);
			i++;
	}
	ft_token_syntax_error(head, &error);
	if (error)
    {
        ft_token_node_free(&head);
        return (NULL);
    }
	return (head);
}
