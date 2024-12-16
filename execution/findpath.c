/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   findpath.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 11:24:30 by mshaheen          #+#    #+#             */
/*   Updated: 2024/12/17 02:14:27 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_path_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**findsplit_path(char **env)
{
	char	*pathy;
	char	**paths;
	int		i;

	i = 0;
	pathy = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			pathy = env[i] + 5;
			break ;
		}
		i++;
	}
	paths = ft_split(pathy, ':');
	return (paths);
}

int	accessible_p(char **array, char *fp, t_shell *data)
{
	if(access(fp, F_OK) == 0)
	{
		if(access(fp, X_OK) == 0)
		{
			free_path_array(array);
			data->exit_code = 0;
			return (1);
		}
		data->exit_code = 126;
		return (0);
	}
	data->exit_code = 127;
	return (0);
}

char *find_in_path(char *cmd, int *input, int *output, t_shell *data)
{
	char	**paths;
	char	*full_path;
	char	*tmp;
	int		i;

	i = 0;
	paths = findsplit_path(data->envi);
	if (!paths)
		free_exec_fail(data, input, output, 12);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if(!tmp)
			free_exec_fail(data, input, output, 12);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if(!full_path)
			free_exec_fail(data, input, output, 12);
		if (accessible_p(paths, full_path, data) == 1)
			return (full_path);
		free(full_path);
		i++;
	}
	free_path_array(paths);
	return (NULL);
}

char *check_file(char *cmd, t_shell *data)
{
	if(access(cmd, F_OK) == 0)
	{
		if(access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
		{
			//perror(cmd);
			data->exit_code = 126;
		}
	}
	else
	{
		//perror(cmd);
		data->exit_code = 127;
	}
	return(NULL);
}

char	*get_cmd_path(char *cmd, int *input, int *output, t_shell *data)
{
	char	*res;

	if (ft_strchr(cmd, '/'))
		return (check_file(cmd, data));
	res = find_in_path(cmd, input, output, data);
	return (res);
}

// char	*get_cmd_path(char *cmd, char **env)
// {
// 	char	*full_path;
// 	char	**paths;
// 	char	*tmp;
// 	int		i;

// 	i = 0;
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		if (access(cmd, X_OK) == 0)
// 			return (ft_strdup(cmd));
// 	}
// 	paths = findsplit_path(env);
// 	if (!paths)
// 		return (NULL);
// 	while (paths[i])
// 	{
// 		tmp = ft_strjoin(paths[i++], "/");
// 		full_path = ft_strjoin(tmp, cmd);
// 		free(tmp);
// 		if (accessible_p(paths, full_path) == 1)
// 			return (full_path);
// 		free(full_path);
// 	}
// 	free_path_array(paths);
// 	return (NULL);
// }
