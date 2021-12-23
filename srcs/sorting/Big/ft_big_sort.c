/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_big_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlormois <mlormois@studient.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:29:32 by mlormois          #+#    #+#             */
/*   Updated: 2021/12/23 17:32:15 by mlormois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pushswap.h"

static void	ft_remplace_counter(int **ct, int *nc)
{
	(*ct)[RA] = (nc[RA] - nc[RR]) * (nc[MET] == 0) + nc[RA] * (nc[MET] != 0);
	(*ct)[RB] = (nc[RB] - nc[RR]) * (nc[MET] == 0) + nc[RB] * (nc[MET] != 0);
	(*ct)[RR] = nc[RR];
	(*ct)[RRA] = (nc[RRA] - nc[RRR]) * (nc[MET] == 2)
		+ nc[RRA] * (nc[MET] != 2);
	(*ct)[RRB] = (nc[RRB] - nc[RRR]) * (nc[MET] == 2)
		+ nc[RRB] * (nc[MET] != 2);
	(*ct)[RRR] = nc[RRR];
	(*ct)[MET] = nc[MET];
	(*ct)[NI] = nc[NI];
}

/*	Calcule du nombre d'instruction pour une valeur SB[INDEX].
**	TIPS : Brancheless pour eviter les conditions.
*/

static int	ft_search_push(t_stack *sa, t_stack *sb, int index, int **ct)
{
	int		nc[8];
	int		tot[4];

	nc[RB] = index;
	nc[RRB] = ft_stack_size(sb) - index;
	nc[RA] = ft_place_value(sa, ft_take_value(sb, index));
	nc[RRA] = ft_stack_size(sa) - nc[RA];
	nc[RR] = nc[RB] * (nc[RB] < nc[RA]) + nc[RA] * (nc[RB] >= nc[RA]);
	nc[RRR] = nc[RRB] * (nc[RRB] < nc[RRA]) + nc[RRA] * (nc[RRB] >= nc[RRA]);
	tot[0] = (nc[RA] - nc[RR]) + (nc[RB] - nc[RR]) + nc[RR];
	tot[1] = (nc[RA] + nc[RRB]);
	tot[2] = (nc[RRA] - nc[RRR]) + (nc[RRB] - nc[RRR]) + nc[RRR];
	tot[3] = (nc[RRA] + nc[RB]);
	nc[MET] = 1 * (tot[1] < tot[0] && tot[1] < tot[2] && tot[1] < tot[3])
		+ 2 * (tot[2] < tot[0] && tot[2] < tot[1] && tot[2] < tot[3])
		+ 3 * (tot[3] < tot[0] && tot[3] < tot[1] && tot[3] < tot[2]) + 0;
	nc[NI] = tot[0] * (nc[MET] == 0) + tot[1] * (nc[MET] == 1)
		+ tot[2] * (nc[MET] == 2) + tot[3] * (nc[MET] == 3);
	if (nc[NI] < (*ct)[NI])
		ft_remplace_counter(ct, nc);
	return ((*ct)[NI]);
}

/*	Une Simple Initialisation de mon tableau de compteur.
**	Je me sert de mon enum pour savoir a quoi correspond chaque int de mon tab.
*/

static int ft_set_counter(int **ct)
{
	(*ct) = (int *)malloc(sizeof(int) * 8);
	if (!(*ct))
		return (EXIT_FAILURE);
	(*ct)[RA] = 0;
	(*ct)[RB] = 0;
	(*ct)[RR] = 0;
	(*ct)[RRA] = 0;
	(*ct)[RRB] = 0;
	(*ct)[RRR] = 0;
	(*ct)[MET] = 0;
	(*ct)[NI] = 2147483647;
	return (EXIT_SUCCESS);
}

/*	Le Best Push:
**	pour chaque valeur de mon tableau, je calcule e nombre d'instruction
**  a faire pour la push a ca place dans A. Et je garde uniquement la
**	meilleur solution dans un tableau. 
**	TIPS: Je m'arrete de check les valeur quand mon nombre d'instruction
**  deviens egale a mon index dans la stack. ( le nombre de RB || RRB ).
*/

void ft_print_ct(int *ct)
{
	printf("RA = %d\n", ct[RA]);
	printf("RB = %d\n", ct[RB]);
	printf("RR = %d\n", ct[RR]);
	printf("RRA = %d\n", ct[RRA]);
	printf("RRB = %d\n", ct[RRB]);
	printf("RRR = %d\n", ct[RRR]);
	printf("METH = %d\n", ct[MET]);
	printf("NI = %d\n", ct[NI]);
}

static t_inst *ft_best_push(t_stack **sa, t_stack **sb, int len_sb)
{
	t_inst	*inst;
	int		nb_inst;
	int		*ct;
	int		i;

	i = 0;
	inst = NULL;
	nb_inst = 2147483647;
	if (ft_set_counter(&ct))
		return (NULL);
	while (i < nb_inst && i < len_sb)
	{
		nb_inst = ft_search_push(*sa, *sb, i, &ct);
		if (len_sb > 1)
			nb_inst = ft_search_push(*sa, *sb, len_sb - i - 1, &ct);
		i++;
	}
	inst = ft_make_inst(sa, sb, ct, ct[MET]);
	free(ct);
	return (inst);
}

/* 
** Big Sort mlormois: 4 ETAPE:
** 1. Je cherche la PLSSC (Plus Longue Sous Suites Croissante) de la stack.
**	  je vais garder un tableau d'int sur les index de cette suite.
** 2. Je push dans B tout les autres valeurs.
** 3. Best Push, de B a A, je cherche la valeur qui rentre le plus rapidement.
** 4. Replace, je replace ma stack avec la valeur MIN en premiere.
*/

void ft_plssc(t_stack **plssc, t_stack *sa, int size)
{
	(void)size;
	t_stack *tmp;

	int min = atoi(INT_MAX);
	int max = atoi(INT_MIN) * -1;


	tmp = sa;
	while (tmp)
	{
		if (tmp->value < min)
			min = tmp->value;
		tmp = tmp->next;
	}
	tmp = sa;
	while (tmp)
	{
		if (tmp->value > max)
			max = tmp->value;
		tmp = tmp->next;
	}
	ft_stack_addback(plssc, ft_stack_create(min));
	ft_stack_addback(plssc, ft_stack_create(max));
}

void	ft_big_sort(t_stack *sa, int size)
{
	t_inst	*inst;
	t_stack	*sb;
	t_stack	*plssc;

	size = 0;
	inst = NULL;
	sb = NULL;
	plssc = NULL;

	// printf("STEP 1\n");
	
	// JE CHERCHE LA PLSSC
	ft_plssc(&plssc, sa, size);

	// printf("STEP 2\n");

	// PUSH B
	ft_inst_addback(&inst, ft_fill_b(&sa, &sb, plssc));
	ft_stack_clear(&plssc);

	// ft_stack_print(sa);
	// ft_stack_print(sb);
	// ft_inst_print(STDOUT_FILENO, inst);

	// printf("STEP 3\n");

	while (sb != NULL)
		ft_inst_addback(&inst, ft_best_push(&sa, &sb, ft_stack_size(sb)));

	// printf("STEP 4\n");
	
	ft_inst_addback(&inst, ft_replace_stack(&sa));

	// printf("STEP 5\n");
	
	// ft_stack_print(sa);
	ft_inst_print(STDOUT_FILENO, inst);
	ft_stack_clear(&sa);
	ft_inst_clear(&inst);
}
