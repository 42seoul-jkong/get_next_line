/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkong <jkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 13:44:31 by jkong             #+#    #+#             */
/*   Updated: 2022/03/19 18:06:01 by jkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

# define BUCKET_SIZE 5

typedef struct s_pair			t_pair;

typedef struct s_string_chain	t_string_chain;

struct s_pair
{
	int				fd;
	t_string_chain	*head;
	t_pair			*next;
};

struct s_string_chain
{
	char			buf[BUFFER_SIZE];
	ssize_t			size;
	ssize_t			offset;
	t_string_chain	*next;
};

/*
** A function which returns a line read from a
** file descriptor
**
** @param #1 File descriptor to read from
**
** @return Read line: correct behavior
** @return NULL: nothing else to read or an error occurred
** @return
** @return The returned line should include the ’\n’, except if you have reached
** @return the end of file and there is no ’\n’.
*/
char	*get_next_line(int fd);

ssize_t	ft_memchri(const void *s, int c, size_t n);
int		link_chain(t_string_chain **head, t_string_chain *new_chain);
int		consume_chain(t_string_chain *chain, char *s, ssize_t *i, ssize_t n);

#endif
