#include "Level.h"

Level::Level(Audio& audio, int levelNum) :
	m_player1(sf::Vector2f(1120.0f, 540.0f), "left"),
	m_player2(sf::Vector2f(160.0f, 540.0f), "right"),
	m_audioRef(audio),
	m_levelNumber(levelNum),
	m_levelLoader(),
	m_hasTimeLimit(false),
	m_hasKillLimit(false),
	m_gameOver(false),
	m_setWinner(false),
	m_hasWinner(false),
	m_playGameOverIndicator(true),
	m_timeLabel("Time Left", sf::Vector2f(595, 40), resourceManager.getFontHolder()["oxinFont"]),
	m_timeLabelValue(": 0", sf::Vector2f(670, 22.5f), resourceManager.getFontHolder()["arialFont"]),
	m_draw1Animator(m_animationHolder),
	m_draw2Animator(m_animationHolder),
	m_winAnimator(m_animationHolder),
	m_loseAnimator(m_animationHolder),
	m_transitionAlpha(0),
	m_transitionCol(255,255,255, m_transitionAlpha) //Make our transition color white with 0 alpha
{
	m_players.push_back(&m_player2);
	m_players.push_back(&m_player1);
	m_viewVector.push_back(&m_bottomView);
	m_viewVector.push_back(&m_topView);

	auto text = m_timeLabelValue.getText();
	m_timeLabelValue.setOrigin(sf::Vector2f(text.getLocalBounds().left, text.getLocalBounds().top));

	//Put all of our labels into our label vector
	m_labels.push_back(&m_timeLabel);
	m_labels.push_back(&m_timeLabelValue);

	//ste teh color of our labesl to white
	for (auto& label : m_labels)
		label->setColor(sf::Color(255,255,255, 255));

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

	//Create our white transition rectangle
	m_transitionRect.setSize(sf::Vector2f(1280, 720));
	m_transitionRect.setPosition(0, 0);
	m_transitionRect.setFillColor(m_transitionCol);

	setUpAnimation();

	m_originalView.setCenter(640, 360);
	m_originalView.setSize(1280, 720);
	m_bottomView.setSize(1280, 360);
	m_bottomView.setViewport(sf::FloatRect(0, .5f, 1.0, .5f));
	m_bottomView.zoom(1.25f);
	m_topView.setSize(1280, 360);
	m_topView.setViewport(sf::FloatRect(0, 0, 1.0, .5f));
	m_topView.zoom(1.25f);
}

void Level::update()
{
	m_contactListener.update(); //Update our contact listener

	if (m_gameOver)
	{
		//When 2.5 seconds has passed since the game has ended and our winner has not been selected, play our indicator animations
		if (m_setWinner == false && m_gameOverClock.getElapsedTime().asSeconds() >= 2.0f)
		{
			auto p1Pos = m_player1.position();
			auto p2Pos = m_player2.position();

			if (m_hasKillLimit || m_hasTimeLimit) //If there was a kill or time limit
			{
				//if player 1 had more kills
				if (m_player1.kills() > m_player2.kills())
				{
					m_winSprite.setPosition(p1Pos.x, p1Pos.y - 75);
					m_loseSprite.setPosition(p2Pos.x, p2Pos.y - 75);
					m_hasWinner = true;
				}
				//if player 2 had more kills
				else if(m_player2.kills() > m_player1.kills())
				{
					m_winSprite.setPosition(p2Pos.x, p2Pos.y - 75);
					m_loseSprite.setPosition(p1Pos.x, p1Pos.y - 75);
					m_hasWinner = true;
				}
				else //Else if the players had the same amount of kills
				{
					m_draw1Sprite.setPosition(p2Pos.x, p2Pos.y - 75);
					m_draw2Sprite.setPosition(p1Pos.x, p1Pos.y - 75);
				}
			}
			m_setWinner = true;
			m_transitionClock.restart();
		}
		else if(m_setWinner) //Else if a winner has been selected
		{
			playAnimation(); //Play our animation

			if (m_transitionClock.getElapsedTime().asSeconds() >= 1.5f)
			{
				//Increase the alpha of our transition
				m_transitionAlpha = lerpValue(m_transitionAlpha, 255, 0.75f);

				//Set the alpha of our color
				m_transitionCol.a = m_transitionAlpha;
				//Set our rectangles color
				m_transitionRect.setFillColor(m_transitionCol);
			}
		}

	}

	//If there is a time limit and the game is not over
	if (false == m_gameOver)
	{
		//if the time gone since our clock was started (restart()) then set our game over to true
		if (m_hasTimeLimit && m_timeLimitClock.getElapsedTime().asSeconds() >= m_timeLimit)
		{
			m_gameOver = true;
			m_gameOverClock.restart();
		}

		//if there is a kill limit on the game and either player has reached 0 then end the game
		if (m_hasKillLimit && m_player1.lives() == 0 || m_hasKillLimit && m_player2.lives() == 0)
		{
			m_gameOver = true;
			m_gameOverClock.restart();
		}

		int timeLeft = m_timeLimit - m_timeLimitClock.getElapsedTime().asSeconds();

		if (timeLeft <= 0)
			timeLeft = 0;

		m_timeLabelValue.setText(std::string(": " + std::to_string(timeLeft)), resourceManager.getFontHolder()["arialFont"], sf::Color::White, sf::Vector2f(0, 0));
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
	int drawIters = 1;

	if (m_isCtf)
	{
		drawIters = 2;
	}




	for (int i = 0; i < drawIters; i++)
	{
		//if the gameode is CTF draw to our two views
		if (m_isCtf)
		{
			m_viewVector.at(i)->setCenter(m_players.at(i)->position());
			window.setView(*m_viewVector.at(i));
		}


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

		//Draw our timer label
		for (auto& label : m_labels)
			label->draw(window);

		//Rendering our torches
		for each (auto& torch in m_torchSprites)
			window.draw(torch);

		m_player1.render(window); //draw the first player	
		m_player2.render(window); //draw the second player

		m_contactListener.draw(window); //Draw our contact listener


		//Draw our game over sprites
		if (m_hasWinner && m_gameOver)
		{
			window.draw(m_winSprite);
			window.draw(m_loseSprite);
		}
		else if (m_gameOver)
		{
			window.draw(m_draw1Sprite);
			window.draw(m_draw2Sprite);
		}

		//Blend our lights into our overlay
		//window.draw(m_overlaySprite, sf::BlendMultiply);

		//Draw our transition rectangle
		window.draw(m_transitionRect);
	}

	window.setView(m_originalView);
}

void Level::playAnimation()
{
	//Animate & Update our sprites
	m_loseAnimator.update(m_loseAnimationClock.restart());
	m_loseAnimator.animate(m_loseSprite);
	m_winAnimator.update(m_winAnimationClock.restart());
	m_winAnimator.animate(m_winSprite);
	m_draw1Animator.update(m_draw1AnimationClock.restart());
	m_draw1Animator.animate(m_draw1Sprite);
	m_draw2Animator.update(m_draw2AnimationClock.restart());
	m_draw2Animator.animate(m_draw2Sprite);

	//If our bool is true
	if (m_playGameOverIndicator)
	{

		m_loseAnimator.play() << "loseAnimation";
		m_winAnimator.play() << "winAnimation";
		m_draw1Animator.play() << "drawAnimation";
		m_draw2Animator.play() << "drawAnimation";

		m_playGameOverIndicator = false;
	}
}

void Level::setUpAnimation()
{
	auto frameSize = sf::Vector2f(100, 50);

	m_winSprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y)); //the first frame in our spritesheet
	m_loseSprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y)); //the first frame in our spritesheet
	m_draw1Sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y)); //the first frame in our spritesheet
	m_draw2Sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y)); //the first frame in our spritesheet

	//Loop for 11 frames
	for (int i = 0; i < 11; i++)
	{
		//add a frame to the animation each loop
		auto frame = sf::IntRect(0 + (frameSize.x * i), 0, frameSize.x, frameSize.y);
		m_drawAnimation.addFrame(0.1f, frame);
		m_winAnimation.addFrame(0.1f, frame);
		m_loseAnimation.addFrame(0.1f, frame);
	}

	//Setting the origin and etxtures of our sprites
	m_winSprite.setTexture(resourceManager.getTextureHolder()["Win Indicator"]);
	m_winSprite.setOrigin(50, 25);
	m_loseSprite.setTexture(resourceManager.getTextureHolder()["Lose Indicator"]);
	m_loseSprite.setOrigin(50, 25);
	m_draw1Sprite.setTexture(resourceManager.getTextureHolder()["Draw Indicator"]);
	m_draw1Sprite.setOrigin(50, 25);
	m_draw2Sprite.setTexture(resourceManager.getTextureHolder()["Draw Indicator"]);
	m_draw2Sprite.setOrigin(50, 25);

	m_winAnimationClock.restart();
	m_loseAnimationClock.restart();
	m_draw1AnimationClock.restart();
	m_draw2AnimationClock.restart();
}

b2Body* Level::createKillBox(float x, float y, float w, float h)
{
	b2Body* body;//the killbox body

	//Setting the killbox properties
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(x / PPM, y / PPM);
	body = world.CreateBody(&bd);

	//Setting the size of our kill box
	b2PolygonShape boxShape;
	boxShape.SetAsBox(w / 2.0f / PPM, h / 2.0f / PPM);

	//Setting the killbox as a sensor
	b2FixtureDef fd;
	fd.shape = &boxShape;
	fd.isSensor = true;
	body->CreateFixture(&fd);
	body->SetUserData("Kill Box");

	return body;
}

std::string Level::handleInput(JoystickController & controller1, JoystickController & controller2)
{
	auto currentScreen = "play game";

	if (false == m_gameOver)
	{
		m_player1.handleJoystick(controller1);
		m_player2.handleJoystick(controller2);

		//If any player attacks 
		if ((controller1.isButtonPressed("X") && m_player1.getCanAttack() == true) || (controller2.isButtonPressed("X") && m_player2.getCanAttack() == true))
		{
			m_audioRef.m_soundMap["SwordSwing"].play();
		}

		return currentScreen;
	}
	else
	{
		if (m_transitionAlpha == 255) //If our white transition alpha is 255 then go to the pre game screen
		{
			currentScreen = "pre game";
		}

		return currentScreen; //Change this to end game screen afterwards
	}
}

void Level::setUpLevel(std::string levelName)
{
	auto floorData = m_levelLoader.data()[levelName]["Floors"];
	auto wallData = m_levelLoader.data()[levelName]["Walls"];
	auto startPoints = m_levelLoader.data()[levelName]["Start Points"];

	//set our players spawn point
	m_player1.spawnPlayer(startPoints.at(0)["x"], startPoints.at(0)["y"], startPoints.at(0)["facingLeft"]);
	m_player2.spawnPlayer(startPoints.at(1)["x"], startPoints.at(1)["y"], startPoints.at(1)["facingLeft"]);

	//Steup our floors
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

	//Setup our killboxes
	auto killBoxData = m_levelLoader.data()[levelName]["Kill Box"];

	for (int i = 0; i < killBoxData.size(); i++)
	{	
		m_killBoxes.push_back(createKillBox(killBoxData.at(i)["PosX"], killBoxData.at(i)["PosY"], killBoxData.at(i)["Width"], killBoxData.at(i)["Height"]));
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
		m_audioRef.m_soundMap["Death"].play();
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

void Level::setLevelParameters(int maxKills, int maxTime, int levelNumber, bool ctf,  std::map<int, std::string>& levelNames)
{
	//Reset our time, isCtf and kill limit booleans
	m_isCtf = ctf;
	m_hasKillLimit = false;
	m_hasTimeLimit = false;
	m_gameOver = false; //resetting our variable
	m_playGameOverIndicator = true;
	m_setWinner = false;
	m_hasWinner = false;

	//Reset our transition
	m_transitionAlpha = 0;
	m_transitionCol.a = m_transitionAlpha;
	m_transitionRect.setFillColor(m_transitionCol);

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

	//add the animation to our animation holder with the specified length and name
	m_animationHolder.addAnimation("winAnimation", m_winAnimation, sf::seconds(.25f));
	m_animationHolder.addAnimation("loseAnimation", m_loseAnimation, sf::seconds(.25f));
	m_animationHolder.addAnimation("drawAnimation", m_drawAnimation, sf::seconds(.25f));
}

//Distance between two points formula
float Level::distance(sf::Vector2f a, sf::Vector2f b)
{
	auto xSq = powf(b.x - a.x, 2);
	auto ySq = powf(b.y - a.y, 2);

	//Return our distance
	return sqrtf(xSq + ySq);
}

float Level::lerpValue(float a, float b, float t)
{
	//If our values are not the same
	if (a != b)
	{
		if (a > b)
		{
			a -= t; //Reduce A by T
			if (a < b) //If A is now less than B, set A to equal to B
				a = b;
		}
		else if (a < b)
		{
			a += t; //Increase A by T
			if (a > b) //If A is now greater than B, set A to equal to B
				a = b;
		}
	}

	return a;
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
	for (auto& floor : m_floors)
		world.DestroyBody(floor.body()); //Destroy all the obstacle bodies

	//Remove all of our killboxes
	for (auto& killBox : m_killBoxes)
		world.DestroyBody(killBox);

	m_killBoxes.clear(); //Clear our killboxes vector
	m_floors.clear(); //Clear all of the floors

	m_player1.resetPlayerParameters();
	m_player2.resetPlayerParameters();
}

bool Level::gameOver()
{
	return m_gameOver;
}

sf::Vector2f Level::lerp(sf::Vector2f a, sf::Vector2f b, float t)
{
	float x = (1 - t) * a.x + t*b.x;
	float y = (1 - t) * a.y + t*b.y;

	return sf::Vector2f(x, y);
}
