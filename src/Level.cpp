#include "Level.h"

Level::Level(Audio& audio, int levelNum) :
	m_player1(sf::Vector2f(1120.0f, 540.0f), "left"),
	m_player2(sf::Vector2f(160.0f, 540.0f), "right"),
	m_audioRef(audio),
	m_levelNumber(levelNum),
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

	//Setup our torch animations
	setupAnimations();
}

void Level::update()
{
	//Update and animate our torch and torch lights
	for (auto i = 0; i < m_torchAnimators.size(); i++)
	{
		//Update and animate our torches
		m_torchAnimators.at(i)->update(m_torchClocks.at(i).restart());
		m_torchAnimators.at(i)->animate(m_torchSprites.at(i));

		//Update and animate our torche lights
		m_torchLightAnimators.at(i)->update(m_torchLightClocks.at(i).restart());
		m_torchLightAnimators.at(i)->animate(m_torchLightSprites.at(i));
	}

	if (m_loopClock.getElapsedTime().asSeconds() > .4f)
	{
		for each (auto& animator in m_torchAnimators)
			animator->play() << "torch"; //play our torch animation

		for each (auto& animator in m_torchLightAnimators)
			animator->play() << "torchLight"; //play our torch light animation

		m_loopClock.restart();
	}

	//update our players
	m_player1.update();
	m_player2.update();
}

void Level::render(sf::RenderWindow & window)
{
	m_overlayTexture.clear(sf::Color(50, 50, 50, 255));
	m_overlayTexture.display();

	//Draw our player + sword lights and also our torches
	m_overlayTexture.draw(m_player1.getLight());
	m_overlayTexture.draw(m_player2.getLight());
	m_overlayTexture.draw(m_player1.getSwordLight());
	m_overlayTexture.draw(m_player2.getSwordLight());
	//Drawing our torch lights onto our overlay
	for each (auto light in m_torchLightSprites)
		m_overlayTexture.draw(light);
	m_overlayTexture.display();

	window.draw(m_bg); //draw the background

	//Rendering our floor
	for each (auto tile in m_floorSprites)
		window.draw(tile);

	//Rendering our torches
	for each (auto torch in m_torchSprites)
		window.draw(torch);

	m_player1.render(window); //draw the first player	
	m_player2.render(window); //draw the second player

	//Blend our lights into our overlay
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
	auto floorData = m_levelLoader.data()["Level " + std::to_string(m_levelNumber)]["Floors"];

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
		//If our bool has changed and our floor has a body then create our floor
		if (createFloor && floorData.at(i)["HasBody"])
		{
			//Get the length of the created floor
			auto floorLength = 50 * floorData.at(i)["TileAmount"];
			//We create an obstacle (a box2d object) with the specified position and size and push it to our floor vector
			m_floors.push_back(Obstacle(sf::Vector2f(floorData.at(i)["StartX"] - 25 + floorLength / 2.0f, floorData.at(i)["posY"]), sf::Vector2f(floorLength, 50)));
		}
	}
}

void Level::setupAnimations()
{
	auto torchData = m_levelLoader.data()["Level " + std::to_string(m_levelNumber)]["Torches"];

	//the size of one frame in our spritesheet is 400 x 55 pixels
	auto torchSourceSize = sf::Vector2i(14, 30);
	auto torchLightSize = sf::Vector2i(200, 200);

	auto startRect = sf::IntRect(0, 0, torchSourceSize.x, torchSourceSize.y); //the first frame in our torch spritesheet
	auto startRectLight = sf::IntRect(0, 0, torchLightSize.x, torchLightSize.y); //the first frame in our light spritesheet
	
	//Loop for each torch in our json data
	for (auto i = 0; i < torchData.size(); i++)
	{
		
		//thor::Animator<sf::Sprite, std::string> torchA(m_animationHolder), lightA(m_animationHolder);
		sf::Clock torchClock, lightClock;
		sf::Sprite torch, light;

		torch.setTexture(resourceManager.getTextureHolder()["torch"]);
		torch.setOrigin(torch.getLocalBounds().left + 7.5, torch.getLocalBounds().top + torch.getLocalBounds().height / 2.0f); //setting the origin of our torch to the center of our texture
		torch.setTextureRect(startRect);
		torch.setPosition(torchData.at(i)["PosX"], torchData.at(i)["PosY"]);

		light.setTexture(resourceManager.getTextureHolder()["torchLight"]);
		light.setOrigin(light.getLocalBounds().left + 100, light.getLocalBounds().top + light.getLocalBounds().height / 2.0f); //setting the origin of our torch to the center of our texture
		light.setTextureRect(startRectLight);
		light.setPosition(torchData.at(i)["PosX"], torchData.at(i)["PosY"]);

		//Push back our sprites to our sprite holders and add our animation clocks to our vectors
		//Along with our animators for each torch and light
		m_torchSprites.push_back(torch);
		m_torchLightSprites.push_back(light);
		m_torchClocks.push_back(torchClock);
		m_torchLightClocks.push_back(lightClock);
		m_torchAnimators.push_back(new thor::Animator<sf::Sprite, std::string>(m_animationHolder));
		m_torchLightAnimators.push_back(new thor::Animator<sf::Sprite, std::string>(m_animationHolder));
	}

	//loop for 6 frames
	for (int i = 0; i < 6; i++)
	{
		auto frame = sf::IntRect(0 + (torchSourceSize.x * i), 0, torchSourceSize.x, torchSourceSize.y);
		m_torchAnimation.addFrame(1.f, frame); //add a frame to our torch animation

		frame = sf::IntRect(0 + (torchLightSize.x * i), 0, torchLightSize.x, torchLightSize.y);
		m_torchLightAnimation.addFrame(1.f, frame); //add a frame to our torch animation
	}

	m_animationHolder.addAnimation("torch", m_torchAnimation, sf::seconds(.4f));
	m_animationHolder.addAnimation("torchLight", m_torchLightAnimation, sf::seconds(.4f));
}
