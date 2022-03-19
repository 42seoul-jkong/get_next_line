/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkong <jkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 13:44:21 by jkong             #+#    #+#             */
/*   Updated: 2022/03/19 20:10:40 by jkong            ###   ########.fr       */
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
			else
				map[fd % BUCKET_SIZE] = NULL;
			result->next = NULL;
			return (result);
		}
		latest = result;
		result = result->next;
	}
	result = new_pair(fd);
	return (result);
}

static void	put_record(t_pair *map[BUCKET_SIZE], int fd, t_pair *pair)
{
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
	pair->next = map[fd % BUCKET_SIZE];
	map[fd % BUCKET_SIZE] = pair;
}

static ssize_t	prepare_line(int *fd, t_pair *pair, int force)
{
	t_string_chain	*chain;
	ssize_t			index;
	ssize_t			length;

	index = -1;
	length = 0;
	chain = pair->head;
	while (chain)
	{
		index = findchr_chain(chain, '\n');
		if (index >= 0)
		{
			length += index + 1;
			break ;
		}
		length += chain->size - chain->offset;
		chain = chain->next;
	}
	if (!force && index < 0)
		return (-1);
	if (force && length == 0)
		*fd = -1;
	return (length);
}

static char	*make_line(int *fd, t_pair *pair, int force)
{
	const ssize_t	length = prepare_line(fd, pair, force);
	t_string_chain	*next;
	char			*result;
	ssize_t			i;

	if (length < 0 || *fd < 0)
		return (NULL);
	result = malloc((length + 1) * sizeof(char));
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
		result[i] = '\0';
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
		if (read_chain(&new_chain, fd) && link_chain(&pair->head, new_chain))
			result = make_line(&fd, pair, new_chain->size == 0);
		else
			fd = -1;
	}
	put_record(map, fd, pair);
	return (result);
}
