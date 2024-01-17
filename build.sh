uncrustify -c .uncrustify.cfg --no-backup *.c

gcc -o mouse mouse.c -framework ApplicationServices

gcc -o hello hello.c

gcc -o triangle triangle.c -framework OpenGL -framework GLUT