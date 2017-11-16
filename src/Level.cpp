#include "Level.h"

Level::Level(Audio& audio, sf::Texture& levelBackground) :
	m_player1(sf::Vector2f(1120.0f, 540.0f), "left"),
	m_player2(sf::Vector2f(160.0f, 540.0f), "right"),
	m_floor(sf::Vector2f(640.0f, 695.0f), sf::Vector2f(1280, 50)),
	m_audioRef(audio)
{
	//Set up the contact listener for box2d
	world.SetContactListener(&m_contactListener);
	//Set pointers to our player objects in our contact listener
	m_contactListener.setPlayers(m_player1, m_player2);

	//Set the textur eof our sprite and place it at 0,0
	m_bg.setTexture(levelBackground);
	m_bg.setPosition(0, 0);

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
	m_overlayTexture.display();
	m_overlayTexture.draw(m_player2.getLight());
	m_overlayTexture.display();
	m_overlayTexture.draw(m_player1.getSwordLight());
	m_overlayTexture.display();
	m_overlayTexture.draw(m_player2.getSwordLight());
	m_overlayTexture.display();

	window.draw(m_bg); //draw the background

	//Rendering our floor
	for each (sf::Sprite tile in m_floorSprites)
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
		m_audioRef.m_soundArray[0].play();
	}
}

void Level::setUpFloor()
{
	for (int i = 0; i < 26; i++)
	{
		sf::Sprite floorTile;
		floorTile.setTexture(resourceManager.getTextureHolder()["stoneTile"]);
		floorTile.setOrigin(floorTile.getLocalBounds().left + floorTile.getLocalBounds().width / 2.0f, floorTile.getLocalBounds().top + floorTile.getLocalBounds().height / 2.0f);
		floorTile.setPosition(25 + (50 * i), 695);
		m_floorSprites.push_back(floorTile);
	}
}
