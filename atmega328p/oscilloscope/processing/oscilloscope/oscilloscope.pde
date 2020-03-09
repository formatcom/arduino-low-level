// processing-java --sketch=../oscilloscope --run
// REF: https://processing.org/reference/
import processing.serial.*;

int[] samples;
int   smax;

Serial pcom;

void setup()
{
	size(800, 550);
	// cuantas veces por segundos queremos que se dibuje
	frameRate(60);

	smax = width / 2;
	samples = new int[smax];

	for (int i = 0; i < smax; i++)
		samples[i] = -1;

	pcom = new Serial(this, "/dev/ttyACM0", 9600);

}

void gui()
{
	background(102);
	textSize(16);

	// Color de la linea horizontal
	stroke(60, 60, 60);

	// Lineas Horizontales
	for (int y = 0; y <= height / 50; y++)
		line(0, y * 50, width, 1 + y * 50);

	// Lineas Vertical
	for (int x = 0; x <= width / 50; x++)
		line(x * 50, 0, 1 + x * 50, height);
}

void draw()
{
	// Super curioso que este ciclo no afecta al render
	// pero segun la documentacion se trabaja de esta manera
	// si cambiamos el valor de frameRate a un valor muy bajo
	// se vera reflejado que esta funcionando perfectamente
	while (pcom.available() > 0)
	{
		gui();

		byte[] c = new byte[1];
		pcom.readBytes(c);

		// Simulamos un unsigned char
		int uchar = c[0] & 0xFF;

		// RESOLUCION DE 8 BIT = 2 ^ 8 = 256
		// Vol = ( ADC * VREF ) / RESOLUCION
		int vol = ( uchar * 5 ) / 255;

		text(uchar, 250, 150);
		text(vol,   250, 170);

		for (int i = 0; i < smax - 1; i++)
			samples[i] = samples[i + 1];

		samples[smax - 1] = height - vol * 50;

		stroke(0, 255, 0);
		for (int i = 1; i <= smax/2; i++)
		{
			if (samples[i] > -1 && samples[i - 1] > -1)
			{
				line(i * 50, samples[i - 1], i * 50 + 50, samples[i]);
			}
		}
	}
}
