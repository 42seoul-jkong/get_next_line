/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkong <jkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 13:44:21 by jkong             #+#    #+#             */
/*   Updated: 2022/03/19 18:08:43 by jkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_pair	*get_record(t_pair *map[BUCKET_SIZE], int fd)
{
	t_pair	*latest;
	t_pair	*result;

	latest = NULL;
	result = map[fd % BUCKET_SIZE];
	while (result)
	{
		if (result->fd == fd)
		{
			if (latest)
				latest->next = result->next;
			result->next = NULL;
			return (result);
		}
		latest = result;
		result = result->next;
	}
	result = malloc(sizeof(t_pair));
	if (result)
	{
		result->fd = fd;
		result->head = NULL;
		result->next = NULL;
	}
	return (result);
}

static void	put_record(t_pair *map[BUCKET_SIZE], int fd, t_pair *pair)
{
	t_pair			**head_ptr;
	t_string_chain	*chain;

	if (!pair->head || fd < 0)
	{
		while (pair->head)
		{
			chain = pair->head->next;
			free(pair->head);
			pair->head = chain;
		}
		free(pair);
		return ;
	}
	head_ptr = &map[fd % BUCKET_SIZE];
	pair->next = *head_ptr;
	*head_ptr = pair;
}

static ssize_t	prepare_line(t_pair *pair, int force)
{
	t_string_chain	*chain;
	ssize_t			index;
	ssize_t			length;

	index = -1;
	length = 0;
	chain = pair->head;
	while (chain)
	{
		index = ft_memchri(&chain->buf[chain->offset], '\n', chain->size);
		if (index < 0)
			length += chain->size - chain->offset;
		else
		{
			length += (index + 1) - chain->offset;
			break ;
		}
		chain = chain->next;
	}
	if (index < 0 && !force)
		return (-1);
	return (length);
}

static char	*make_line(int *fd, t_pair *pair, int force)
{
	const ssize_t	length = prepare_line(pair, force);
	t_string_chain	*next;
	char			*result;
	ssize_t			i;

	if (length < 0)
		return (NULL);
	result = malloc(length + 1);
	if (result)
	{
		i = 0;
		while (pair->head && i < length)
		{
			if (!consume_chain(pair->head, result, &i, length))
				break ;
			next = pair->head->next;
			free(pair->head);
			pair->head = next;
		}
	}
	else
		*fd = -1;
	return (result);
}

char	*get_next_line(int fd)
{
	static t_pair	*map[BUCKET_SIZE];
	t_pair *const	pair = get_record(map, fd);
	t_string_chain	*new_chain;
	char			*result;

	if (!pair)
		return (NULL);
	result = make_line(&fd, pair, 0);
	while (!result && !(fd < 0))
	{
		new_chain = malloc(sizeof(t_string_chain));
		if (new_chain)
		{
			new_chain->size = read(fd, new_chain->buf, sizeof(new_chain->buf));
			if (link_chain(&pair->head, new_chain))
				result = make_line(&fd, pair, new_chain->size == 0);
			else
				fd = -1;
		}
		else
			fd = -1;
	}
	put_record(map, fd, pair);
	return (result);
}
