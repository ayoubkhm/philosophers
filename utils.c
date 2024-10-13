/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:14:15 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/14 00:14:18 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_current_time(void)
{
	struct timeval	tv;
	long			time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

void	ft_usleep(long time_in_ms)
{
	long	start_time;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
		usleep(500);
}

void	print_status(t_philos *philo, char *message)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->someone_died)
	{
		printf("%ld %d %s\n",
			get_current_time() - philo->data->start_time,
			philo->id,
			message);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	free_resources(t_data *data, t_philos *philosophers)
{
	int	i;

	i = 0;
	while (i < data->nb_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	free(data->forks);
	free(philosophers);
}
