/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkong <jkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 13:44:41 by jkong             #+#    #+#             */
/*   Updated: 2022/03/19 18:40:30 by jkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		((unsigned char *)b)[i++] = (unsigned char)c;
	return (b);
}

ssize_t	ft_memchri(const void *s, int c, size_t n)
{
	ssize_t	i;

	i = 0;
	while (i < n)
	{
		if (((const unsigned char *)s)[i] == (unsigned char)c)
			return (i);
		i++;
	}
	return (-1);
}

int	link_chain(t_string_chain **head, t_string_chain *new_chain)
{
	t_string_chain	*chain;

	chain = *head;
	if (chain)
	{
		while (chain->next)
			chain = chain->next;
		chain->next = new_chain;
	}
	else
		*head = new_chain;
	new_chain->offset = 0;
	new_chain->next = NULL;
	return (!(new_chain->size < 0));
}

int	consume_chain(t_string_chain *chain, char *s, ssize_t *i, ssize_t n)
{
	while (chain->offset < chain->size && *i < n)
		s[(*i)++] = chain->buf[chain->offset++];
	if (chain->offset < chain->size)
		return (0);
	return (1);
}
