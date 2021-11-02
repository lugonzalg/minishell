#!/bin/sh

mkdir -p ./doc
mkdir -p ./bin
echo "\033[1;34m\tCompiling source files of Libft and creating library\033[0;m"
make -C ./src/libft
echo "\033[1;34m\tCompiling bonus source files of Libft and updating library\033[0;m"
make -C ./src/libft bonus
echo "\033[1;34m\tCompiling source files of ft_printf and creating library\033[0;m"
make -C ./src/ft_printf
find ./src -maxdepth 3 -name "*.a" -type f -exec mv {} ./bin \;
find ./ -maxdepth 1 -name "*.txt" -type f -exec mv {} ./doc \;
