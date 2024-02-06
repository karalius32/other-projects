#include <iostream>
#include <vector>
#include <Windows.h>

using namespace std;

const int width = 100, height = 100;
wchar_t output[width * height];
float player_x = 8, player_y = 8, player_z = 7, player_rotation_z = 6.28 / 2, player_rotation_x = 6.28 / 6, player_pov = 6.28 / 5;
const int map_width = 20, map_length = 20, map_height = 20;
float ray_speed = 0.05;
float eps = 0.05;

int map[map_height][map_length][map_width];
int map_layer[map_length][map_width] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


void render_frame()
{
	fill(&output[0], &output[0] + width * height, ' ');

	float rotation_increment_z = player_pov / width;
	float rotation_increment_x = player_pov / height;
	for (int i = 0; i < width; i++)
	{
		float ray_rotation_z = player_rotation_z - (rotation_increment_z * i);

		for (int j = 0; j < height; j++)
		{
			char c_to_render = ' ';
			float ray_rotation_x = player_rotation_x - rotation_increment_x * j;
			bool hit_wall = false;
			float ray_x = player_x, ray_y = player_y, ray_z = player_z;
			float distance = 0;

			while (!hit_wall)
			{
				ray_x += ray_speed * cos(ray_rotation_z) * cos(ray_rotation_x);
				ray_y -= ray_speed * sin(ray_rotation_z) * cos(ray_rotation_x);
				ray_z += ray_speed * sin(ray_rotation_x);
				distance += ray_speed;

				if (ray_x < 0 || ray_x >= map_width || ray_y < 0 || ray_y >= map_length || ray_z < 0 || ray_z >= map_height || distance > 10)
				{
					break;
				}
				if (map[(int)ray_z][(int)ray_y][(int)ray_x] == 1)
				{
					hit_wall = true;
					c_to_render = '#';

					if ((ray_y - (int)ray_y < eps || (int)ray_y + 1 - ray_y < eps) && (ray_x - (int)ray_x < eps || (int)ray_x + 1 - ray_x < eps) ||
						(ray_y - (int)ray_y < eps || (int)ray_y + 1 - ray_y < eps) && (ray_z - (int)ray_z < eps || (int)ray_z + 1 - ray_z < eps) ||
						(ray_x - (int)ray_x < eps || (int)ray_x + 1 - ray_x < eps) && (ray_z - (int)ray_z < eps || (int)ray_z + 1 - ray_z < eps))
						c_to_render = ' ';
				}
				else if (map[(int)ray_z][(int)ray_y][(int)ray_x] == 2)
				{
					hit_wall = true;
					c_to_render = '.';

					if ((ray_y - (int)ray_y < eps || (int)ray_y + 1 - ray_y < eps) && (ray_x - (int)ray_x < eps || (int)ray_x + 1 - ray_x < eps) ||
						(ray_y - (int)ray_y < eps || (int)ray_y + 1 - ray_y < eps) && (ray_z - (int)ray_z < eps || (int)ray_z + 1 - ray_z < eps) ||
						(ray_x - (int)ray_x < eps || (int)ray_x + 1 - ray_x < eps) && (ray_z - (int)ray_z < eps || (int)ray_z + 1 - ray_z < eps))
						c_to_render = ' ';
				}
			}

			output[j * width + i] = c_to_render;
		}
	}
}

int main()
{
	fill(&output[0], &output[0] + width * height, ' ');
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	fill(&map[0][0][0], &map[0][0][0] + map_height * map_width * map_length, 0);
	
	for (int i = 0; i < map_width; i++)
	{
		for (int j = 0; j < map_length; j++)
		{
			map[0][i][j] = map_layer[i][j];
			map[1][i][j] = map_layer[i][j];
			map[2][i][j] = map_layer[i][j];
			map[3][i][j] = map_layer[i][j];
		}
	}

	float rotation_speed = 0.1;
	float player_speed = 0.1;

	while (1)
	{
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			float temp_player_x = player_x + player_speed  * cos(player_rotation_z + 1.57 - player_pov / 2);
			float temp_player_y = player_y - player_speed * sin(player_rotation_z + 1.57 - player_pov / 2);
			if (temp_player_x >= 0 && temp_player_x < map_width && temp_player_y >= 0 && temp_player_y < map_length 
				&& map[(int)player_z][(int)temp_player_y][(int)temp_player_x] == 0)
			{
				player_x = temp_player_x;
				player_y = temp_player_y;
			}
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			float temp_player_x = player_x + player_speed * cos(player_rotation_z - 1.57 - player_pov / 2);
			float temp_player_y = player_y - player_speed * sin(player_rotation_z - 1.57 - player_pov / 2);
			if (temp_player_x >= 0 && temp_player_x < map_width && temp_player_y >= 0 && temp_player_y < map_length
				&& map[(int)player_z][(int)temp_player_y][(int)temp_player_x] == 0)
			{
				player_x = temp_player_x;
				player_y = temp_player_y;
			}
		}
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			float temp_player_x = player_x + player_speed * cos(player_rotation_z - player_pov / 2);
			float temp_player_y = player_y - player_speed * sin(player_rotation_z - player_pov / 2);
			if (temp_player_x >= 0 && temp_player_x < map_width && temp_player_y >= 0 && temp_player_y < map_length
				&& map[(int)player_z][(int)temp_player_y][(int)temp_player_x] == 0)
			{
				player_x = temp_player_x;
				player_y = temp_player_y;
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			float temp_player_x = player_x - player_speed * cos(player_rotation_z - player_pov / 2);
			float temp_player_y = player_y + player_speed * sin(player_rotation_z - player_pov / 2);
			if (temp_player_x >= 0 && temp_player_x < map_width && temp_player_y >= 0 && temp_player_y < map_length
				&& map[(int)player_z][(int)temp_player_y][(int)temp_player_x] == 0)
			{
				player_x = temp_player_x;
				player_y = temp_player_y;
			}
		}
		if (GetAsyncKeyState((unsigned short)'R') & 0x8000)
		{
			float temp_player_z = player_z + player_speed;
			if (map[(int)temp_player_z][(int)player_y][(int)player_x] == 0)
			{
				player_z = temp_player_z;
			}
		}
		if (GetAsyncKeyState((unsigned short)'F') & 0x8000)
		{
			float temp_player_z = player_z - player_speed;
			if (map[(int)temp_player_z][(int)player_y][(int)player_x] == 0)
			{
				player_z = temp_player_z;
			}
		}
		if (GetAsyncKeyState((unsigned short)VK_SPACE) & 0x8000)
		{
			float ray_rotation_z = player_rotation_z - player_pov / 2;
			float ray_rotation_x = player_rotation_x - player_pov / 2;
			bool hit_wall = false;
			float ray_x = player_x, ray_y = player_y, ray_z = player_z;

			while (!hit_wall)
			{
				ray_x += ray_speed * cos(ray_rotation_z) * cos(ray_rotation_x);
				ray_y -= ray_speed * sin(ray_rotation_z) * cos(ray_rotation_x);
				ray_z += ray_speed * sin(ray_rotation_x);

				if (ray_x < 0 || ray_x >= map_width || ray_y < 0 || ray_y >= map_length || ray_z < 0 || ray_z >= map_height)
				{
					break;
				}
				if (map[(int)ray_z][(int)ray_y][(int)ray_x] != 0)
				{
					hit_wall = true;
					map[(int)ray_z][(int)ray_y][(int)ray_x] = 0;
				}
			}
		}
		if (GetAsyncKeyState((unsigned short)VK_SHIFT) & 0x8000)
		{
			int block_type = 2;

			float ray_rotation_z = player_rotation_z - player_pov / 2;
			float ray_rotation_x = player_rotation_x - player_pov / 2;
			bool hit_wall = false;
			float ray_x = player_x, ray_y = player_y, ray_z = player_z;

			while (!hit_wall)
			{
				ray_x += ray_speed * cos(ray_rotation_z) * cos(ray_rotation_x);
				ray_y -= ray_speed * sin(ray_rotation_z) * cos(ray_rotation_x);
				ray_z += ray_speed * sin(ray_rotation_x);

				if (ray_x < 0 || ray_x >= map_width || ray_y < 0 || ray_y >= map_length || ray_z < 0 || ray_z >= map_height)
				{
					break;
				}
				if (map[(int)ray_z][(int)ray_y][(int)ray_x] != 0)
				{
					hit_wall = true;

					if ((int)ray_x + 1 - ray_x < eps)
						map[(int)ray_z][(int)ray_y][(int)ray_x + 1] = block_type;
					else if (ray_x - (int)ray_x < eps && (int)ray_x != 0)
						map[(int)ray_z][(int)ray_y][(int)ray_x - 1] = block_type;
					else if ((int)ray_y + 1 - ray_y < eps)
						map[(int)ray_z][(int)ray_y + 1][(int)ray_x] = block_type;
					else if (ray_y - (int)ray_y < eps && (int)ray_y != 0)
						map[(int)ray_z][(int)ray_y - 1][(int)ray_x] = block_type;
					else if ((int)ray_z + 1 - ray_z < eps)
						map[(int)ray_z + 1][(int)ray_y][(int)ray_x] = block_type;
					else if (ray_z - (int)ray_z < eps && (int)ray_z != 0)
						map[(int)ray_z - 1][(int)ray_y][(int)ray_x] = block_type;
				}
			}
		}
		if (GetAsyncKeyState((unsigned short)VK_LEFT) & 0x8000)
		{
			player_rotation_z += rotation_speed;
		}
		if (GetAsyncKeyState((unsigned short)VK_RIGHT) & 0x8000)
		{
			player_rotation_z -= rotation_speed;
		}
		if (GetAsyncKeyState((unsigned short)VK_UP) & 0x8000)
		{
			if (player_rotation_x < 6.28 / 4)
				player_rotation_x += rotation_speed;
		}
		if (GetAsyncKeyState((unsigned short)VK_DOWN) & 0x8000)
		{
			if (player_rotation_x > 0)
				player_rotation_x -= rotation_speed;
		}

		render_frame();
		swprintf_s(output, 80, L"X=%3.2f, Y=%3.2f, Z=%3.2f, rotation_z=%3.2f, rotation_x=%3.2f", player_x, player_y, player_z, player_rotation_z, player_rotation_x);
		output[width * height / 2 + width / 2] = '+';
		WriteConsoleOutputCharacter(hConsole, output, width * height, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}
