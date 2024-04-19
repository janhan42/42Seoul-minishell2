/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 18:51:30 by janhan            #+#    #+#             */
/*   Updated: 2024/04/19 19:07:59 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	*ft_dup_flag(t_parse *parse)
{
	int		*result;
	size_t	token_index;

	token_index = 0;
	result = (int *)malloc(sizeof(int) * parse->token_count);
	if (!result)
		exit(ft_error("dup_env_flag failed", 1));
	while (token_index < parse->token_count)
	{
		result[token_index] = parse->tokens[token_index].env_flag;
		token_index++;
	}
	return (result);
}

static void	ft_get_exec_arr_size(t_parse *parse, t_exec *exec)
{
	size_t	token_index;

	exec->exec_arr_size = 1;
	token_index = 0;
	while (token_index < parse->token_count)
	{
		if (parse->tokens[token_index].type == PIPE)
			exec->exec_arr_size++;
		token_index++;
	}
}

static void	ft_init_exec_info(t_exec_info *exec_info, t_parse *parse)
{
	exec_info->cmd_path = NULL;
	exec_info->cmd = NULL;
	exec_info->cmd_index = 0;
	exec_info->redirect = NULL;
	exec_info->redirect_index = 0;
	exec_info->use_pipe = FALSE;
	exec_info->infile_fd = NONE;
	exec_info->outfile_fd = NONE;
	exec_info->builtin_parent = FALSE;
	exec_info->env_flags = ft_dup_flag(parse);
	exec_info->dqoute_flags = ft_dup_dqoute_flag(parse);
	exec_info->sqoute_flags = ft_dup_sqoute_flag(parse);
}

static int	ft_add_exec_info(t_parse *parse, t_exec *exec)
{
	t_exec_info	*exec_info;
	size_t		exec_arr_i;

	parse->token_index = 0;
	exec_arr_i = 0;
	while (exec_arr_i < exec->exec_arr_size)
	{
		exec_info = &exec->exec_arr[exec_arr_i];
		ft_init_exec_info(exec_info, parse);
		if (ft_set_exec_info(parse, exec_info) == FAILURE)
		{
			ft_free_exec(exec, exec_arr_i);
			return (FAILURE);
		}
		exec_arr_i++;
		//parse->token_index++; ??????
	}
	return (SUCCESS);
}

int	ft_make_exec_info(t_info *info, t_parse *parse, t_exec *exec)
{
	ft_get_exec_arr_size(parse, exec);
	if (ft_init_exec(info, parse, exec) == FAILURE)
		return (FAILURE);
	if (ft_add_exec_info(parse, exec) == FAILURE)
	{
		ft_free_tokens(parse, parse->token_count);
		return (FAILURE);
	}
	return (SUCCESS);
}
