/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgantelm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 13:14:33 by bgantelm          #+#    #+#             */
/*   Updated: 2016/01/26 15:40:38 by bgantelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int		main(int ac, char **av)
{
	t_tool        	*tools;

	tools = malloc(sizeof(*tools));
	if (ac != 2)
	{
		ft_putendl("Bad number of arguments");
		return(1);
	}
	tools->mlx_ptr = mlx_init();
	tools->mlx_win = mlx_new_window(tools->mlx_ptr, 1280, 960, "I <3 FdF");
	if (fill_tools(tools, av[1]) == -1)
	{
		ft_putendl("Invalid File");
		return (1);
	}
	tools->fd = open(av[1], O_RDONLY);
	launch_fdf(tools);
	mlx_loop(tools->mlx_ptr);
	return (0);
}
