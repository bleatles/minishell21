/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: georgijvasilcikov <georgijvasilcikov@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 19:35:04 by georgijvasi       #+#    #+#             */
/*   Updated: 2022/06/11 19:35:26 by georgijvasi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_len_eq(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
		i++;
	return (i);
}

int	size_mass(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	**new_envp(char **envp)
{
	char	**new_mass;
	int		i;

	new_mass = malloc(sizeof(char *) * (size_mass(envp) + 1));
	if (!new_mass)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_mass[i] = ft_strdup(envp[i]);
		i++;
	}
	new_mass[i] = NULL;
	return (new_mass);
}

int	ft_sym_export(char *str)
{
	int	j;

	j = 0;
	while (str[j] && str[j] != '=')
		j++;
	return (j);
}

void	id_cmd(void)
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = g_line.cmd;
	while (cmd)
	{
		cmd->i = ft_itoa(i);
		i++;
		cmd = cmd->next;
	}
}
