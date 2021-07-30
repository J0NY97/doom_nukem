gcc -o path_finder bui_path.c -I../libft -L../libft -L../libpf -I../libpf -lpf -lft -I../SDL/include -L../SDL/lib -lSDL2;
./path_finder;
rm path_finder;
