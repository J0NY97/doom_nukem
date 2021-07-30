gcc -o path_finder bui_path.c -I../libft -I../libpf -L../libft -L../libpf -lpf -lft -I../SDL_Frameworks/SDL2.framework/Headers -framework SDL2 -F../SDL_Frameworks;
./path_finder;
rm path_finder;
