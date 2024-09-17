all:
	g++ *.cpp -o trabalho1 -Iinclude -Lsrc -lglfw -lGL src/glad.c -g
clean:
	rm trabalho1 *.o
