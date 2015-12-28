/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GNL.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcherchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/05 17:17:56 by hcherchi          #+#    #+#             */
/*   Updated: 2015/12/28 16:55:17 by hcherchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	find_and_replace(char *line)
{
	int i;

	i = 0;
	if (line == NULL)
		return (0);
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (0);
	if (line[i] == '\n')
	{
		line[i] = '\0';
		if (line[i + 1] == '\0')
			return (0);
	}
	return (1);
}

static char	*ft_realloc(char *buf, size_t size, int *count)
{
	char *newbuf;

	*count = *count + 1;
	newbuf = ft_strnew(size);
	if (buf)
	{
		if (newbuf)
			ft_memcpy(newbuf, buf, ft_strlen(buf));
	}
	ft_memdel((void **)&buf);
	return (newbuf);
}

char		*joinsave(char *psave, char *save)
{
	char	*join;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (psave == NULL)
		return (save);
	join = ft_strnew(ft_strlen(psave) + ft_strlen(save));
	while (psave[i] != '\0')
	{
		join[j] = psave[i];
		i++;
		j++;
	}
	i = 0;
	while (save[i] != '\0')
	{
		join[j] = save[i];
		j++;
		i++;
	}
	ft_memdel((void **)&save);
	return (join);
}

int			readnl(int fd, char **save)
{
	char	*psave;
	int		ret;
	int		count;

	count = 0;
	psave = *save;
	*save = ft_strnew(BUFF_SIZE);
	while ((ret = read(fd, *save + BUFF_SIZE * count, BUFF_SIZE)) > 0)
	{
		if (ret == BUFF_SIZE)
			*save = ft_realloc(*save, BUFF_SIZE * (count + 2), &count);
		if (ft_strchr(*save, '\n'))
		{
			*save = joinsave(psave, *save);
			return (1);
		}
	}
	if (ret == -1)
		return (-1);
	*save = joinsave(psave, *save);
	if (ret == 0 && ft_strlen(*save) == 0)
		return (0);
	return (1);
}

int			get_next_line(const int fd, char **line)
{
	static char	*save = NULL;
	int			check;
	static char	*todel = NULL;

	check = 1;
	if (!save || !strchr(save, '\n'))
	{
		check = readnl(fd, &save);
		ft_memdel((void **)&todel);
		if (check != 0)
			todel = save;
	}
	if (check == -1)
		return (-1);
	if (check == 0)
		ft_memdel((void **)&save);
	*line = save;
	if (find_and_replace(save) == 0)
		save = NULL;
	else
		save += ft_strlen(save) + 1;
	return (check);
}
