/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebatchas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/15 18:44:01 by ebatchas          #+#    #+#             */
/*   Updated: 2019/10/13 15:15:20 by ebatchas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/rt.h"

t_object	*ft_mesh_new(void)
{
	t_object	*new;

	if ((new = (t_object *)ft_memalloc(sizeof(*new))))
	{
		new->type = MESH;
		new->name = "MESHE";
		new->pos = (t_vec3){0.0, 0.0, 0.0};
		new->translate = (t_vec3){0.0, 0.0, 0.0};
		new->rotate = (t_vec3){0.0, 0.0, 0.0};
		new->scale = (t_vec3){1.0f, 1.0f, 1.0f};
		new->e.mesh = ft_default_mesh();
		new->material = ft_default_material();
		new->next = NULL;
	}
	else
		ft_print_error("malloc error");
	return (new);
}

t_vec3		ft_normal_mesh(t_triangle *tr)
{
	t_vec3	n;

	n = ft_vec3_cross(ft_vec3_sub(tr->b, tr->a), ft_vec3_sub(tr->c, tr->a));
	return (ft_vec3_normalized(n));
}

int			ft_mesh_compute(t_object *p, t_intersect *in)
{
	t_ray	r;
	int		i;
	int		index;

	index = -1;
	i = -1;
	r = in->ray;
	while (++i < p->e.mesh.size)
		if (ft_mesh_intersect(&p->e.mesh.triangles[i], &r, &in->t))
			index = i;
	if (!(index > -1 && index < i))
		return (0);
	in->current = p;
	in->p = ft_vec3_sum(r.start, ft_vec3_kmult(in->t, r.dir));
	in->n = ft_normal_mesh(&p->e.mesh.triangles[index]);
	in->p = ft_translate_vec3(in->p, p->translate, 0);
	in->p = ft_rotate_vec3(in->p, p->rotate, 0);
	in->p = ft_scale_vec3(in->p, p->scale, 0);
	in->n = ft_rotate_vec3(in->n, p->rotate, 0);
	in->n = ft_scale_vec3(in->n, p->scale, -1);
	in->n = ft_vec3_normalized(in->n);
	return (1);
}

int			ft_mesh_intersect(t_triangle *tr, t_ray *r, float *t)
{
	t_aux_triangles a;

	a.edge1 = ft_vec3_sub(tr->b, tr->a);
	a.edge2 = ft_vec3_sub(tr->c, tr->a);
	a.h = ft_vec3_cross(r->dir, a.edge2);
	a.det = ft_vec3_dot(a.edge1, a.h);
	if (fabs(a.det) >= 1.0e-6)
	{
		a.invdet = 1.0 / a.det;
		a.s = ft_vec3_sub(r->start, tr->a);
		a.u = ft_vec3_dot(a.s, a.h) * a.invdet;
		if (a.u < 1.0e-6 || a.u > 1.0001)
			return (0);
		a.q = ft_vec3_cross(a.s, a.edge1);
		a.v = ft_vec3_dot(r->dir, a.q) * a.invdet;
		if ((a.v < 1.0e-6) || ((a.u + a.v) > 1.0001f))
			return (0);
		a.t1 = ft_vec3_dot(a.edge2, a.q) * a.invdet;
		if (a.t1 < *t && a.t1 > 1.0e-6f)
		{
			*t = a.t1;
			return ((a.det > 0.001f) ? 2 : 1);
		}
	}
	return (0);
}
