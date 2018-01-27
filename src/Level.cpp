#include "Level.h"

Level::Level(Audio& audio) :
	m_player1(sf::Vector2f(1120.0f, 540.0f), "left"),
	m_player2(sf::Vector2f(160.0f, 540.0f), "right"),
	m_audioRef(audio),
	m_levelLoader()
{
	//Set up the contact listener for box2d
	world.SetContactListener(&m_contactListener);
	//Set pointers to our player objects in our contact listener
	m_contactListener.setPlayers(m_player1, m_player2);

	//Set the texture of our sprite and place it at 0,0
	m_bg.setTexture(resourceManager.getTextureHolder()["castleBG"]);
	m_bg.setPosition(0, 0);

	//Setup the floor of our level
	setUpFloor();

	//setting the parameters of our dark overlay, we will draw lights onto this render texture
	m_overlayTexture.create(1280, 720);
	m_overlaySprite.setTexture(m_overlayTexture.getTexture());
	m_overlaySprite.setPosition(0,0);
}

void Level::update()
{
	//update our players
	m_player1.update();
	m_player2.update();
}

void Level::render(sf::RenderWindow & window)
{
	m_overlayTexture.clear(sf::Color(50, 50, 50, 255));
	m_overlayTexture.display();

	//Draw our player and sword lights
	m_overlayTexture.draw(m_player1.getLight());
	m_overlayTexture.draw(m_player2.getLight());
	m_overlayTexture.draw(m_player1.getSwordLight());
	m_overlayTexture.draw(m_player2.getSwordLight());
	m_overlayTexture.display();

	window.draw(m_bg); //draw the background

	//Rendering our floor
	for each (auto tile in m_floorSprites)
		window.draw(tile);

	m_player1.render(window); //draw the first player	
	m_player2.render(window); //draw the second player

	window.draw(m_overlaySprite, sf::BlendMultiply);
}

void Level::handleInput(JoystickController & controller1, JoystickController & controller2)
{
	m_player1.handleJoystick(controller1);
	m_player2.handleJoystick(controller2);

	//If any player attacks 
	if ((controller1.isButtonPressed("X") && m_player1.getCanAttack() == true) || (controller2.isButtonPressed("X") && m_player2.getCanAttack() == true))
	{
		m_audioRef.m_soundMap["SwordSwing"].play();
	}
}

void Level::setUpFloor()
{
	auto floorData = m_levelLoader.data()["Level 1"]["Floors"];

	for (int i = 0; i < floorData.size(); i++)
	{
		bool createFloor = false;

		for (int j = 0; j < floorData.at(i)["TileAmount"]; j++)
		{
			sf::Sprite tile;
			tile.setTexture(resourceManager.getTextureHolder()["stoneTile"]); 
			tile.setOrigin(tile.getLocalBounds().left + tile.getLocalBounds().width / 2.0f, tile.getLocalBounds().top + tile.getLocalBounds().height / 2.0f);
			tile.setPosition(floorData.at(i)["StartX"] + (50 * j), floorData.at(i)["posY"]);
			m_floorSprites.push_back(tile);
			createFloor = true; //set our bool to true
		}
		//If our bool has changed then create our floor
		if (createFloor)
		{
			//Get the length of the created floor
			auto floorLength = 50 * floorData.at(i)["TileAmount"];
			//We create an obstacle (a box2d object) with the specified position and size
			m_floors.push_back(Obstacle(sf::Vector2f(floorLength / 2.0f, floorData.at(i)["posY"]), sf::Vector2f(floorLength, 50)));
		}
	}
}
