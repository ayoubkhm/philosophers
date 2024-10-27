/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:13:42 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/14 00:13:44 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philosopher_state(t_philos *philo)
{
	t_data	*data;
	int		result;

	data = philo->data;
	result = 0;
	pthread_mutex_lock(&data->meal_mutex);
	if (philo->done)
	{
		pthread_mutex_unlock(&data->meal_mutex);
		return (0);
	}
	if ((get_current_time() - philo->last_meal_time) > data->t_to_die)
	{
		pthread_mutex_lock(&data->someone_died_mutex);
		data->someone_died = 1;
		pthread_mutex_lock(&data->print_mutex);
		printf("%ld %d died\n", get_current_time()
			- data->start_time, philo->id);
		pthread_mutex_unlock(&data->print_mutex);
		pthread_mutex_unlock(&data->someone_died_mutex);
		result = 1;
	}
	pthread_mutex_unlock(&data->meal_mutex);
	return (result);
}

int	check_all_philosophers(t_philos *philosophers)
{
	t_data	*data;
	int		i;
	int		finished_eating;

	data = philosophers[0].data;
	i = 0;
	finished_eating = 1;
	while (i < data->nb_philosophers)
	{
		if (check_philosopher_state(&philosophers[i]))
			return (1);
		pthread_mutex_lock(&data->meal_mutex);
		if (data->must_eat_count != -1
			&& philosophers[i].meals_eaten
			< data->must_eat_count)
			finished_eating = 0;
		pthread_mutex_unlock(&data->meal_mutex);
		i++;
	}
	if (data->must_eat_count != -1 && finished_eating)
	{
		data->all_satisfied = 1;
		printf("Number of meals requested has been achieved.\n");
		return (2);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philos		*philosophers;
	t_data			*data;
	int				status;

	philosophers = (t_philos *)arg;
	data = philosophers[0].data;
	while (!data->someone_died)
	{
		status = check_all_philosophers(philosophers);
		if (status == 1)
			return (NULL);
		if (status == 2)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
