/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendos- <brendos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 11:44:27 by brendos-          #+#    #+#             */
/*   Updated: 2025/11/09 13:52:46 by brendos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# define _DEFAULT_SOURCE
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>

void				ft_putnbr(int nb);
int					ft_atoi(char *str);
void				ft_putchar(char c);

typedef struct s_data
{
	unsigned char	byte;
	int				bit_count;
}					t_data;

#endif
