FLAG =-Wall -Werror -Wextra 

all: clang s21_grep tests cppcheck leaks

s21_grep: clean 
	gcc $(FLAG) s21_grep.c -o s21_grep

tests:
	sh test.sh
	sh test_grep.sh
	# sh test_hard.sh

clang:
	clang-format -i --style=file:../../materials/linters/.clang-format ./s21_grep.c
	clang-format -n --style=file:../../materials/linters/.clang-format ./s21_grep.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem ./s21_grep.c

leaks:
	leaks -atExit -- ./s21_grep

clean: 
	rm -f ./s21_grep
	rm -f supertest.txt