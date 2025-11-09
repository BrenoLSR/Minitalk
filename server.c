/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendos- <brendos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 11:44:09 by brendos-          #+#    #+#             */
/*   Updated: 2025/11/09 13:36:41 by brendos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static t_data	g_data;

static void	handler(int signal, siginfo_t *info, void *context)
{
	(void)context;
	if (signal == SIGUSR2)
		g_data.byte |= (1 << g_data.bit_count);
	g_data.bit_count++;
	if (g_data.bit_count == 8)
	{
		if (g_data.byte == '\0')
			write(1, "\n", 1);
		else
			write(1, &g_data.byte, 1);
		g_data.byte = 0;
		g_data.bit_count = 0;
	}
	if (info && info->si_pid > 0)
		kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	g_data.byte = 0;
	g_data.bit_count = 0;
	write(1, "server id - ", 12);
	ft_putnbr(getpid());
	write(1, "\n", 1);
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}
