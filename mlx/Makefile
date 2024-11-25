NAME        =	libmlx.a

SRCS        = 	srcs/mlx_clear_window.c \
              	srcs/mlx_destroy_display.c \
              	srcs/mlx_destroy_image.c \
              	srcs/mlx_destroy_window.c \
              	srcs/mlx_expose_hook.c \
              	srcs/mlx_flush_event.c \
              	srcs/mlx_get_color_value.c \
              	srcs/mlx_get_data_addr.c \
              	srcs/mlx_hook.c \
              	srcs/mlx_init.c \
              	srcs/mlx_int_anti_resize_win.c \
              	srcs/mlx_int_do_nothing.c \
              	srcs/mlx_int_get_visual.c \
              	srcs/mlx_int_param_event.c \
             	srcs/mlx_int_set_win_event_mask.c \
             	srcs/mlx_int_str_to_wordtab.c \
             	srcs/mlx_int_wait_first_expose.c \
             	srcs/mlx_key_hook.c \
              	srcs/mlx_loop_hook.c \
              	srcs/mlx_loop.c \
              	srcs/mlx_mouse_hook.c \
              	srcs/mlx_mouse.c \
              	srcs/mlx_new_image.c \
              	srcs/mlx_new_window.c \
              	srcs/mlx_pixel_put.c \
              	srcs/mlx_put_image_to_window.c \
              	srcs/mlx_rgb.c \
              	srcs/mlx_screen_size.c \
              	srcs/mlx_set_font.c \
              	srcs/mlx_string_put.c \
              	srcs/mlx_xpm.c
              
S_DIR       =	srcs
I_DIR       =	include
O_DIR       =	obj

CC          =	gcc
OBJ         =	$(patsubst $(S_DIR)/%.c, $(O_DIR)/%.o, $(SRCS))
CFLAGS      =	-O3 -I$(I_DIR)

all: 			$(NAME)

$(O_DIR)/%.o:	$(S_DIR)/%.c
				@mkdir -p $(O_DIR)	
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME): 		$(OBJ)
				@ar -r $(NAME) $(OBJ)
				@ranlib $(NAME)

clean:
				@rm -rf $(O_DIR)

fclean: 		clean
				@rm -f $(NAME)
				@rm -rf $(O_DIR)

re: 			fclean all

.PHONY: 		all clean fclean re
