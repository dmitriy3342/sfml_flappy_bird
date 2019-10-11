#include <SFML/Graphics.hpp>
#include <iostream>



class Entity
{
public:
	virtual void draw() = 0; //Отрисовка объектов
	virtual void process() = 0; //Обработка логики объектов
	virtual void processEvents(sf::Event event) = 0; //Логика обработки событий системы
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

	virtual void processEvents(sf::Event event)
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

	virtual void processEvents(sf::Event event)
	{
	}
};



class Bird : public Entity
{
	sf::RenderWindow& window;
	sf::Texture texture;
	sf::RectangleShape shape;
	int birdIndex;
	int birdSpeed;


	sf::IntRect getTextureRectForBird(int index)
	{
		int row = index / 5;
		int column = index % 5;
		return sf::IntRect(178 * column, 166 * row, 180, 166);
	}

	void animation()
	{

	}

public:

	Bird(sf::RenderWindow& window)
		: window(window)
	{
		texture.loadFromFile("imgs/sprite-bird-animated.png");


		shape.setSize(sf::Vector2f(100, 100));
		shape.setTexture(&texture);

		shape.setPosition(window.getSize().x / 2 - 75, window.getSize().y / 2 - 75);

		birdIndex = 0;
		birdSpeed = 30;
	}


	virtual void draw()
	{
		window.draw(shape);
	}


	virtual void process()
	{
		shape.setTextureRect(getTextureRectForBird(birdIndex / 2));
		birdIndex++;

		if (birdIndex > 13 * 2)
		{
			birdIndex = 0;
		}
	}

	virtual void processEvents(sf::Event event)
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
	entities.push_back(new Bird(window));



	//Травка
	entities.push_back(new Grass(window));


	//FPS
	entities.push_back(new FPS(window, fontRegular));






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
				entities[i]->processEvents(event);
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
