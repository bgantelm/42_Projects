#include <RTv1.h>
#include <stdio.h>

t_color find_color(t_tool *t, float k, t_color objcolor, t_color lightcolor, float kdist)
{
    objcolor.r = objcolor.r * t->LumAmb + lightcolor.r * k * 0.4 * kdist;
    objcolor.g = objcolor.g * t->LumAmb + lightcolor.g * k * 0.4 * kdist;
    objcolor.b = objcolor.b * t->LumAmb + lightcolor.b * k * 0.4 * kdist;
    
    if (objcolor.r < 0)
        objcolor.r = 0;
    if (objcolor.b < 0)
        objcolor.b = 0;
    if (objcolor.g < 0)
        objcolor.g = 0;
    if (objcolor.r > 255)
        objcolor.r = 255;
    if (objcolor.b > 255)
        objcolor.b = 255;
    if (objcolor.g > 255)
        objcolor.g = 255;
    return (objcolor);
}

void	pixel_put_to_image(t_tool *t, int x, int y, t_color color)
{
    t->image.data = mlx_get_data_addr(t->mlx_img, &t->image.bpp, &t->image.size_line, &t->image.endian);
    t->image.data[x * t->image.bpp / 8 + y * t->image.size_line] = (unsigned char)color.b;
    t->image.data[x * t->image.bpp / 8 + 1 + y * t->image.size_line] = (unsigned char)color.g;
    t->image.data[x * t->image.bpp / 8 + 2 + y * t->image.size_line] = (unsigned char)color.r;
}

t_ray  get_ray(t_tool *t, float x, float y)
{
    t_ray  ray;
    t_pos   B;
    
    // position de depart du rayon (Camera)
    ray.O.x = t->cam.pos.x;
    ray.O.y = t->cam.pos.y;
    ray.O.z = t->cam.pos.z;
    
    // calcul de la position d'arrivee du rayon sur le viewplane
    B.x = t->cam.upleft.x + t->cam.r_vect.x * t->cam.x_indent * x - t->cam.h_vect.x * t->cam.y_indent * y;
    B.y = t->cam.upleft.y + t->cam.r_vect.y * t->cam.x_indent * x - t->cam.h_vect.y * t->cam.y_indent * y;
    B.z = t->cam.upleft.z + t->cam.r_vect.z * t->cam.x_indent * x - t->cam.h_vect.z * t->cam.y_indent * y;
    
    // calcul du vecteur directeur du rayon
    ray.D = vectorSub(&B, &ray.O);
    vectorNorm(&ray.D); // norme pour avoir taille de vecteur = 1 (vecteur unitaire)
    return (ray);
}


void    draw(t_tool *t, float x, float y)
{
    t_ray  ray;
    t_light light;
    t_color final_color;
    t_object *l_objects;
    t_object *cyl;
    t_object *plan;
    t_object *curObject;
    t_object *curObject2;
    
    float k;
    
    cyl = malloc(sizeof(t_object));
    
    plan = malloc(sizeof(t_object));
    
    // raypon de la sphere
    cyl->rad = 1;
    // centre de la sphere
    cyl->O.x = 0;
    cyl->O.y = 0;
    cyl->O.z = 10;
//    cyl->D.x = 0;
//    cyl->D.y = 1;
//    cyl->D.z = 0;
    
    // couleur de la sphere
    cyl->color.r = 0;
    cyl->color.g = 100;
    cyl->color.b = 100;
    
    cyl->next = plan;
    cyl->type = SPHERE;
    
    plan->a = 0;
    plan->b = 1;
    plan->c = 0;
    plan->d = 1;
    
    // couleur de la sphere
    plan->color.r = 200;
    plan->color.g = 0;
    plan->color.b = 0;
    
    plan->next = NULL;
    plan->type = PLAN;
    
    l_objects = cyl;
    
    // position de la lumiere
    light.O.x = 1;
    light.O.y = 1;
    light.O.z = 0;
    
    // couleur de la lumiere (blanche ici)
    light.color.r = 255;
    light.color.g = 255;
    light.color.b = 255;
    
    // Pour obtenir le rayon avec position de depart et vecteur directeur unitaire
    ray = get_ray(t, x, y);
    
    // si il y a une intersection avec la sphere
    if ((curObject = intersection(l_objects, ray)))
    {
        t_ray   impact;
        t_ray   lightray;
        
        // on calcule les coordonnes du point touché avec Depart rayon + vecteur unitaire rayon * coef;
        impact.O.x = ray.O.x + ray.D.x * curObject->dist;
        impact.O.y = ray.O.y + ray.D.y * curObject->dist;
        impact.O.z = ray.O.z + ray.D.z * curObject->dist;
        
        // depart du rayon de lumiere
        lightray.O.x = light.O.x;
        lightray.O.y = light.O.y;
        lightray.O.z = light.O.z;
        
        // calcul du vecteur directeur unitaire du rayon de lumiere
        lightray.D = vectorSub(&impact.O, &lightray.O);
        vectorNorm(&lightray.D); // norme pour avoir une distance de 1
        
        if ((curObject2 = intersection(l_objects, lightray)) && curObject2->type == curObject->type)
        {
        // calcul du vecteur normal a la sphere au point d'impact
        find_normal(&impact, curObject);
        vectorNorm(&impact.D); // norme pour avoir une distance de 1
        
        // calcul de l'angle forme par le rayon de lumiere et la normale qui correspond a un coefficient (0 < k < 1) de luminosité de la sphere
        k = - (vectorDot(&lightray.D, &impact.D));
            
            float kdist;
        kdist = (60 - curObject->dist) / 60;
        
        // calcul de la couleur finale grace a la couleur de base de l'objet, de la lumiere, au coefficient de luminosité et au coefficient de lumiere d'ambiance (fixe)
        final_color = find_color(t, k, curObject->color, light.color, kdist);
        }
        else
        {
            final_color.r = 0;
            final_color.g = 0;
            final_color.b = 0;
        }
        pixel_put_to_image(t, x, y, final_color);
    }
}

