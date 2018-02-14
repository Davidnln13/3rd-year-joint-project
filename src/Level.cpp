#include "Level.h"

Level::Level(Audio& audio, int levelNum) :
	m_player1(sf::Vector2f(1120.0f, 540.0f), "left"),
	m_player2(sf::Vector2f(160.0f, 540.0f), "right"),
	m_audioRef(audio),
	m_levelNumber(levelNum),
	m_levelLoader(),
	m_hasTimeLimit(false),
	m_hasKillLimit(false),
	m_gameOver(false)
{
	//Set up the contact listener for box2d
	world.SetContactListener(&m_contactListener);
	//Set pointers to our player objects in our contact listener
	m_contactListener.setPlayers(m_player1, m_player2);

	//Set the texture of our sprite and place it at 0,0
	m_bg.setTexture(resourceManager.getTextureHolder()["castleBG"]);
	m_bg.setPosition(0, 0);

	//setting the parameters of our dark overlay, we will draw lights onto this render texture
	m_overlayTexture.create(1280, 720);
	m_overlaySprite.setTexture(m_overlayTexture.getTexture());
	m_overlaySprite.setPosition(0, 0);

	m_particleSys = new ParticleSystem(0, 50, sf::Vector2f(640, 360));
}

void Level::update()
{
	m_particleSys->update();

	if (m_gameOver)
	{
		// do something
	}

	//If there is a time limit and the game is not over
	if (false == m_gameOver)
	{
		//if the time gone since our clock was started (restart()) then set our game over to true
		if (m_hasTimeLimit && m_timeLimitClock.getElapsedTime().asSeconds() >= m_timeLimit)
			m_gameOver = true;
		//if there is a kill limit on the game and either player has reached 0 then end the game
		if (m_hasKillLimit && m_player1.lives() == 0 || m_hasKillLimit && m_player2.lives() == 0)
		{
			m_gameOver = true;
		}
	}
	
	//Update and animate our torch and torch lights
	for (auto i = 0; i < m_torchAnimators.size(); i++)
	{
		//Update and animate our torches
		m_torchAnimators.at(i)->update(m_torchClocks.at(i).restart());
		m_torchAnimators.at(i)->animate(m_torchSprites.at(i));

		//Update and animate our torch lights
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

	//Checks if we need to respawn any players, and if so, respawn them as far away from the other player as possible
	checkForRespawn(m_player1, m_player2);
	checkForRespawn(m_player2, m_player1);
}

void Level::render(sf::RenderWindow & window)
{
	m_overlayTexture.clear(sf::Color(50, 50, 50, 255));
	m_overlayTexture.display();

	//Draw our player + sword lights
	m_overlayTexture.draw(m_player1.getLight());
	m_overlayTexture.draw(m_player2.getLight());
	m_overlayTexture.draw(m_player1.getSwordLight());
	m_overlayTexture.draw(m_player2.getSwordLight());
	//Drawing our torch lights onto our overlay
	for each (auto& light in m_torchLightSprites)
		m_overlayTexture.draw(light);

	m_overlayTexture.display();

	window.draw(m_bg); //draw the background

	//Rendering our floor
	for each (auto& tile in m_floorSprites)
		window.draw(tile);

	//Rendering our windows
	for each(auto& win in m_windowSprites)
		window.draw(win);

	//Rendering our torches
	for each (auto& torch in m_torchSprites)
		window.draw(torch);


	m_player1.render(window); //draw the first player	
	m_player2.render(window); //draw the second player

	m_particleSys->draw(window);

	//Blend our lights into our overlay
	window.draw(m_overlaySprite, sf::BlendMultiply);



}

std::string Level::handleInput(JoystickController & controller1, JoystickController & controller2)
{
	if (false == m_gameOver)
	{
		m_player1.handleJoystick(controller1);
		m_player2.handleJoystick(controller2);

		//If any player attacks 
		if ((controller1.isButtonPressed("X") && m_player1.getCanAttack() == true) || (controller2.isButtonPressed("X") && m_player2.getCanAttack() == true))
		{
			m_audioRef.m_soundMap["SwordSwing"].play();
		}

		if(controller1.isButtonPressed("RightThumbClick"))
			m_particleSys = new ParticleSystem(0, 75, sf::Vector2f(640, 360));

		return "play game";
	}
	else
	{
		return "play game"; //Change this to end game screen afterwards
	}
}

void Level::setUpLevel(std::string levelName)
{
	auto floorData = m_levelLoader.data()[levelName]["Floors"];
	auto startPoints = m_levelLoader.data()[levelName]["Start Points"];

	//set our players spawn point
	m_player1.spawnPlayer(startPoints.at(0)["x"], startPoints.at(0)["y"], startPoints.at(0)["facingLeft"]);
	m_player2.spawnPlayer(startPoints.at(1)["x"], startPoints.at(1)["y"], startPoints.at(1)["facingLeft"]);


	for (int i = 0; i < floorData.size(); i++)
	{
		bool createFloor = false;
		int startX = floorData.at(i)["StartX"]; // Store the X value here due to dodgy references in JSON

		for (int j = 0; j < floorData.at(i)["TileAmount"]; j++)
		{
			sf::Sprite tile;
			tile.setTexture(resourceManager.getTextureHolder()["stoneTile"]); 
			tile.setOrigin(tile.getLocalBounds().left + tile.getLocalBounds().width / 2.0f, tile.getLocalBounds().top + tile.getLocalBounds().height / 2.0f);
			tile.setPosition(startX + (50 * j), floorData.at(i)["PosY"]);
			m_floorSprites.push_back(tile);
			createFloor = true; //set our bool to true
		}
		//If our bool has changed and our floor has a body then create our floor
		if (createFloor && floorData.at(i)["HasBody"])
		{
			//Get the length of the created floor
			int floorLength = 50 * floorData.at(i)["TileAmount"];
			//We create an obstacle (a box2d object) with the specified position and size and push it to our floor vector
			m_floors.push_back(Obstacle(sf::Vector2f((startX - 25) + (floorLength / 2.0f), floorData.at(i)["PosY"]), sf::Vector2f(floorLength, 50)));
		}
	}


	//Setup our window sprites for our chosen level
	auto windowData = m_levelLoader.data()[levelName]["Windows"];

	//Loop through our window data
	for (int i = 0; i < windowData.size(); i++)
	{
		sf::Sprite window;
		window.setTexture(resourceManager.getTextureHolder()["windowTile"]);
		window.setOrigin(window.getLocalBounds().left + window.getLocalBounds().width / 2.0f, window.getLocalBounds().top + window.getLocalBounds().height / 2.0f);
		window.setPosition(windowData.at(i)["x"], windowData.at(i)["y"]);
		m_windowSprites.push_back(window);
	}

	//Setup our spawn points for our chosen level
	auto spawnData = m_levelLoader.data()[levelName]["Spawn Points"];

	//Loop through our spawn points data
	for (int i = 0; i < spawnData.size(); i++)
	{
		m_spawnPoints.push_back(sf::Vector2f(spawnData.at(i)["x"], spawnData.at(i)["y"]));
	}

	//Setup our torch animations
	setupAnimations(levelName);
}

void Level::checkForRespawn(Player& deadPlayer, Player& otherPlayer)
{
	//If the first player is dead then respawn the player at the furthest spawn point from player 2
	if (deadPlayer.dead())
	{
		sf::Vector2f selectedSpawn;
		auto otherPlayerPos = otherPlayer.position(); //get the other players position
		float maxDistance = 0; //max distance from our spawn points

		//Loop through our spawn points
		for (auto& pos : m_spawnPoints)
		{
			auto dist = distance(otherPlayerPos, pos); //get the distance between the other player and the spawn point
			//if the distance bewteen the points is greater than our max distance
			if (dist > maxDistance && pos != deadPlayer.lastSpawnPos())
			{
				selectedSpawn = pos; //Set our spawn point
				maxDistance = dist; //set our max distance
			}
		}

		//Using the ternary operator to determine wheter to spawn the player facing left or not
		bool facingLeft = (selectedSpawn.x > otherPlayerPos.x) ? true : false;

		deadPlayer.setSpawnPoint(selectedSpawn, facingLeft);
	}
}

void Level::setupAnimations(std::string levelName)
{
	auto torchData = m_levelLoader.data()[levelName]["Torches"];

	//the size of one frame
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

void Level::setLevelParameters(int maxKills, int maxTime, int levelNumber, std::map<int, std::string>& levelNames)
{
	std::cout << "Kill limit: " << maxKills << std::endl;
	std::cout << "Time limit: " << maxTime << std::endl;
	std::cout << "Level: " << levelNumber << std::endl;

	m_levelNumber = levelNumber;
	m_timeLimit = maxTime;
	m_killLimit = maxKills;

	//If our time limit is greater than zero then start our clock and set our bool
	if (m_timeLimit > 0)
	{
		//We multiply our time limit by 60 to convert our second to a minute
		m_timeLimit *= 60;
		m_hasTimeLimit = true;
		m_timeLimitClock.restart();
	}
	//If our kill limit is greater than 0 then set our bool to true
	if (m_killLimit > 0)
		m_hasKillLimit = true;

	//Setup our level and pass our level name to our method
	setUpLevel(levelNames[m_levelNumber]);

	m_player1.setParameters(m_killLimit);
	m_player2.setParameters(m_killLimit);
}

//Distance between two points formula
float Level::distance(sf::Vector2f a, sf::Vector2f b)
{
	auto xSq = powf(b.x - a.x, 2);
	auto ySq = powf(b.y - a.y, 2);

	//Return our distance
	return sqrtf(xSq + ySq);
}

void Level::clearLevel()
{
	//Clearing our containers to avoid accumulation of data in our containers
	m_spawnPoints.clear();
	m_windowSprites.clear();
	m_floorSprites.clear();
	m_torchAnimators.clear();
	m_torchLightAnimators.clear();
	m_torchSprites.clear();
	m_torchLightSprites.clear();
	m_torchAnimation = thor::FrameAnimation();
	m_torchLightAnimation = thor::FrameAnimation();
	m_animationHolder = thor::AnimationMap<sf::Sprite, std::string>();

	//Remove all our box 2d bodies from our obstacles and clear our floor vector
	for (auto floor : m_floors)
		world.DestroyBody(floor.body()); //Destroy all the obstacle bodies

	m_floors.clear(); //Clear all of the floors
}

sf::Vector2f Level::lerp(sf::Vector2f a, sf::Vector2f b, float t)
{
	float x = (1 - t) * a.x + t*b.x;
	float y = (1 - t) * a.y + t*b.y;

	return sf::Vector2f(x, y);
}
