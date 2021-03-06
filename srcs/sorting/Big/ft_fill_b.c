/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlormois <mlormois@studient.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 18:40:57 by mlormois          #+#    #+#             */
/*   Updated: 2021/12/25 23:15:28 by mlormois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pushswap.h"

static int	ft_stack_ison(t_stack *pls, int value)
{
	if (!pls)
		return (EXIT_FAILURE);
	while (pls)
	{
		if (pls->value == value)
			return (EXIT_FAILURE);
		pls = pls->next;
	}
	return (EXIT_SUCCESS);
}

static void	ft_more(t_inst **inst, t_stack **sa, t_stack **sb, t_stack *pls)
{
	int	med;
	int	last;
	int	flag;

	flag = 0;
	last = ft_last_value(*sa);
	med = ft_search_median(*sa);
	while (*sa && !flag)
	{
		if ((*sa)->value == last)
			flag = 1;
		if ((*sa)->value > med && !ft_stack_ison(pls, (*sa)->value))
			_px(inst, sa, sb, B);
		else
			_rx(inst, sa, sb, A);
	}
}

static void	ft_rest(t_inst **inst, t_stack **sa, t_stack **sb, t_stack *pls)
{
	int	last;
	int	flag;

	flag = 0;
	last = ft_last_value(*sa);
	while (*sa && !flag)
	{
		if ((*sa)->value == last)
			flag = 1;
		if (!ft_stack_ison(pls, (*sa)->value))
			_px(inst, sa, sb, B);
		else
			_rx(inst, sa, sb, A);
	}
}

static void	ft_plssc(t_stack **plssc, t_stack *sa)
{
	int		min;
	t_stack	*super_stack;
	t_stack	*ptr_start;

	min = ft_min_value(sa);
	super_stack = ft_stackdup(sa);
	ft_stack_addback(&super_stack, ft_stackdup(sa));
	ptr_start = super_stack;
	while (super_stack->value != min)
		super_stack = super_stack->next;
	ft_stack_addfront(plssc, ft_stack_create(super_stack->value));
	super_stack = super_stack->next;
	while (super_stack->value != min)
	{
		if (super_stack->value > (*plssc)->value)
			ft_stack_addfront(plssc, ft_stack_create(super_stack->value));
		super_stack = super_stack->next;
	}
	ft_stack_clear(&ptr_start);
}

t_inst	*ft_fill_b(t_stack **sa, t_stack **sb)
{
	t_inst	*inst;
	t_stack	*plssc;

	inst = NULL;
	plssc = NULL;
	ft_plssc(&plssc, *sa);
	ft_more(&inst, sa, sb, plssc);
	ft_rest(&inst, sa, sb, plssc);
	ft_stack_clear(&plssc);
	return (inst);
}
