/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 16:47:55 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/05/28 20:13:05 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	ft_double_buffer(char **buffer);
static char	*ft_return_line(char **buffer, char **remainder);
static char	*ft_allocating(char **remainder, char **buffer);

char	*ft_allocating(char **remainder, char **buffer)
{
	if (*remainder)
	{
		*buffer = ft_calloc(ft_strlen(*remainder) + 1 + BUFFER_SIZE, 1);
		if (!(*buffer))
			return (NULL);
		ft_strlcpy(*buffer, *remainder, ft_strlen(*remainder) + 1);
		free(*remainder);
	}
	else
	{
		*buffer = ft_calloc(BUFFER_SIZE + 1, 1);
		if (!(*buffer))
			return (NULL);
	}
	return (*buffer);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	static char	*remainder;
	int			byte_read;

	byte_read = 1;
	buffer = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (ft_allocating(&remainder, &buffer) == NULL)
		return (ft_free(&buffer, &remainder));
	while (byte_read != 0)
	{
		byte_read = read(fd, &buffer[ft_strlen(buffer)], BUFFER_SIZE);
		if (byte_read == -1)
			return (free(buffer), NULL);
		remainder = ft_strchr(buffer, '\n');
		if (remainder)
			return (ft_return_line(&buffer, &remainder));
		if (ft_double_buffer(&buffer) == 0)
			return (ft_free(&buffer, &remainder));
	}
	if (ft_strlen(buffer) == 0)
		return (ft_free(&buffer, &remainder));
	return (buffer);
}

int	ft_double_buffer(char **buffer)
{
	int		buffer_len;
	char	*temp;

	buffer_len = ft_strlen(*buffer);
	temp = malloc(buffer_len + 1);
	if (!temp)
		return (0);
	ft_strlcpy(temp, *buffer, buffer_len + 1);
	free(*buffer);
	*buffer = ft_calloc(buffer_len + BUFFER_SIZE + 1, 1);
	if (!(*buffer))
	{
		free(temp);
		return (0);
	}
	ft_strlcpy(*buffer, temp, buffer_len + 1);
	free(temp);
	return (1);
}

char	*ft_return_line(char **buffer, char **remainder)
{
	char	*line;
	int		line_len;
	char	*temp;

	line_len = ft_strlen(*buffer) - ft_strlen(*remainder) + 1;
	line = malloc(line_len + 1);
	if (!line)
	{
		free(*buffer);
		return (NULL);
	}
	temp = malloc(1 + ft_strlen(*remainder));
	if (!temp)
	{
		free(*buffer);
		free(line);
		return (NULL);
	}
	ft_strlcpy(temp, ((*remainder) + 1), ft_strlen(*remainder));
	*remainder = temp;
	ft_strlcpy(line, *buffer, line_len + 1);
	free(*buffer);
	return (line);
}
