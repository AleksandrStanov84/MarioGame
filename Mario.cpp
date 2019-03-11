//CopyRight by Aleksandr A. Stanov
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <String>
#include <vector>
#include <algorithm>

using namespace sf;
using namespace std;

class ENEMY;

float moveX = 0;
float moveY = 0;

const int H = 22;
const int W = 150;

String TileMap[H] = {
	"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
	"0                  w                   w                                                                                                             0",
	"0                                                     w                           w                       w                          w               0",
	"0                                                                                                                 w                                w 0",
	"0         w                   w             w                w        w                            w                                                 0",
	"0                                                                            w                   w  rrr rrrrr   r   r   r rrrr r   r    w   r        0",
	"0                             w                                                       w             rr    r    r r  rr  r r  r r   r       r r       0",
	"0                   w                                  w                   w                      w  rr   r   r r r r r r r wr  r r   w   r r r      0",
	"0                                      w                                                            rrr   r  wr   r r  rr rrrr   r        r   r  r   0",
	"0                                                                                    m                                                       w       0",
	"0       w                                                       w                                     w                            w                 0",
	"0                      m                                                       w                                     w        m                 s    0",
	"0               m                                                                   t0                                                          s    0",
	"0                                        bbb                                       bbbmm                                                        s    0",
	"0                                               bm                            s                                            bbbb                 s    0",
	"0               m    bmbmb                                              s                                                                       s    0",
	"0                                                   bbm                ss            bmmm                                       s       mm    m s    0",
	"0                                                                    sssss                                      s   s           s               s    0",
	"0                          s   s       t0                t0         sssssss   t0          s   s     t0          s   s           s   t0          s    0",
	"GGGGGGGGGGGGGGGGGGGGGGGGGGGGcccGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGcccGGGGGGGGGGGGGGGGGGG   GGGGGGGGGGGGGGGGGGGGGG   GGGGGGGGG",
	"GGGGGGGGGGGGGGGGGGGGGGGGGGGG   GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG   GGGGGGGGGGGGGGGGGGG   GGGGGGGGGGGGGGGGGGGGGG   GGGGGGGGG",
	"GGGGGGGGGGGGGGGGGGGGGGGGGGGG   GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG   GGGGGGGGGGGGGGGGGGG   GGGGGGGGGGGGGGGGGGGGGG   GGGGGGGGG",
};

//class PICTURE
//{
//
//};


class PLAYER
{
private:
	float dx, dy;
	bool onGround;
	float currentFrame;

public:
	FloatRect rect;
	Sprite sprite;

	PLAYER(Texture &image)
	{
		sprite.setTexture(image);
		rect = FloatRect(118 + 18, 35, 16, 28);
		dx = 0.1;
		dy = 0.1;
		currentFrame = 0;
	}

	friend void fightWithEnemy(PLAYER& Mario, ENEMY& enemy);

	void update(float time)
	{
		rect.left += dx * time;
		Collision(0);

		if (!onGround) dy += 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);

		currentFrame += time * 0.005;
		if (currentFrame > 2) currentFrame -= 2;

		if (dx > 0)
			sprite.setTextureRect(IntRect(118 + 18 * int(currentFrame), 35, 16, 28));

		if (dx < 0)
			sprite.setTextureRect(IntRect(118 + 18 * int(currentFrame) + 16, 35, -16, 28));

		sprite.setPosition(rect.left - moveX, rect.top - moveY);

		dx = 0;
	}

	void Collision(int val)
	{
		for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++)
			for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++)
			{
				if ((TileMap[i][j] == 'G') || (TileMap[i][j] == 'b') || (TileMap[i][j] == '0')
					|| (TileMap[i][j] == 's') || (TileMap[i][j] == 't') || (TileMap[i][j] == 'm'))
				{
					if (dy > 0 && val == 1)
					{
						rect.top = i * 16 - rect.height;
						dy = 0;
						onGround = true;
					}
					if (dy < 0 && val == 1)
					{
						rect.top = i * 16 + 16;
						dy = 0;
					}
					if (dx > 0 && val == 0)
					{
						rect.left = j * 16 - rect.width;
					}
					if (dx < 0 && val == 0)
					{
						rect.left = j * 16 + 16;
					}
				}
				if (TileMap[i][j] == 'c')
				{
					TileMap[i][j] == ' ';
				}
			}
	}

	void MarioControlToMoveLeft()//MarioControlToMoveLeft
	{
		if (Keyboard::isKeyPressed(Keyboard::Left))
			dx = -0.1;
	}

	void MarioControlToMoveRight()
	{
		if (Keyboard::isKeyPressed(Keyboard::Right))
			dx = +0.1;
	}

	void MarioControlToMoveUp()
	{
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (onGround)
			{
				dy = -0.27;
				onGround = false;
			}
		}
	}
};


class ENEMY
{
private:
	float dx, dy;
	float currentFrame;
	bool life;
public:
	FloatRect rect;
	Sprite sprite;

	void set(Texture& image, int x, int y)
	{
		sprite.setTexture(image);
		rect = FloatRect(x, y, 16, 16);
		dx = 0.05;
		currentFrame = 0;
		life = true;
	}

	friend void fightWithEnemy(PLAYER& Mario, ENEMY& enemy);

	void update(float time)
	{
		rect.left += dx * time;

		Collision();

		currentFrame += time * 0.005;

		if (currentFrame > 2)
			currentFrame -= 2;

		sprite.setTextureRect(IntRect(70 + 16 * int(currentFrame), 0, 16, 16));

		if (!life)
			sprite.setTextureRect(IntRect(85, 0, 16, 16));

		sprite.setPosition(rect.left - moveX, rect.top - moveY);
	}

	void Collision()
	{
		for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++)
			for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++)
			{
				if ((TileMap[i][j] == 'G') || (TileMap[i][j] == '0') || (TileMap[i][j] == 's'))
				{
					if (dx > 0)
					{
						rect.left = j * 16 - rect.width; dx *= -1;
					}
					else if (dx < 0)
					{
						rect.left = j * 16 + 16;         dx *= -1;
					}
				}
			}
	}
};

void fightWithEnemy(PLAYER& Mario, ENEMY& enemy)
{
	if (Mario.rect.intersects(enemy.rect))
	{
		if (enemy.life)
		{
			if (Mario.dy > 0)
			{
				Mario.dy = -0.2;
				enemy.dx = 0;
				enemy.life = false;
			}
			else
			{
				Sleep(500);
				exit(1);
			}
		}
	}
}

int main()
{
	RenderWindow window(VideoMode(800, 350), "Mario!");//SFML works!

	Texture tileSet;
	tileSet.loadFromFile("mario_new.png");

	Sprite tile(tileSet);
	Clock clock;
	Event event;

	PLAYER Mario(tileSet);

	//vector<ENEMY*>enemy;
	//vector<ENEMY*>::iterator it;
	//enemy.push_back(new ENEMY->set(tileSet, 48 * 16, 18 * 16));
	//enemy.push_back(new ENEMY->set(tileSet, 16 * 16, 18 * 16));

	ENEMY  enemy;
	ENEMY  enemy1;
	ENEMY  enemy2;
	ENEMY  enemy3;
	ENEMY  enemy4;
	ENEMY  enemy5;
	ENEMY  enemy6;

	// координаты врагов
	enemy.set(tileSet, 48 * 16, 18 * 16);
	enemy1.set(tileSet, 16 * 16, 18 * 16);
	enemy2.set(tileSet, 85 * 16, 18 * 16);
	enemy3.set(tileSet, 105 * 16, 18 * 16);
	enemy4.set(tileSet, 125 * 16, 18 * 16);
	enemy5.set(tileSet, 115 * 16, 15 * 16);
	enemy6.set(tileSet, 40 * 16, 16 * 16);

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 500;  // регулируем скорость игры

		if (time > 20)
		{
			time = 20;
		}

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		// движение Марио
		Mario.MarioControlToMoveLeft();
		Mario.MarioControlToMoveRight();
		Mario.MarioControlToMoveUp();

		Mario.update(time);
		enemy.update(time);
		enemy1.update(time);
		enemy2.update(time);
		enemy3.update(time);
		enemy4.update(time);
		enemy5.update(time);
		enemy6.update(time);

		fightWithEnemy(Mario, enemy);
		fightWithEnemy(Mario, enemy1);
		fightWithEnemy(Mario, enemy2);
		fightWithEnemy(Mario, enemy3);
		fightWithEnemy(Mario, enemy4);
		fightWithEnemy(Mario, enemy5);
		fightWithEnemy(Mario, enemy6);

		// движение карты
		if (Mario.rect.left > 400)
		{
			moveX = Mario.rect.left - 400;
		}

		window.clear(Color(107, 140, 255));

		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
			{
				if (TileMap[i][j] == 'G')
					tile.setTextureRect(IntRect(0, 0, 16, 16));// ground

				if (TileMap[i][j] == 'b')
					tile.setTextureRect(IntRect(63, 17, 16, 16));// brick

				if (TileMap[i][j] == 'm')
					tile.setTextureRect(IntRect(48, 16, 16, 16));// money

				if (TileMap[i][j] == 'w')
					tile.setTextureRect(IntRect(0, 47 - 8, 32 + 16, 95 - 47));// w-cloud

				if (TileMap[i][j] == 'c')
					tile.setTextureRect(IntRect(130, 0, 16, 16 / 2));// dangerous bridge

				if (TileMap[i][j] == 'r')
					tile.setTextureRect(IntRect(32, 0, 16, 16));//rock

				if (TileMap[i][j] == 't')
					tile.setTextureRect(IntRect(42, 42, 38, 16));// truba

				if (TileMap[i][j] == 's')
					tile.setTextureRect(IntRect(32, 0, 16, 16));// stone

				if ((TileMap[i][j] == ' ') || (TileMap[i][j] == '0')) continue;

				tile.setPosition(j * 16 - moveX, i * 16 - moveY);
				window.draw(tile);
			}

		window.draw(Mario.sprite);
		window.draw(enemy.sprite);
		window.draw(enemy1.sprite);
		window.draw(enemy2.sprite);
		window.draw(enemy3.sprite);
		window.draw(enemy4.sprite);
		window.draw(enemy5.sprite);
		window.draw(enemy6.sprite);

		window.display();
	}
}