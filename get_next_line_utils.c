/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkong <jkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 13:44:41 by jkong             #+#    #+#             */
/*   Updated: 2022/03/19 20:09:56 by jkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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

int	read_chain(t_string_chain **chain, int fd)
{
	t_string_chain	*result;

	*chain = malloc(sizeof(t_string_chain));
	if (!*chain)
		return (0);
	result = *chain;
	result->size = read(fd, result->buf, sizeof(result->buf));
	result->offset = 0;
	result->next = NULL;
	return (1);
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
	return (!(new_chain->size < 0));
}

ssize_t	findchr_chain(t_string_chain *chain, int c)
{
	const void		*s = &chain->buf[chain->offset];
	const ssize_t	n = chain->size - chain->offset;
	ssize_t			i;

	i = 0;
	while (i < n)
	{
		if (((const unsigned char *)s)[i] == (unsigned char)c)
			return (i);
		i++;
	}
	return (-1);
}

int	consume_chain(t_string_chain *chain, char *s, ssize_t *i, ssize_t n)
{
	while (*i < n && chain->offset < chain->size)
		s[(*i)++] = chain->buf[chain->offset++];
	if (chain->offset < chain->size)
		return (0);
	return (1);
}
