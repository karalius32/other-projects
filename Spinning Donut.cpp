#include <iostream>
#include <string>
#include <cmath>

using namespace std;

string render_frame(double angle)
{
	const int width = 200, height = 60, center_x = width / 2, center_y = height / 2;
	int k1 = 25, k2 = 100;
	int r1 = 10, r2 = 40;
	char output[height][width];
	fill(&output[0][0], &output[0][0] + sizeof(output), ' ');

	double cosA = cos(angle);
	double sinA = sin(angle);
	double cosA2 = pow(cosA, 2);
	double sinA2 = pow(sinA, 2);
	double sinA3 = sinA2 * sinA;
	double sincosA = sinA * cosA;

	for (double i = 0; i < 6.28; i += 0.628)
	{
		for (double j = 0; j < 6.28; j += 0.025)
		{
			double cosAj = cos(angle + j);
			double sinAj = sin(angle + j);

			int X = r2 + r1 * cos(i);
			int Y = r1 * sin(i);
			int Z = 0;

			// y rotation
			int x = X * cosAj - Z * sinAj;
			int y = Y;
			int z = X * sinAj + Z * cosAj;

			// x rotation
			int temp_y = y;
			y = y * cosA - z * sinA;
			z = temp_y * sinA + z * cosA;

			// z rotation
			int temp_x = x;
			x = x * cosA - y * sinA;
			y = temp_x * sinA + y * cosA;

			//".,-~:;=!*#$@"
			char c = "@$#*!=;:~-,."[(int)(((double)z + 40) / 80 * 12)];
			output[k1 * y / (k2 + z) + center_y][k1 * x / (k2 + z) + center_x] = c;
		}
	}

	string frame = "";
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			frame.push_back(output[i][j]);
		}

		frame.push_back('\n');
	}

	return frame;
}

int main()
{
	int a = 10;

	double angle = 0;
	while (true)
	{
		cout << render_frame(angle);
		angle += 0.1;
		system("cls");
	}

	return 0;
}