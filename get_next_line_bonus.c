/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yahy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 14:45:31 by mel-yahy          #+#    #+#             */
/*   Updated: 2025/10/17 14:45:31 by mel-yahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*extract_line(char **left_over, char *nl_pos)
{
	char	*line;
	char	*tmp;

	if (*left_over == NULL || (*left_over)[0] == '\0')
		return (NULL);
	if (nl_pos != NULL)
	{
		line = ft_substr(*left_over, 0, nl_pos - *left_over + 1);
		tmp = *left_over;
		*left_over = ft_strdup(nl_pos + 1);
		if ((*left_over)[0] == '\0')
		{
			free(*left_over);
			*left_over = NULL;
		}
		free(tmp);
	}
	else
	{
		line = ft_strdup(*left_over);
		free(*left_over);
		*left_over = NULL;
	}
	return (line);
}

static int	read_into_buf(int fd, char *buf, char **left_over)
{
	int	bytes;

	bytes = read(fd, buf, BUFFER_SIZE);
	if (bytes < 0)
	{
		free(*left_over);
		*left_over = NULL;
		return (-1);
	}
	buf[bytes] = '\0';
	return (bytes);
}

char	*get_next_line(int fd)
{
	static char	*left_overs[MAX_FDS] = {NULL};
	char		*buf;
	char		*nl_pos;
	char		*tmp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf == NULL)
		return (NULL);
	nl_pos = ft_strchr(left_overs[fd], '\n');
	while (nl_pos == NULL && read_into_buf(fd, buf, &left_overs[fd]) > 0)
	{
		if (left_overs[fd] == NULL)
			left_overs[fd] = ft_strdup(buf);
		else
		{
			tmp = left_overs[fd];
			left_overs[fd] = ft_strjoin(left_overs[fd], buf);
			free(tmp);
		}
		nl_pos = ft_strchr(left_overs[fd], '\n');
	}
	free(buf);
	return (extract_line(&left_overs[fd], nl_pos));
}
