#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <math.h>

class Orbiter : public olc::PixelGameEngine
{
public:
	Orbiter()
	{
		sAppName = "Orbiter";
	}
private:

	olc::Pixel colors[6] =
	{
		olc::BLACK,
		olc::BLUE,
		olc::RED,
		olc::YELLOW,
		olc::GREEN,
		olc::WHITE
	};

	const double G = 6.67 * pow(10, -11);
	const int screen[2] = { 1200, 1000 };

	int nObj = 0;
	float t = 0;

	float m;
	float q;

	bool bHover = 0;
	bool bHeld = 0;
	bool ShowMenu = 0;

	struct object {

		int x;
		int y;

		float mass;
		float radius;

		float force;
		float forceArg;
		float acceleration;

	};

	object objs[];

	void initObj()
	{
		nObj++;
		std::cout << "Insert object" << nObj << " Mass: ";
		std::cin >> objs[nObj].mass;
		std::cout << "Insert object" << nObj << " Radius: ";
		std::cin >> objs[nObj].radius;
		objs[nObj].x = screen[0] / 2;
		objs[nObj].y = screen[1] / 2 - (100 * (nObj - 1));
		if (nObj == 2)
		{
			getForces();
			acceleration();
		}
	}

	void drawVects()
	{
		//Calculationg gravitational force orientation
		m = (((float)objs[1].y - (float)objs[2].y) / ((float)objs[1].x - (float)objs[2].x));
		q = (((float)objs[1].x * (float)objs[2].y) - ((float)objs[2].x * (float)objs[1].y)) / ((float)objs[1].x - (float)objs[2].x);
		
		//Calculating gravitational force vector argument
		objs[2].forceArg = atan(m);
		
		//Calculating gravitational force vector relative coords
		float forceX = (objs[2].force * pow(10, 11)) * cos(objs[2].forceArg);
		float forceY = (objs[2].force * pow(10, 11)) * sin(objs[2].forceArg);

		//Drawing objects connection line
		if (m <= 1 && m >= -1)
		{
			int y = objs[2].y;
			for (int x = objs[2].x; x < objs[1].x || x > objs[1].x; )
			{
				y = (m * x) + q;
				Draw(x, y);

				if (objs[2].x > objs[1].x) x--;
				else x++;
			}
		}
		else
		{
			int x = objs[2].x;
			for (int y = objs[2].y; y < objs[1].y || y > objs[1].y; )
			{
				x = (y - q) / m;
				Draw(x, y);
				if (objs[2].y > objs[1].y) y--;
				else y++;
			}
		}

		//Drawing gravitational foce vector 
		if (objs[2].x > objs[1].x)
			DrawLine(objs[2].x, objs[2].y,objs[2].x - forceX, objs[2].y - forceY, olc::GREEN);
		else 
			DrawLine(objs[2].x, objs[2].y, forceX + objs[2].x, forceY + objs[2].y, olc::GREEN);

	}
	void updateObjs(float fElapsedTime)
	{

		if (t >= (1/(objs[2].acceleration * pow(10, 7))))
		{

			if (m <= 1 && m >= -1)
			{
				objs[2].y = (m * objs[2].x) + q;
				if (objs[1].x > objs[2].x)
				{
					objs[2].x ++;
				}
				else
				{
					objs[2].x --;
				}
			}
			else
			{
				objs[2].x = (objs[2].y - q) / m;
				if (objs[1].y > objs[2].y)
				{
					objs[2].y ++;
				}
				else
				{
					objs[2].y --;
				}
			}

			t = 0;
		}
		else t += fElapsedTime;

	}
	void getForces()
	{
		//Calculating gravitational force 
		float mTot = 1;
		int distance = 0;

		for (int i = 1; i <= nObj; i++)
		{
			mTot *= objs[i].mass;
		}

		distance = sqrt(pow((objs[1].x - objs[2].x), 2) + pow((objs[1].y - objs[2].y), 2));

		for (int i = 1; i <= nObj; i++)
		{
			objs[i].force = G * (mTot / pow(distance, 2));
		}

	}
	void acceleration()
	{
		//Calculating object acceleration 
		objs[2].acceleration = objs[2].force / objs[2].mass;
		//std::cout << objs[2].acceleration << std::endl;
	}

	void input()
	{
		for (int i = 1; i <= nObj; i++)
		{
			
			if (GetMouseX() > objs[i].x - objs[i].radius && GetMouseX() < objs[i].x + objs[i].radius && GetMouseY() > objs[i].y - objs[i].radius && GetMouseY() < objs[i].y + objs[i].radius)
			{
				DrawCircle(objs[i].x, objs[i].y, objs[i].radius + 1, olc::YELLOW);
				if (GetMouse(0).bHeld)
				{
					objs[i].x = GetMouseX();
					objs[i].y = GetMouseY();
				}
			}
				
		}
	}

	void menuInteraction()
	{
		if (ShowMenu)
		{
			
			if (GetMouseX() > ScreenWidth() - 350 && GetMouseX() < ScreenWidth() - 300 && GetMouseY() > 10 && GetMouseY() < 55)
			{
				if (GetMouse(0).bPressed)
				{
					ShowMenu = 0;
				}
				return;
			}
			
			if (GetMouseX() > ScreenWidth() - 275 && GetMouseX() < ScreenWidth() - 35 && GetMouseY() > 55 && GetMouseY() < 95)
			{
				bHover = 1;
				if (GetMouse(0).bHeld)
				{
					bHeld = 1;
				}
				else if (GetMouse(0).bReleased)
				{
					bHeld = 0;
					initObj();
				}
				return;
			}
			else
			{
				bHover = 0;
				bHeld = 0;
			}
				
		}
		else
		{
			
			if (GetMouseX() > ScreenWidth() - 50 && GetMouseX() < ScreenWidth() && GetMouseY() > 0 && GetMouseY() < 50)
			{
				if (GetMouse(0).bPressed)
				{
					ShowMenu = 1;
				}
				return;
			}
				
		}

		return;
	}
	void menu()
	{
		if (ShowMenu)
		{

			FillRect(ScreenWidth() - 301, 0, 300, ScreenHeight() - 1, olc::BLACK);
			DrawRect(ScreenWidth() - 301, 0, 300, ScreenHeight() - 1);

			DrawRect(ScreenWidth() - 350, 0, 49, 50);
			FillRect(ScreenWidth() - 345, 23, 40, 4);


			if (bHover)
			{
				FillRect(ScreenWidth() - 275, 55, 240, 40, olc::BLACK);
				DrawRect(ScreenWidth() - 275, 55, 240, 40, olc::YELLOW);
				if(bHeld)
					DrawString(ScreenWidth() - 255, 65, "+ New Object", olc::YELLOW, 2);
				else
					DrawString(ScreenWidth() - 255, 65, "+ New Object", olc::WHITE, 2);
			}
			else
			{
				FillRect(ScreenWidth() - 275, 55, 240, 40, olc::GREY);
				FillRect(ScreenWidth() - 270, 50, 240, 40, olc::BLACK);
				DrawRect(ScreenWidth() - 270, 50, 240, 40);
				DrawString(ScreenWidth() - 250, 60, "+ New Object", olc::WHITE, 2);
			}

			if (nObj != 0)
			{
				for (int i = 1; i <= nObj; i++)
				{
					FillRect(ScreenWidth() - 270, 110 + ((i - 1) * 130), 240, 130, olc::BLACK);
					DrawRect(ScreenWidth() - 270, 110 + ((i - 1) * 130), 240, 130);
					DrawString(ScreenWidth() - 250, 120 + ((i - 1) * 130), "Object" + std::to_string(i), olc::WHITE, 2);
					FillRect(ScreenWidth() - 100, 115 + ((i - 1) * 130), 50, 25, colors[i]);

					DrawString(ScreenWidth() - 230, 155 + ((i - 1) * 130), "Mass: " + std::to_string((int)objs[i].mass), olc::WHITE, 2);
					DrawString(ScreenWidth() - 230, 175 + ((i - 1) * 130), "Radius: " + std::to_string((int)objs[i].radius), olc::WHITE, 2);
					DrawString(ScreenWidth() - 230, 195 + ((i - 1) * 130), "Coords: " + std::to_string(objs[i].x) + "," + std::to_string(objs[i].y), olc::WHITE, 1.5);
				}
			}

		}
		else
		{
			DrawRect(ScreenWidth() - 51, 0, 50, 50);
			FillRect(ScreenWidth() - 45, 23, 40, 4);
			FillRect(ScreenWidth() - 27, 5, 5, 40);
		}

		menuInteraction();

	}

public:
	bool OnUserCreate() override
	{
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{

		Clear(olc::BLACK);
		if (nObj != 0)
		{	
			input();
		
			if (nObj == 2)
			{
				updateObjs(fElapsedTime);
				drawVects();
			}
			for (int i = 1; i <= nObj; i++)
			{
				FillCircle(objs[i].x, objs[i].y, objs[i].radius, colors[i]);
			}
			
		}

		menu();

		return true;
	}
};
int main()
{
	Orbiter demo;
	if (demo.Construct(1500, 1000, 1, 1))
		demo.Start();
	return 0;
}