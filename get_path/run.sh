gcc -o find_path ./get_path/path.c -I./libs/libft -L./libs/libft -I./libs/libpf -L./libs/libpf -lft -lpf -I ./libs/SDL_MAC/SDL2.framework/Headers -framework SDL2 -F ./libs/SDL_MAC;
./find_path;
rm find_path;
