/*
 * Creado por Vinicio Valbuena
 *
 * Trabajare con multihilos ya que quiero poder trabajar la
 * parte grafica y la parte de comunicación con el hardware
 * en procesos paralelos.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <GL/glut.h>

#define WIDTH  800
#define HEIGHT 600
#define CELL   50
#define VLINE  ( WIDTH/CELL  )
#define HLINE  ( HEIGHT/CELL )
#define SMAX   ( WIDTH/2     )
#define PCOM   "/dev/ttyACM0"

#define STATE_INIT          ( 1<<0 )
#define STATE_CONNECTED     ( 1<<1 )
#define STATE_DISCONNECTED  ( 1<<2 )
#define STATE_END           ( 1<<3 )

struct sync {
	int samples[SMAX];
	uint8_t state;
};

float n;
int pcom;

struct sync * control;

void * newSync()
{
	key_t key = ftok("/proc/self", 'k');
	if (key == -1)
	{
		return (void *) -1;
	}

	int shmid = shmget(key, sizeof(struct sync), IPC_CREAT | 0666);
	if (shmid == -1)
	{
		return (struct sync *) -1;
	}

	return (void *) shmat(shmid, NULL, 0);

};

void drawGrid()
{

	// OpenGL se representa el mundo de -1 a 1 asi que pasamos de pixel
	// al rango de OpenGL

	// 2.0 es la amplitud del rango en OpenGL es decir desde el -1 hasta el 1
	// son 2 espacio.

	n = 2.0 / VLINE;

	for (float x = -1.0; x <= 1; x += n)
	{
		glBegin (GL_LINES);
		glVertex2f(x, -1.0);
		glVertex2f(x,  1.0);
		glEnd();

	}

	n = 2.0 / HLINE;

	for (float y = -1.0; y <= 1; y += n)
	{
		glBegin (GL_LINES);
		glVertex2f(-1.0, y);
		glVertex2f( 1.0, y);
		glEnd();
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGrid();

	glutSwapBuffers();
}

void gui()
{
	int argc = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("oscilloscope");
	glutDisplayFunc(display);
	glutMainLoop();
}

int main(void)
{
	pid_t pGUI;
	pid_t pIO;

	control = newSync();
	if (control == (void *)-1)
	{
		printf("%s\n", strerror(errno));
		return 1;
	}

	control->state = STATE_INIT|STATE_DISCONNECTED;
	pGUI = fork();

	if (pGUI == -1)
	{
		printf("%s\n", strerror(errno));
		return 2;
	}

	if (pGUI == 0)
	{
		gui();
		return 0;
	}

	pIO = fork();

	if (pIO == -1)
	{
		printf("%s\n", strerror(errno));
		return 3;
	}

	if (pIO == 0)
	{
		// Esto mover a una funcion de control
		// no gustar que este en el main
		for(;;)
		{
			pcom = open(PCOM, O_RDONLY);
			if (pcom == -1)
			{
				printf("PCOM %s | ERROR %s\n", PCOM, strerror(errno));
			}
			else
			{
				control->state |= STATE_CONNECTED;
				break;
			}

			if ((control->state & STATE_END))
			{
				break;
			}
			sleep(2);
		}

		close(pcom);
		printf("CLOSE PCOM %s | STATE %X \n", PCOM, control->state);
		return 0;
	}

	// Espera a que termine el proceso que controla la gui
	while (waitpid(pGUI, NULL, 0) != pGUI);
	control->state = STATE_END;

	// Esperamos que termine el proceso que controla la comunicación
	while (waitpid(pIO, NULL, 0) != pIO);

	return 0;
}
