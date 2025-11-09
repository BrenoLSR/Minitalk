/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendos- <brendos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 14:22:54 by brendos-          #+#    #+#             */
/*   Updated: 2025/11/09 15:57:38 by brendos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static t_data	g_data;

static void	handler(int signal, siginfo_t *info, void *context)
{
	int	sig;

	(void)context;
	if (signal == SIGUSR2)
		g_data.byte |= (1 << g_data.bit_count);
	g_data.bit_count++;
	sig = SIGUSR1;
	if (g_data.bit_count == 8)
	{
		if (g_data.byte == '\0')
			sig = SIGUSR2;
		else
			write(1, &g_data.byte, 1);
		g_data.byte = 0;
		g_data.bit_count = 0;
	}
	kill(info->si_pid, sig);
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
