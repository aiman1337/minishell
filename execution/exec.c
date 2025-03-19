/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohaben- <mohaben-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 11:10:22 by mohaben-          #+#    #+#             */
/*   Updated: 2025/03/19 12:03:34 by mohaben-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



void	execute_ast(t_ast_node *ast, t_exec *exec)
{
	if (!ast)
		return ;
	
	if (ast->type == AST_COMMAND)
		execute_command(ast, exec);
	else if (ast->type == AST_PIPE)
		ft_execute_pipe(ast, exec);
	else if (ast->type == AST_AND_AND)
	{
		execute_ast(ast->left, exec);
		if (exec->exit_status == 0)
			execute_ast(ast->right, exec);
	}
	else if (ast->type == AST_OR_OR)
	{
		execute_ast(ast->left, exec);
		if (exec->exit_status != 0)
			execute_ast(ast->right, exec);
	}
	// else if (ast->type == AST_SUBSHELL)
	// 	execute_subshell(ast, exec);
}
