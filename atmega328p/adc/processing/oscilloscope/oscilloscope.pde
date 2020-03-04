// processing-java --sketch=../oscilloscope --run
// REF: https://processing.org/reference/
import processing.serial.*;

int[] samples;
int   smax;

Serial pcom;

void setup()
{
	size(530, 290);
	background(102);

	smax = width / 2;
	samples = new int[smax];

	for (int i = 0; i < smax; i++)
		samples[i] = -1;

		pcom = new Serial(this, "/dev/ttyUSB0", 9600);
}

void gui()
{
	textSize(16);

	text(5, 5, 280 - 5 * 50 - 2);
	text('V', 15, 280 - 5 * 50 - 2);
	for (int i = 4; i>=0; i--)
	{
		text(i, 5, 280 - i * 50 - 2);
	}

	// Color de la linea horizontal
	stroke(60, 60, 60);
	for (int i = 0; i < 6; i++)
		line(35, 28 + i * 50, width-15, 28 + i * 50);

	// Linea vertical
	for (int i = 0; i < (width - 30) / 50  + 1; i++)
		line(35 + i * 50, 28, 35 + i * 50, height - 12);
}

void draw()
{
	gui();
}
