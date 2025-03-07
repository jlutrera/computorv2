/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plotter.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:56:16 by jutrera-          #+#    #+#             */
/*   Updated: 2025/02/11 19:56:16 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLOTTER_H
# define PLOTTER_H

# include "computor.h"
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include "../mlx/include/mlx.h"

typedef struct s_data
{
	void 		*mlx;
	void 		*win;
	int 		width;
	int 		height;
	Display		*display;
	Window		window;
	int 		window_destroyed;
	char		*function;
	double		zoom;
	double		offset_x;
	double		offset_y;
	int			xpress;
	int			ypress;
	bool		rightbuttonpressed;
	XSizeHints	*size_hints;
} t_data;

// Window size by default
# define WIDTH			500
# define HEIGHT			500

// Colours
# define P_RED   		0xFF0000
# define P_GREEN 		0x00FF00
# define P_BLUE  		0x0000FF
# define P_WHITE 		0xFFFFFF
# define P_YELLOW		0xFFFF00
# define P_CYAN 		0x4BB5FF
# define P_DARKGREY		0x404040

// Keycodes
# define K_UP			0xFF52
# define K_DOWN			0xFF54
# define K_LEFT			0xFF51
# define K_RIGHT		0xFF53
# define Q_KEY			113
# define q_KEY			81

// Mouse buttons
# define MSCROLL_UP		4
# define MSCROLL_DOWN	5
# define MSBTN_LEFT		1
# define MSBTN_RIGTH	3

#endif