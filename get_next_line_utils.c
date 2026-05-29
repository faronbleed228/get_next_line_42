/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 10:31:24 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/05/14 11:25:14 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_calloc(size_t nelem, size_t elsize)
{
	void			*ptr;
	size_t			max;
	size_t			i;
	unsigned char	*str;

	i = 0;
	max = -1;
	if (nelem == 0 || elsize == 0)
	{
		ptr = malloc(1);
		return (ptr);
	}
	if (elsize != 0 && nelem > max / elsize)
		return (NULL);
	ptr = malloc(elsize * nelem);
	if (!ptr)
		return (NULL);
	str = (unsigned char *)ptr;
	while (elsize * nelem > i)
		str[i++] = '\0';
	return (ptr);
}

size_t	ft_strlen(const char *str)
{
	int	result;

	result = 0;
	while (*str)
	{
		result++;
		str++;
	}
	return (result);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	i;

	i = 0;
	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	while (src[i] && (i + 1) < size)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}

char	*ft_free(char **buffer, char **remainder)
{
	if (*buffer)
		free(*buffer);
	if (*remainder)
		free(*remainder);
	*buffer = NULL;
	*remainder = NULL;
	return (NULL);
}
