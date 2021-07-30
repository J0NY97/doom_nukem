gcc -o find_path ./get_path/path.c -I./libs/libpf -L./libs/libpf -lpf -I./libs/libft -L./libs/libft -lft -I./libs/SDL/include -L./libs/SDL/lib -lSDL2;
./find_path;
rm find_path;
