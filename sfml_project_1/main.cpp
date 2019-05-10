#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

struct Borders
{
	Borders(int _left, int _top, int _right, int _bottom) : left(_left), top(_top), right(_right), bottom(_bottom)
	{
	}

	int left, top, right, bottom;
};


class Entity
{
public:
	virtual void draw() = 0; //Отрисовка объектов
	virtual void process() = 0; //Обработка логики объектов
	virtual void processEvents() = 0; //Логика обработки событий системы
	virtual Borders getBorders() = 0; //Функция возвращает границы объекта
};

class Grass : public Entity
{
	sf::RenderWindow& window;
	sf::Texture texture;
	sf::RectangleShape shape;
	float coefGrassZoom;
	float grassWidth;
	float сoefGrassWidth;
	int numberGrassInWidth;
	float grassSpeed;

public:

	Grass(sf::RenderWindow& window) : window(window)
	{
		texture.loadFromFile("imgs/grass-1.png");
		texture.setRepeated(true);

		shape.setSize(sf::Vector2f(window.getSize().x * 3, 80));
		shape.setTexture(&texture);
		shape.setPosition(0, window.getSize().y - shape.getSize().y + 5);


		coefGrassZoom = 988 / 80;
		grassWidth = 3227 / coefGrassZoom;
		сoefGrassWidth = window.getSize().x * 3 / grassWidth;
		numberGrassInWidth = window.getSize().x / grassWidth;
		grassSpeed = 2;

		shape.setTextureRect(sf::IntRect(0, 0, 3227 * сoefGrassWidth, 988));
	}

	virtual Borders getBorders()
	{
		return Borders(
			shape.getPosition().x,
			shape.getPosition().y,
			shape.getPosition().x + shape.getSize().x,
			shape.getPosition().y + shape.getSize().y
		);
	}

	virtual void draw()
	{
		window.draw(shape);
	}

	virtual void process()
	{
		if (shape.getPosition().x < -(numberGrassInWidth + 1) * grassWidth)
		{
			shape.setPosition(0, shape.getPosition().y);
		}

		shape.move(-grassSpeed, 0);
	}

	virtual void processEvents()
	{
	}
};

class FPS : public Entity
{
	sf::RenderWindow& window;
	sf::Text text;
	sf::Clock clock;
	int fps;
	int countFps;
	unsigned int size;
public:

	FPS(sf::RenderWindow& window, sf::Font& font) : window(window)
	{
		fps = 0;
		countFps = 0;
		size = 20;
		text.setFont(font);
		text.setFillColor(sf::Color(255, 166, 0));
		text.setCharacterSize(size);
	}


	virtual Borders getBorders()
	{
		return Borders(
			text.getPosition().x,
			text.getPosition().y,
			text.getPosition().x + text.getLineSpacing(),
			text.getPosition().y + text.getCharacterSize()
		);
	}

	virtual void draw()
	{
		window.draw(text);
	}

	virtual void process()
	{
		sf::Time elapsed = clock.getElapsedTime();
		if (elapsed.asMilliseconds() >= 1000)
		{
			fps = countFps;
			countFps = -1;
			clock.restart();
		}
		text.setString("FPS: " + std::to_string(fps));

		countFps++;
	}

	virtual void processEvents()
	{
	}
};

class Column : public Entity
{
	sf::RenderWindow& window;
	sf::RectangleShape shapeUp;
	sf::RectangleShape shapeDown;
	sf::Texture texture;

	int columnWidth;
	int columnHeightSpace;
	int speed;
	int spaceX;


public:

	static int columnCountProgress;

	Column(sf::RenderWindow& window, int positionX, int spaceX) : window(window), spaceX(spaceX)
	{
		columnWidth = 100;
		columnHeightSpace = 150;
		speed = 2;

		texture.loadFromFile("imgs/column.png");

		shapeDown.setPosition(positionX, std::rand() % 400 + 200);
		shapeDown.setSize(sf::Vector2f(columnWidth, window.getSize().y - shapeDown.getPosition().y));
		shapeDown.setTexture(&texture);

		shapeUp.setPosition(shapeDown.getPosition().x, 0);
		shapeUp.setSize(sf::Vector2f(columnWidth, shapeDown.getPosition().y - columnHeightSpace));
		shapeUp.setTexture(&texture);
	}


	virtual Borders getBorders()
	{
		return Borders(
			shapeUp.getPosition().x,
			shapeUp.getSize().y,
			shapeUp.getPosition().x + shapeUp.getSize().x,
			shapeDown.getPosition().y
		);
	}

	virtual void draw()
	{
		window.draw(shapeUp);
		window.draw(shapeDown);
	}

	virtual void process()
	{
		if (shapeDown.getPosition().x + shapeDown.getSize().x > 0)
		{
			shapeDown.setPosition(shapeDown.getPosition().x - speed, shapeDown.getPosition().y);
			shapeUp.setPosition(shapeDown.getPosition().x - speed, shapeUp.getPosition().y);
		}
		else
		{
			shapeDown.setPosition(window.getSize().x + spaceX, std::rand() % 400 + 200);
			shapeDown.setSize(sf::Vector2f(columnWidth, window.getSize().y - shapeDown.getPosition().y));

			shapeUp.setPosition(shapeDown.getPosition().x - speed, shapeUp.getPosition().y);
			shapeUp.setSize(sf::Vector2f(columnWidth, shapeDown.getPosition().y - columnHeightSpace));

			columnCountProgress++;
		}
	}

	virtual void processEvents()
	{
	}
};

int Column::columnCountProgress = 0; // Зануляем счетчик пройденных колонн

class Statistic : public Entity
{
	sf::RenderWindow& window;
	sf::Text text;
	unsigned int size;
public:

	Statistic(sf::RenderWindow& window, sf::Font& font) : window(window)
	{
		size = 20;
		text.setFont(font);
		text.setFillColor(sf::Color(255, 166, 0));
		text.setCharacterSize(size);
		text.setPosition(0, 50);
	}


	virtual Borders getBorders()
	{
		return Borders(
			text.getPosition().x,
			text.getPosition().y,
			text.getPosition().x + text.getLineSpacing(),
			text.getPosition().y + text.getCharacterSize()
		);
	}

	virtual void draw()
	{
		window.draw(text);
	}

	virtual void process()
	{
		text.setString("COUNT: " + std::to_string(Column::columnCountProgress));
	}

	virtual void processEvents()
	{
	}
};


class Bird : public Entity
{
	sf::RenderWindow& window;
	std::vector<Entity*>& entities;
	sf::Event& event;
	sf::Texture texture;
	sf::RectangleShape shape;
	int birdIndex;
	int birdSpeed;

	sf::Music musicCaput;
	

	sf::IntRect getTextureRectForBird(int index)
	{
		int row = index / 5;
		int column = index % 5;
		return sf::IntRect(178 * column, 166 * row, 180, 166);
	}

	void animation()
	{
		shape.setTextureRect(getTextureRectForBird(birdIndex / 2));
		birdIndex++;

		if (birdIndex > 13 * 2)
		{
			birdIndex = 0;
		}
	}

public:

	Bird(sf::RenderWindow& window, sf::Event& event, std::vector<Entity*>& entities)
		: window(window), event(event), entities(entities)
	{
		texture.loadFromFile("imgs/sprite-bird-animated.png");
		musicCaput.openFromFile("audio/caput.wav");
	

		shape.setSize(sf::Vector2f(60, 60));
		shape.setTexture(&texture);

		shape.setPosition(window.getSize().x / 2 - 75, window.getSize().y / 2 - 75);

		birdIndex = 0;
		birdSpeed = 30;
	}

	virtual Borders getBorders()
	{
		return Borders(
			shape.getPosition().x,
			shape.getPosition().y,
			shape.getPosition().x + shape.getSize().x,
			shape.getPosition().y + shape.getSize().y
		);
	}

	virtual void draw()
	{
		window.draw(shape);
	}


	virtual void process()
	{
		animation();
		Borders birdBorders = getBorders();
		for (int i = 0; i < entities.size(); i++)
		{
			if (dynamic_cast<Column*>(entities[i]))
			{
				Borders spaceInColumn = entities[i]->getBorders();

				if (
					(
						((birdBorders.right >= spaceInColumn.left) && (birdBorders.right <= spaceInColumn.right)) ||
						((birdBorders.left >= spaceInColumn.left) && (birdBorders.left <= spaceInColumn.right))
					) && (
						(birdBorders.top <= spaceInColumn.top) || (birdBorders.bottom >= spaceInColumn.bottom)
					)
				)
				{
					//std::cout << "BAM" << std::endl;
					musicCaput.stop();
					musicCaput.play();
						
				}
			}
		}
	}

	virtual void processEvents()
	{
		if (event.KeyPressed && (event.key.code == sf::Keyboard::W))
		{
			if (shape.getPosition().y > 0)
			{
				shape.move(0, -birdSpeed);
			}
			else
			{
				shape.setPosition(shape.getPosition().x, 0);
			}
		}
		if (event.KeyPressed && (event.key.code == sf::Keyboard::S))
		{
			if (shape.getPosition().y + shape.getSize().y < window.getSize().y)
			{
				shape.move(0, birdSpeed);
			}
			else
			{
				shape.setPosition(shape.getPosition().x, window.getSize().y - shape.getSize().y);
			}
		}
		if (event.KeyPressed && (event.key.code == sf::Keyboard::A))
		{
			if (shape.getPosition().x > 0)
			{
				shape.move(-birdSpeed, 0);
			}
			else
			{
				shape.setPosition(0, shape.getPosition().y);
			}
		}
		if (event.KeyPressed && (event.key.code == sf::Keyboard::D))
		{
			if (shape.getPosition().x + shape.getSize().x < window.getSize().x)
			{
				shape.move(birdSpeed, 0);
			}
			else
			{
				shape.setPosition(window.getSize().x - shape.getSize().x, shape.getPosition().y);
			}
		}
	}
};


int main()
{
	//Скрыть консоль
	//FreeConsole(); 

	//Создаем окно
	int HEIGHT = 800;
	int WIDTH = 1200;
	int FPS_MAX = 60;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
	window.setFramerateLimit(FPS_MAX);

	sf::Event event; //Событие системы

	std::vector<Entity*> entities;

	sf::Font fontLight, fontBold, fontRegular;

	if (fontLight.loadFromFile("fonts/Roboto-Light-webfont.woff"))
		std::cout << "Font Light was downloaded!" << std::endl;

	if (fontBold.loadFromFile("fonts/Roboto-Bold-webfont.woff"))
		std::cout << "Font Bold was downloaded!" << std::endl;

	if (fontRegular.loadFromFile("fonts/Roboto-Regular-webfont.woff"))
		std::cout << "Font Regular was downloaded!" << std::endl;


	//Птичка
	entities.push_back(new Bird(window, event, entities));


	//Колонны

	entities.push_back(new Column(window, window.getSize().x + 300 * 0, 200));
	entities.push_back(new Column(window, window.getSize().x + 300 * 1, 200));
	entities.push_back(new Column(window, window.getSize().x + 300 * 2, 200));
	entities.push_back(new Column(window, window.getSize().x + 300 * 3, 200));
	entities.push_back(new Column(window, window.getSize().x + 300 * 4, 200));


	//Травка
	entities.push_back(new Grass(window));


	//FPS
	entities.push_back(new FPS(window, fontRegular));

	//Statistic
	entities.push_back(new Statistic(window, fontRegular));


	sf::Music music;
	if (!music.openFromFile("audio/1.wav"))
		return -1; // error
	music.play();
	


	//Основной цикл приложения 
	while (window.isOpen())
	{
		//Цикл обработки событий системы
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//Логика обработки событий
			for (int i = 0; i < entities.size(); i++)
			{
				entities[i]->processEvents();
			}
		}

		//Начало блока логики программы
		for (int i = 0; i < entities.size(); i++)
		{
			entities[i]->process();
		}
		//Конец блока логики программы

		//Очистка экрана, путем заливки цветом
		window.clear(sf::Color::White);

		//Рисование
		for (int i = 0; i < entities.size(); i++)
		{
			entities[i]->draw();
		}

		//Показать все отрисованные элементы
		window.display();
	}
	return 0;
}
