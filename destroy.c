/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:13:17 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/14 00:13:20 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_fork_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}

void	destroy_global_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
}
