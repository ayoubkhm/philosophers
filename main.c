/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:13:34 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/14 00:13:35 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_all(t_data *data, t_philos **philos, int argc, char **argv)
{
	if (init_data_values(data, argc, argv) != 0)
		return (1);
	if (init_global_mutexes(data) != 0)
		return (1);
	if (init_fork_mutexes(data) != 0)
	{
		destroy_global_mutexes(data);
		return (1);
	}
	if (init_philosophers(philos, data) != 0)
	{
		destroy_fork_mutexes(data);
		free(data->forks);
		destroy_global_mutexes(data);
		return (1);
	}
	return (0);
}

int	start_philosopher_threads(t_data *data, t_philos *philosophers)
{
	int	i;

	i = 0;
	data->start_time = get_current_time();
	while (i < data->nb_philosophers)
	{
		philosophers[i].last_meal_time = data->start_time;
		if (pthread_create(&philosophers[i].thread_id, NULL,
				philosopher_routine, (void *)&philosophers[i]) != 0)
		{
			printf("Error: Thread creation failed\n");
			return (i);
		}
		i++;
	}
	return (0);
}

void	handle_thread_creation_error(t_philos *philosophers, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		pthread_cancel(philosophers[j].thread_id);
		j++;
	}
}

int	manage_monitor_and_wait(t_data *data, t_philos *philosophers)
{
	pthread_t	monitor_thread;
	int			i;

	if (pthread_create(&monitor_thread, NULL,
			monitor_routine, (void *)philosophers) != 0)
	{
		printf("Error: Monitor thread creation failed\n");
		i = 0;
		while (i < data->nb_philosophers)
		{
			pthread_cancel(philosophers[i].thread_id);
			i++;
		}
		return (1);
	}
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < data->nb_philosophers)
	{
		pthread_join(philosophers[i].thread_id, NULL);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data			data;
	t_philos		*philosophers;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Incorrect number of arguments\n");
		return (1);
	}
	if (initialize_all(&data, &philosophers, argc, argv) != 0)
		return (1);
	if (start_philosopher_threads(&data, philosophers) != 0)
	{
		free_resources(&data, philosophers);
		return (1);
	}
	if (manage_monitor_and_wait(&data, philosophers) != 0)
	{
		free_resources(&data, philosophers);
		return (1);
	}
	free_resources(&data, philosophers);
	return (0);
}
