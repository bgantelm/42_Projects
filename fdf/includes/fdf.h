/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcherchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/24 19:45:29 by hcherchi          #+#    #+#             */
/*   Updated: 2016/01/25 18:05:20 by hcherchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
#define FDF_H

#include "libft.h"
#include "mlx.h"
#include <math.h>
#include <fcntl.h>

typedef struct	s_tool
{
	void	*mlx_ptr;
	void	*mlx_win;
	int		nbline;
	int		nbcol;
}				t_tool;

typedef	struct	s_point
{
	int		x;
	int		y;
	int		z;
}				t_point;

int		check_error(char **split);
int		fill_tools(t_tool *tools, char *filename);
void	draw_line(t_point *a, t_point *b, t_tool *tools);
int		tablen(char **tab);
#endif
