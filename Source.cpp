#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <math.h>
#include <vector>

class Orbiter : public olc::PixelGameEngine
{
public:
	Orbiter()
	{
		sAppName = "Orbiter";
	}
private:

	olc::Pixel colors[5] =
	{
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

	std::vector<object> objs;

	void initObj()
	{
		objs.push_back(object());
		nObj++;
		std::cout << "Insert object" << nObj << " Mass: ";
		std::cin >> objs[nObj - 1].mass;
		std::cout << "Insert object" << nObj << " Radius: ";
		std::cin >> objs[nObj - 1].radius;
		objs[nObj - 1].x = screen[0] / 2;
		objs[nObj - 1].y = screen[1] / 2 - (100 * (nObj - 1));
		if (objs.size() > 2) 
		{
			getForces();
			acceleration();
		}
	}

	void drawVects()
	{
		//Calculationg gravitational force orientation
		m = (((float)objs[0].y - (float)objs[1].y) / ((float)objs[0].x - (float)objs[1].x));
		q = (((float)objs[0].x * (float)objs[1].y) - ((float)objs[1].x * (float)objs[0].y)) / ((float)objs[0].x - (float)objs[1].x);
		
		//Calculating gravitational force vector argument
		objs[1].forceArg = atan(m);
		
		//Calculating gravitational force vector relative coords
		float forceX = (objs[1].force * pow(10, 11)) * cos(objs[1].forceArg);
		float forceY = (objs[1].force * pow(10, 11)) * sin(objs[1].forceArg);

		//Drawing objects connection line
		if (m <= 1 && m >= -1)
		{
			int y = objs[1].y;
			for (int x = objs[1].x; x < objs[0].x || x > objs[0].x; )
			{
				y = (m * x) + q;
				Draw(x, y);

				if (objs[1].x > objs[0].x) x--;
				else x++;
			}
		}
		else
		{
			int x = objs[1].x;
			for (int y = objs[1].y; y < objs[0].y || y > objs[0].y; )
			{
				x = (y - q) / m;
				Draw(x, y);
				if (objs[1].y > objs[0].y) y--;
				else y++;
			}
		}

		//Drawing gravitational foce vector 
		if (objs[1].x > objs[0].x)
			DrawLine(objs[1].x, objs[1].y,objs[1].x - forceX, objs[1].y - forceY, olc::GREEN);
		else 
			DrawLine(objs[1].x, objs[1].y, forceX + objs[1].x, forceY + objs[1].y, olc::GREEN);

	}
	void updateObjs(float fElapsedTime)
	{
		if (objs.size() > 2) {
			if (t >= (1 / (objs[1].acceleration * pow(10, 7))))
			{

				if (m <= 1 && m >= -1)
				{
					objs[1].y = (m * objs[1].x) + q;
					if (objs[0].x > objs[1].x)
					{
						objs[1].x++;
					}
					else
					{
						objs[1].x--;
					}
				}
				else
				{
					objs[1].x = (objs[1].y - q) / m;
					if (objs[0].y > objs[1].y)
					{
						objs[1].y++;
					}
					else
					{
						objs[1].y--;
					}
				}

				t = 0;
			}
			else t += fElapsedTime;
		}
	}
	void getForces()
	{
		//Calculating gravitational force 
		float mTot = 1;
		int distance = 0;

		for (int i = 0; i < objs.size(); i++)
		{
			mTot *= objs[i].mass;
		}

		if (objs.size() > 2) {
			distance = sqrt(pow((objs[0].x - objs[1].x), 2) + pow((objs[0].y - objs[1].y), 2));
		}

		for (int i = 0; i < objs.size(); i++)
		{
			objs[i].force = G * (mTot / pow(distance, 2));
		}

	}
	void acceleration()
	{
		//Calculating object acceleration ù
		if (objs.size() > 2) {
			objs[1].acceleration = objs[1].force / objs[1].mass;
		}
		//std::cout << objs[1].acceleration << std::endl;
	}

	void input()
	{
		for (int i = 0; i < objs.size(); i++)
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
				for (int i = 0; i < objs.size(); i++)
				{
					FillRect(ScreenWidth() - 270, 110 + (i * 130), 240, 130, olc::BLACK);
					DrawRect(ScreenWidth() - 270, 110 + (i * 130), 240, 130);
					DrawString(ScreenWidth() - 250, 120 + (i * 130), "Object" + std::to_string(i + 1), olc::WHITE, 2);
					FillRect(ScreenWidth() - 100, 115 + (i * 130), 50, 25, colors[i]);

					DrawString(ScreenWidth() - 230, 155 + (i * 130), "Mass: " + std::to_string((int)objs[i].mass), olc::WHITE, 2);
					DrawString(ScreenWidth() - 230, 175 + (i * 130), "Radius: " + std::to_string((int)objs[i].radius), olc::WHITE, 2);
					DrawString(ScreenWidth() - 230, 195 + (i * 130), "Coords: " + std::to_string(objs[i].x) + "," + std::to_string(objs[i].y), olc::WHITE, 1.5);
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
		if (objs.size() != 0)
		{	
			input();
		
			if (objs.size() == 2)
			{
				updateObjs(fElapsedTime);
				drawVects();
			}
			for (int i = 0; i < objs.size(); i++)
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