/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: georgijvasilcikov <georgijvasilcikov@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 19:32:13 by georgijvasi       #+#    #+#             */
/*   Updated: 2022/06/11 19:35:26 by georgijvasi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char **envp) //обработка стандартных команд(bin)
{
	char	**paths;
	char	*tmp;
	int		i;

	i = 0;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5)) // находим часть в окружении с путями
		{
			paths = ft_split(*envp + 5, ':');
			while (paths[i])
			{
				tmp = ft_strjoin(paths[i], "/");
				free(paths[i]);
				paths[i] = ft_strdup(tmp);
				free(tmp);
				i++;
			}
			return (paths);
		}
		envp++;
	}
	return (0);
}

char	*find_cmd(char **paths, char **cmd_flags)
{
	char	*cmd;
	int		i;

	i = 0;
	cmd = ft_strdup(cmd_flags[0]);
	if (!access(cmd_flags[0], F_OK))
		return (cmd);
	free(cmd);
	while (paths[i])
	{
		cmd = ft_strjoin(paths[i], cmd_flags[0]);
		if (!access(cmd, F_OK))
			return (cmd);
		else
			free(cmd);
		i++;
	}
	if (!cmd)
		errmsg("minishell: ", cmd_flags[0], ": command not found", -127);
	return (NULL);
}

int	exec_built(t_cmd *cmd) // обработка встроенных команд
{
	int	i;

	i = 1;
	if (!ft_strncmp(cmd->args[0], "exit", ft_strlen(cmd->args[0])))
		i = 1;
	else if (!ft_strncmp(cmd->args[0], "echo", ft_strlen(cmd->args[0])))
		ft_echo(cmd);
	else if (!ft_strncmp(cmd->args[0], "pwd", ft_strlen(cmd->args[0])))
		ft_pwd();
	else if (!ft_strncmp(cmd->args[0], "env", ft_strlen(cmd->args[0])))
		ft_env();
	else if (!ft_strncmp(cmd->args[0], "export", ft_strlen(cmd->args[0])))
		g_line.envp = ft_export(cmd->args);
	else if (!ft_strncmp(cmd->args[0], "unset", ft_strlen(cmd->args[0])))
		g_line.envp = ft_unset(cmd->args, g_line.envp);
	else if (!ft_strncmp(cmd->args[0], "cd", ft_strlen(cmd->args[0])))
		i = 1;
	else
		i = 0;
	return (i);
}

void	execute(t_cmd *cmd) //запуск команды на исполнение
{
	char	**paths;
	char	*name;
	pid_t	p;

	if (exec_built(cmd))
		return ;
	p = fork(); //создаем дочерний процесс, т.к. execve после выполнения закрывает процесс
	if (!p)
	{
		paths = get_paths(g_line.envp);
		if (!paths)
			errmsg("minishell: ", cmd->args[0], ": command not found", -127);
		name = find_cmd(paths, cmd->args);
		ft_free(paths);
		if (execve(name, cmd->args, g_line.envp) == -1)
			errmsg("minishell: ", cmd->args[0], ": command not found", -127);
		if (name)
			free(name);
	}
	else
		waitpid(p, &g_line.status, 0);
}
