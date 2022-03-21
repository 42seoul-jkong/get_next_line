/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkong <jkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 13:44:41 by jkong             #+#    #+#             */
/*   Updated: 2022/03/21 21:42:43 by jkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_pair	*new_pair(int fd)
{
	t_pair	*result;

	result = malloc(sizeof(t_pair));
	if (!result)
		return (NULL);
	result->fd = fd;
	result->head = NULL;
	result->next = NULL;
	return (result);
}

t_chain	*new_chain(void)
{
	t_chain	*result;

	result = malloc(sizeof(t_chain));
	if (!result)
		return (NULL);
	result->offset = 0;
	result->next = NULL;
	return (result);
}

int	create_chain(t_chain **head, t_chain **elem, int fd)
{
	t_chain	*target;
	t_chain	*chain;

	*elem = new_chain();
	target = *elem;
	if (!target)
		return (0);
	target->size = read(fd, target->buf, sizeof(target->buf));
	chain = *head;
	if (chain)
	{
		while (chain->next)
			chain = chain->next;
		chain->next = target;
	}
	else
		*head = target;
	return (1);
}

ssize_t	findchr_chain(t_chain *chain, int c)
{
	const void *const	s = &chain->buf[chain->offset];
	const ssize_t		n = chain->size - chain->offset;
	ssize_t				i;

	i = 0;
	while (i < n)
	{
		if (((const unsigned char *)s)[i] == (unsigned char)c)
			return (i);
		i++;
	}
	return (-1);
}

int	consume_chain(t_chain *chain, char *s, ssize_t *i, ssize_t n)
{
	while (*i < n && chain->offset < chain->size)
		s[(*i)++] = chain->buf[chain->offset++];
	if (chain->offset < chain->size)
		return (0);
	return (1);
}
