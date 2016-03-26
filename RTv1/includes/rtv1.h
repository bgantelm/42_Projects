/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcherchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/23 14:56:14 by hcherchi          #+#    #+#             */
/*   Updated: 2016/03/26 11:46:17 by hcherchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# include <libft.h>
# define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
# define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
# define SPHERE 0
# define CYL 1
# define CONE 2
# define PLAN 3
# define E 0.0001

typedef struct		s_k
{
	double			spec;
	double			diff;
	double			dist;
}					t_k;

typedef struct		s_equation
{
	double			a;
	double			b;
	double			c;
	double			discr;
	double			t1;
	double			t0;
}					t_equation;

typedef struct		s_color
{
	int				r;
	int				g;
	int				b;
}					t_color;

typedef struct      s_colors
{
    t_color         *reflect;
    t_color         *base;
    t_color         *refract;
}                   t_colors;

typedef struct		s_pos
{
	double			x;
	double			y;
	double			z;
}					t_pos;

typedef struct		s_object
{
	int				type;
	double			dist;
	t_color			*color;
	t_pos			*o;
	t_pos			*d;
	double			rad;
	double			h;
	double			shiny;
    double          mirror;
    double          transp;
    double          refract;
	struct s_object	*next;
}					t_object;

typedef struct		s_ray
{
	t_pos			*o;
	t_pos			*d;
}					t_ray;

typedef struct		s_light
{
	t_pos			*o;
	t_color			*color;
	double			dist;
	double			lumdiff;
	struct s_light	*next;
}					t_light;

typedef struct		s_cam
{
	t_pos			*pos;
	t_pos			*h_vect;
	t_pos			*r_vect;
	t_pos			*vect;
	double			dist;
	double			w;
	double			h;
	t_pos			*upleft;
	int				x_res;
	int				y_res;
	double			indent;
}					t_cam;

typedef struct		s_image
{
	int				endian;
	int				size_line;
	int				bpp;
	char			*data;
}					t_image;

typedef struct		s_tool
{
	void			*mlx_ptr;
	void			*mlx_win;
	void			*mlx_img;
	t_object		*l_objects;
	t_light			*l_lights;
	t_image			*image;
	t_cam			*cam;
	double			lumamb;
    int             depth;
}					t_tool;

double				minimum(t_object *l_objects);
void				fill_dist(t_object *l_objects, t_ray *ray);
t_object			*intersection(t_object *l_objects, t_ray *ray);
double				intersection_plan(t_object *plan, t_ray *ray);
double				intersection_sphere(t_object *sphere, t_ray *ray);
double				intersection_cone(t_object *cone, t_ray *ray);
double				intersection_cyl(t_object *cyl, t_ray *ray);

void				draw(t_tool *t, int x, int y);
t_ray				*get_ray(t_tool *t, double x, double y);
t_color				*get_color(t_ray *ray, t_tool *t);
t_color     *get_final_color(t_colors   *colors, t_object *object);
t_ray				*get_normal(t_object *object, t_ray *ray);
void				get_cyl_normal(t_ray *impact, t_object *object);
t_ray				*get_lightray(t_ray *impact, t_light *light);
void				illuminate(t_tool *t, t_object *object, t_ray *impact,
					t_color *final_color);
double				get_kspec(t_ray *lightray, t_ray *impact, double kdist,
					double intens);
double				get_kdiff(t_ray *lightray, t_ray *impact, double kdist,
					double intens);
void				update_color(double k, t_color *lightcolor,
					t_color *final_color, t_color *objcolor);
void				pixel_put_to_image(t_tool *t, int x, int y,
					t_color *color);

void				vectornorm(t_pos *v);
t_pos				*vectorsub(t_pos *v1, t_pos *v2);
double				vectordot(t_pos *v1, t_pos *v2);
t_pos				*vectorscale(double c, t_pos *v);
t_pos				*vectoradd(t_pos *v1, t_pos *v2);
t_pos				*vectorcopy(t_pos *v1);
t_pos				*vectornew(double x, double y, double z);
t_pos				*rotation(t_pos *axe, t_pos *vect);

void				parser(int fd, t_tool *tools);
void				parse_light(t_tool *tools, int fd);
void				parse_light2(t_light *light, char **split, char *line);
void				parse_object(t_tool *tools, int fd);
void				parse_object2(t_object *object, char **split, char *line);
void				parse_camera(t_tool *tools, int fd);
void				parse_camera2(t_tool *tools, char **split, char *line);
void				add_object(t_object **l_objects, t_object *new);
void				add_light(t_light **l_lights, t_light *new);
int					object_type(char **split);
t_pos				*fill_pos(char **split);
t_color				*fill_color(char **split);

t_color				*new_color();
t_colors            *new_colors();
void				init_color(t_tool *t, t_color *objcolor, t_color *f_color);
void				normalize_color(t_color *final_color);
void				add_color(t_color *color1, t_color *color2);
void				div_color(t_color *color, double n);

void				ft_error(int i);
void				init_camera(t_tool *tools);
void				init_light(t_light *light);
void				init_object(t_object *object);
void				update_cam(t_tool *t);
void				run_through(t_tool *t);
void				init_param(t_tool *t);
int					event(int keycode, t_tool *t);
void				change_pos(int keycode, t_tool *t);

void				clean_ray(t_ray *ray);
void				clean_obj(t_object *l_objects);
void				clean_lights(t_light *l_lights);
void				clean_cam(t_cam *cam);
void				clean_tools(t_tool *tools);

#endif
