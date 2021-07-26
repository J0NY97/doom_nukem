gcc -o path_finder bui_path.c -I../libft -I../libpf -I../SDL2/includes -L../libft -L../libpf -L../SDL2/lib -lSDL2 -lpf -lft;
./path_finder;
rm path_finder;
