/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsihmaok <rshimaok@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:01:36 by rsihmaok          #+#    #+#             */
/*   Updated: 2022/11/07 13:01:36 by rsihmaok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_free(char **ptr1, char **ptr2)
{
	if (ptr1)
	{
		free(*ptr1);
		*ptr1 = NULL;
	}
	if (ptr2)
	{
		free(*ptr2);
		*ptr2 = NULL;
	}
	return (NULL);
}

char	*ft_next(char *save, size_t len, int *flg)
{
	char	*s;
	size_t	new_len;

	new_len = ft_strlen(save) - len;
	if (new_len <= 1)
		return (ft_free(&save, NULL));
	s = (char *)malloc(new_len);
	if (!s)
	{
		*flg = -1;
		return (ft_free(&save, NULL));
	}
	ft_strlcpy(s, &(save[len + 1]), new_len);
	free(save);
	return (s);
}

char	*ft_line(char *save, size_t len)
{
	char	*line;

	line = (char *)malloc(len + 2);
	if (!line)
		return (ft_free(&save, NULL));
	ft_strlcpy(line, save, len + 2);
	return (line);
}

char	*ft_read(int fd, char *save)
{
	char	*buf;
	ssize_t	read_ret;

	read_ret = 0;
	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (ft_free(&save, NULL));
	while (!ft_strchr(save, '\n'))
	{
		read_ret = read(fd, buf, BUFFER_SIZE);
		if (read_ret == -1)
			return (ft_free(&save, &buf));
		if (read_ret == 0)
			break ;
		buf[read_ret] = '\0';
		save = gnl_strjoin(save, buf);
		if (!save)
			break ;
	}
	free(buf);
	return (save);
}

char	*get_next_line(int fd)
{
	static char	*save;  //static char *save[OPEN_MAX]にするとbonusできる
	char		*line;
	size_t		len;
	int			error_flg;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	save = ft_read(fd, save);
	if (!save)
		return (NULL);
	len = 0;
	while (save[len] != '\n' && save[len])
		len++;
	line = ft_line(save, len);
	if (!line)
		return (NULL);
	error_flg = 0;
	save = ft_next(save, len, &error_flg);
	if (error_flg == -1)
		return (ft_free(&line, NULL));
	return (line);
}

// gcc -Wall -Wextra -Werror -g -fsanitize=address -fsanitize=undefined
// -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c
// int	main(void)
// {
// 	int fd;
// 	size_t i;
// 	char *line;

// 	fd = open("test.txt", O_RDONLY);
// 	i = 1;
// 	// fd = 0;
// 	while (1)
// 	{
// 		line = get_next_line(fd);
// 		printf("%ld: %s \n", i, line);
// 		if (line == NULL)
// 			break ;
// 		free(line);
// 		i++;
// 	}
// 	return (0);
// }