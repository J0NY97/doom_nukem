gcc -o find_path ./get_path/path.c -I./libs/SDL2/includes -L./libs/SDL2/lib -lSDL2;
./find_path;
rm find_path;
