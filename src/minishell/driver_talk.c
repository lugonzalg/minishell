/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   driver_talk.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:51:45 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/19 20:29:07 by mikgarci         ###   ########.fr       */
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
	struct winsize	win;
	struct termios	term;

	fd = ttyslot();
	status = isatty(fd);
	if (status)
	{
		tty = ttyname(fd);
//		fd = open(tty, =_
		status = ioctl(fd, TIOCGWINSZ, &win);
		status = ioctl(fd, TIOCGETA, &term);
	}
	win.ws_col = 65535;
	status = ioctl(fd, TIOCSWINSZ, &win);
}
