/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   driver_talk.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:51:45 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/22 20:01:48 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

void	driver_talk(void)
{
	int		fd;
	char	*tty;
	int		status;
	//struct winsize	win;
	//struct termios	term;
	fd = ttyslot();
	status = isatty(fd);
	if (0 && status)
	{
		tty = ttyname(fd);
		//status = ioctl(fd, TIOCGWINSZ, &win);
		//status = ioctl(fd, TIOCGETA, &term);
		//win.ws_col = 65535;
		//status = ioctl(fd, TIOCSWINSZ, &win);
	}
}
