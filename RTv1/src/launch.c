#include <rtv1.h>
#include <stdio.h>

void    launch(char *scene, t_tool *tools)
{
    parser(open(scene, O_RDONLY), tools);
    init_param(tools);
    init_cams(tools);
    run_through(tools);
}

void	init_param(t_tool *t)
{
    t->rt->indent = 0.001;
    t->rt->w = t->rt->x_res * t->rt->indent;
    t->rt->h = t->rt->y_res * t->rt->indent;
    t->rt->dist = t->rt->w / (2 * tan((60 / 2) * (M_PI / 180)));
    t->rt->mlx_win = mlx_new_window(t->mlx_ptr, t->rt->x_res, t->rt->y_res, "RTv1");
    t->rt->image = malloc(sizeof(t_image));
    t->rt->image->mlx_img = mlx_new_image(t->mlx_ptr, t->rt->x_res, t->rt->y_res);
    t->rt->image->data = mlx_get_data_addr(t->rt->image->mlx_img, &t->rt->image->bpp,
                                       &t->rt->image->size_line, &t->rt->image->endian);
    t->rt->image->texture = NULL;
    t->rt->image->screen = NULL;
    t->rt->sky = NULL; // SKY A RAJOUTER A PARTIR DU PARSER
}

void    init_cams(t_tool *t)
{
    t->rt->middlecams = malloc(sizeof(t_cam*) * 6);
    t->rt->middlecams[0] = new_cam(vectornew(0, 0, -5), vectornew(0, 0, 1), t, 0);
    t->rt->middlecams[1] = new_cam(vectornew(-5, 0, 0), vectornew(1, 0, 0), t, 1);
    t->rt->middlecams[2] = new_cam(vectornew(5, 0, 0), vectornew(-1, 0, 0), t, 2);
    t->rt->middlecams[3] = new_cam(vectornew(0, 0, 5), vectornew(0, 0, -1), t, 3);
    t->rt->middlecams[4] = new_cam(vectornew(0, 5, 0), vectornew(0, -1, 0), t, 4);
    if (t->rt->pos && t->rt->vect)
        t->rt->middlecams[5] = new_cam(t->rt->pos, t->rt->vect, t, 5);
    else
        t->rt->middlecams[5] = new_cam(vectornew(0, 2.5, -5), vectornew(0, -1, 2), t, 5);
    t->rt->upcams = malloc(sizeof(t_cam*) * 5);
    t->rt->upcams[0] = new_cam(vectornew(0, 2.5, -5), vectornew(0, -1, 2), t, 0);
    t->rt->upcams[1] = new_cam(vectornew(-5, 2.5, 0), vectornew(2, -1, 0), t, 1);
    t->rt->upcams[2] = new_cam(vectornew(5, 2.5, 0), vectornew(-2, -1, 0), t, 2);
    t->rt->upcams[3] = new_cam(vectornew(0, 2.5, 5), vectornew(0, -1, -2), t, 3);
    t->rt->upcams[4] = new_cam(vectornew(0, 10, 0), vectornew(0, -1, 0), t, 4);
    t->rt->cam = t->rt->middlecams[5];
}


// A REVOIR PAS DE STDIO
void put_image_to_file(t_tool *t)
{
    FILE *f;
    
    f = fopen("save.jpg", "w");
    fprintf(f, "P6 %d %d %d\n",t->rt->x_res, t->rt->y_res, 255);
    fwrite(t->rt->image->screen, 3, t->rt->x_res * t->rt->y_res, f);
    fclose(f);
}

void run_through(t_tool *t)
{
    int x;
    int y;
    
    y = 0;
    while (y < t->rt->y_res)
    {
        x = 0;
        while (x < t->rt->x_res)
        {
            draw(t, x, y);
            x += 1;
        }
        y += 1;
    }
    if (t->rt->screenshot == 0)
        mlx_put_image_to_window(t->mlx_ptr, t->rt->mlx_win, t->rt->image->mlx_img, 0, 0);
    else
        put_image_to_file(t);
    mlx_string_put(t->mlx_ptr, t->rt->mlx_win, 50, 25, 0x009933FF, "2 - 4 - 5 - 6 - 8 and arrows to change CAMERA");
    mlx_string_put(t->mlx_ptr, t->rt->mlx_win, 50, 50, 0x009933FF, "P : SCREENSHOT");
    mlx_string_put(t->mlx_ptr, t->rt->mlx_win, 50, 75, 0x009933FF, "Press 'DELETE' to go back to the menu");
    mlx_key_hook(t->rt->mlx_win, rt_event, t);
}

t_cam *new_cam(t_pos *pos, t_pos *vect, t_tool *t, int nb)
{
    t_cam *cam;
    
    vectornorm(vect);
    cam = malloc(sizeof(t_cam));
    cam->vect = vect;
    cam->pos = pos;
    cam->r_vect = malloc(sizeof(t_pos));
    cam->r_vect->x = cam->vect->z;
    cam->r_vect->y = 0;
    cam->r_vect->z = - 1 * cam->vect->x;
    if (cam->vect->x == 0 && cam->vect->z == 0)
        cam->r_vect->x = 1;
    cam->h_vect = malloc(sizeof(t_pos));
    cam->h_vect->x = -cam->r_vect->y * cam->vect->z + cam->r_vect->z * cam->vect->y;
    cam->h_vect->y = -cam->r_vect->z * cam->vect->x + cam->r_vect->x * cam->vect->z;
    cam->h_vect->z = -cam->r_vect->x * cam->vect->y + cam->r_vect->y * cam->vect->x;
    vectornorm(cam->r_vect);
    vectornorm(cam->h_vect);
    cam->upleft = malloc(sizeof(t_pos));
    cam->upleft->x = cam->pos->x + cam->vect->x * t->rt->dist + cam->h_vect->x * (t->rt->h / 2) - cam->r_vect->x * (t->rt->w / 2);
    cam->upleft->y = cam->pos->y + cam->vect->y * t->rt->dist+ cam->h_vect->y * (t->rt->h / 2) - cam->r_vect->y * (t->rt->w / 2);
    cam->upleft->z = cam->pos->z + cam->vect->z * t->rt->dist+ cam->h_vect->z * (t->rt->h / 2) - cam->r_vect->z * (t->rt->w / 2);
    cam->nb = nb;
    return (cam);
}
