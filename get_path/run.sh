gcc -o find_path ./get_path/path.c -I ./libs/SDL_Frameworks/SDL2.framework/Headers -framework SDL2 -F ./libs/SDL_Frameworks;
./find_path;
rm find_path;
