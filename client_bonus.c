/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendos- <brendos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 14:23:14 by brendos-          #+#    #+#             */
/*   Updated: 2025/11/09 15:41:32 by brendos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile sig_atomic_t	g_acknowledged = 0;

static void	handle_ack(int sig)
{
	if (sig == SIGUSR2)
		write(1, "\nMessage received successfully!\n", 32);
	g_acknowledged = 1;
}

static void	send_character(pid_t server_pid, unsigned char ch)
{
	int	bit_position;

	bit_position = 0;
	while (bit_position < 8)
	{
		g_acknowledged = 0;
		if ((ch >> bit_position) & 1)
			kill(server_pid, SIGUSR2);
		else
			kill(server_pid, SIGUSR1);
		while (!g_acknowledged)
			pause();
		bit_position++;
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;
	pid_t				server_pid;
	int					i;

	if (argc != 3)
		return (1);
	server_pid = (pid_t)ft_atoi(argv[1]);
	if (server_pid <= 0 || kill(server_pid, 0) == -1)
		return (1);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_ack;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	i = 0;
	while (1)
	{
		send_character(server_pid, (unsigned char)argv[2][i]);
		if (argv[2][i] == '\0')
			break ;
		i++;
	}
	return (0);
}
