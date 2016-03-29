#include <rtv1.h>

t_ray	*get_lightray(t_ray *impact, t_light *light)
{
    t_ray	*lightray;
    float   dist;
    
    lightray = malloc(sizeof(t_ray));
    if (light->type == SUN || light->type == SPOT)
    {
        lightray->o = vectorcopy(light->o);
        lightray->d = vectorsub(impact->o, lightray->o);
    }
    else if (light->type == LIGHTPLAN)
    {
        lightray->d = vectorcopy(light->d);
        if ((dist = intersection_plan(light->d, light->h, impact->o, vectorscale(-1, lightray->d))) != -1)
        {
            lightray->o = vectoradd(impact->o, vectorscale(dist, vectorscale(-1, lightray->d)));
        }
        else
            return (NULL);
    }
    vectornorm(lightray->d);
    if (light->type == SPOT && acos(vectordot(lightray->d, light->d)) > light->angle)
        return (NULL);
    return (lightray);
}

t_ray		*get_ray(t_tool *t, double x, double y)
{
    t_ray	*ray;
    t_pos	*tmp;
    
    ray = malloc(sizeof(t_ray));
    ray->o = malloc(sizeof(t_pos));
    tmp = malloc(sizeof(t_pos));
    ray->o->x = t->cam->pos->x;
    ray->o->y = t->cam->pos->y;
    ray->o->z = t->cam->pos->z;
    tmp->x = t->cam->upleft->x + t->cam->r_vect->x * t->cam->indent * x
    - t->cam->h_vect->x * t->cam->indent * y;
    tmp->y = t->cam->upleft->y + t->cam->r_vect->y * t->cam->indent * x
    - t->cam->h_vect->y * t->cam->indent * y;
    tmp->z = t->cam->upleft->z + t->cam->r_vect->z * t->cam->indent * x
    - t->cam->h_vect->z * t->cam->indent * y;
    ray->d = vectorsub(tmp, ray->o);
    vectornorm(ray->d);
    return (ray);
}

t_ray       *get_reflectray(t_ray *ray, t_tool *t, t_ray *impact)
{
    t_ray *reflect;
    
    t->depth += 1;
    reflect = malloc(sizeof(t_ray));
    reflect->o = vectorcopy(impact->o);
    reflect->d = vectoradd(vectorscale(-2 * vectordot(ray->d, impact->d), impact->d), ray->d);
    return (reflect);
}

t_ray       *get_refractray(t_ray *ray, t_ray *impact, t_object *object)
{
    t_ray   *refract;
    double   cos1;
    double   cos2;
    double   n;
    
    n = 1 / object->refract;
    cos1 = vectordot(impact->d, ray->d);
    cos2 = sqrtf(1 - pow(n, 2) * (1 - pow(cos1, 2)));
    refract = malloc(sizeof(t_ray));
    refract->o = vectorcopy(impact->o);
    if (cos1 > 0)
        refract->d = vectorsub(vectorscale(1 / object->refract, ray->d), vectorscale(n * cos1  - cos2, impact->d));
    else
        refract->d = vectorsub(vectorscale(1 / object->refract, ray->d), vectorscale(n * cos1  + cos2, impact->d));
    return (refract);
}