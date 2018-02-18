#include "Level.h"

Level::Level(Audio& audio, int levelNum) :
	m_player1(audio, sf::Vector2f(1120.0f, 540.0f), "left"),
	m_player2(audio, sf::Vector2f(160.0f, 540.0f), "right"),
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
	m_transitionCol(255, 255, 255, m_transitionAlpha), //Make our transition color white with 0 alpha
	m_blueFlag(resourceManager.getTextureHolder()["Blue Flag"]),
	m_yellowFlag(resourceManager.getTextureHolder()["Yellow Flag"]),
	m_yellowBase(sf::Vector2f(100, 100)),
	m_blueBase(sf::Vector2f(100, 100)),
	m_p1CapLabel("0", sf::Vector2f(-620, 690), resourceManager.getFontHolder()["arialFont"]),
	m_p2CapLabel("0", sf::Vector2f(1280 + 360, 720), resourceManager.getFontHolder()["arialFont"])
{
	//Our players position indicator
	m_p2PosIndSprite.setTexture(resourceManager.getTextureHolder()["Player Pos Indicator"]);
	m_p2PosIndSprite.setOrigin(m_p2PosIndSprite.getGlobalBounds().width / 2.0f, m_p2PosIndSprite.getGlobalBounds().height / 2.0f);
	m_p2PosIndSprite.setColor(sf::Color::Cyan);

	m_p1PosIndSprite.setTexture(resourceManager.getTextureHolder()["Player Pos Indicator"]);
	m_p1PosIndSprite.setOrigin(m_p1PosIndSprite.getGlobalBounds().width / 2.0f, m_p1PosIndSprite.getGlobalBounds().height / 2.0f);
	m_p1PosIndSprite.setColor(sf::Color::Yellow);


	//Set our capture label for our first player
	m_p1CapLabel.setSize(26);
	m_p1CapLabel.setOrigin(sf::Vector2f(0,0));
	m_p1CapLabel.setPosition(sf::Vector2f(-550, 675));
	m_p1CapLabel.setColor(sf::Color::White);

	//Set our capture label for our second player
	m_p2CapLabel.setSize(26);
	m_p2CapLabel.setOrigin(sf::Vector2f(0, 0));
	m_p2CapLabel.setPosition(sf::Vector2f(1960, 675));
	m_p2CapLabel.setColor(sf::Color::White);

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

	//setting the parameters of our dark overlay, we will draw lights onto this render texture
	m_overlayTexture.create(3260, 1440);
	m_overlayTexture.setSmooth(true);

	m_overlay.setTexture(m_overlayTexture.getTexture());
	m_overlay.setOrigin(m_overlay.getGlobalBounds().left + m_overlay.getGlobalBounds().width / 2.0f, m_overlay.getGlobalBounds().top + m_overlay.getGlobalBounds().height / 2.0f);
	m_overlay.setPosition(640, 360);

	//Create our white transition rectangle
	m_transitionRect.setSize(sf::Vector2f(4500, 1440));
	m_transitionRect.setOrigin(m_transitionRect.getGlobalBounds().width / 2.0, m_transitionRect.getGlobalBounds().height / 2.0);
	m_transitionRect.setPosition(640, 360);
	m_transitionRect.setFillColor(m_transitionCol);

	//Setup our animation for our win/lose icons
	setUpAnimation();

	//Setting our base rectangles
	m_yellowBase.setOrigin(50, 50);
	m_yellowBase.setOutlineColor(sf::Color::Yellow);
	m_yellowBase.setOutlineThickness(2);
	m_yellowBase.setFillColor(sf::Color::Transparent);
	m_yellowBase.setPosition(-590, 620);
	m_blueBase.setOrigin(50, 50);
	m_blueBase.setOutlineColor(sf::Color::Cyan);
	m_blueBase.setOutlineThickness(2);
	m_blueBase.setFillColor(sf::Color::Transparent);
	m_blueBase.setPosition(1280 + 640 - 12.5f, 620);

	//Setting up our split screen views
	m_testView.setSize(1280, 720);
	m_testView.zoom(2);
	m_testView.setCenter(640, 360);
	m_bottomView.setSize(1280, 360);
	m_bottomView.setViewport(sf::FloatRect(0, .5f, 1.0, .5f));
	m_bottomView.zoom(1.25f);
	m_topView.setSize(1280, 360);
	m_topView.setViewport(sf::FloatRect(0, 0, 1.0, .5f));
	m_topView.zoom(1.25f);

	//Setting our capture sprites
	m_p1CaptureSprite.setTexture(resourceManager.getTextureHolder()["Capture Indicator"]);
	m_p1CaptureSprite.setPosition(-680, 680);
	m_p2CaptureSprite.setTexture(resourceManager.getTextureHolder()["Capture Indicator"]);
	m_p2CaptureSprite.setPosition(1830, 680);

	m_blueFlagClock.restart();
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
			else if (m_isCtf)
			{
				if (m_player1.captures() > m_player2.captures())
				{
					m_winSprite.setPosition(p1Pos.x, p1Pos.y - 75);
					m_loseSprite.setPosition(p2Pos.x, p2Pos.y - 75);
					m_hasWinner = true;
				}
				else if (m_player2.captures() > m_player1.captures())
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
		//If the gamemode is CTF
		if (m_isCtf)
		{
			//Checks if the player has a flag, if so, set the flags position
			checkFlagPickup(m_player1, m_blueFlag, m_blueFlagClock);	
			checkFlagPickup(m_player2, m_yellowFlag, m_yellowFlagClock);

			//Check if any flags have been reset
			checkFlagReset(m_player1, m_player2, m_yellowFlag, m_yellowBase);
			checkFlagReset(m_player2, m_player1, m_blueFlag, m_blueBase);

			//Check for any flag captures
			checkFlagCapture(m_player1, m_blueFlag, m_yellowBase);
		}


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
	window.clear(sf::Color::Black); //Clear the scree n with a black background

	int drawIters = 1; //How many times to draw our level screen

	for (int i = 0; i < drawIters; i++)
	{
		//if the gameode is CTF draw to our two views
		if (m_isCtf)
		{
			drawIters = 2; //Make it draw our screen twice and set our view each time
			m_viewVector.at(i)->setCenter(m_players.at(i)->position());
			window.setView(*m_viewVector.at(i));
			//window.setView(m_testView); //TEMP!
		}

		//m_overlayTexture.setView(*m_viewVector.at(i));
		m_overlayTexture.clear(sf::Color(50, 50, 50, 225));
		m_overlayTexture.display();

		//Draw our player + sword lights
		drawToOverlay(m_player1.getLight());
		drawToOverlay(m_player2.getLight());
		drawToOverlay(m_player1.getSwordLight());
		drawToOverlay(m_player2.getSwordLight());

		//Drawing our torch lights onto our overlay
		for each (auto& light in m_torchLightSprites)
			drawToOverlay(light);

		m_overlayTexture.display();

		window.draw(m_bg); //draw the background

		//Render our walls
		for each (auto& tile in m_wallSprites)
			window.draw(tile);

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

		//If the gameode is ctf draw our flags and flag bases
		if (m_isCtf)
		{
			m_blueFlag.draw(window);
			m_yellowFlag.draw(window);
			window.draw(m_yellowBase);
			window.draw(m_blueBase);
		}

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

		m_p1CapLabel.draw(window);
		m_p2CapLabel.draw(window);
		window.draw(m_p1CaptureSprite);
		window.draw(m_p2CaptureSprite);


		//Blend our lights into our overlay
		window.draw(m_overlay, sf::BlendMultiply);

		//Draw our transition rectangle
		window.draw(m_transitionRect);

		if (m_isCtf && i == 1)
			drawPlayerPosIndicator(m_player1, m_player2, m_p2PosIndSprite, window);
		if(m_isCtf && i == 0)
			drawPlayerPosIndicator(m_player2, m_player1, m_p1PosIndSprite, window);
	}

	//Reset our windows view to the defualt view of the window(ie. the one giving to the window when it was created)
	window.setView(window.getDefaultView());
}

void Level::drawToOverlay(sf::Sprite sprite)
{
	auto newSprite = sprite; //Get a copy of the original sprite
	newSprite.setPosition(newSprite.getPosition().x + 990, newSprite.getPosition().y + 360); //Add an offset to it so our texture renders to the overlay at the correct position
	m_overlayTexture.draw(newSprite, sf::BlendAdd); //Draw with the blend add render mode to our overlay texture
}

void Level::drawPlayerPosIndicator(Player & p1, Player & p2, sf::Sprite& indicator, sf::RenderWindow& window)
{
	auto p1Pos = p1.position();
	auto p2Pos = p2.position();
	p1Pos.y = 0;
	p2Pos.y = 0;
	auto xDiff = distance(p1Pos, p2Pos);
	p1Pos = p1.position();
	p2Pos = p2.position();
	p1Pos.x = 0;
	p2Pos.x = 0;
	auto yDiff = distance(p1Pos, p2Pos);

	if (xDiff >= 820 || yDiff >= 260)
	{
		if (xDiff >= 820)
		{
			if (p1.position().x < p2.position().x)
			{
				indicator.setPosition(window.getView().getCenter().x + 640 + 160 - indicator.getGlobalBounds().width / 2.0f, indicator.getPosition().y);
			}
			else if (p1.position().x > p2.position().x)
			{
				indicator.setPosition(window.getView().getCenter().x - 640 - 160 + indicator.getGlobalBounds().width / 2.0f, indicator.getPosition().y);
			}
		}
		else if (xDiff < 820)
		{
			indicator.setPosition(p2.position().x, indicator.getPosition().y);
		}
		if (yDiff >= 260)
		{
			if (p1.position().y < p2.position().y)
			{
				indicator.setPosition(indicator.getPosition().x, window.getView().getCenter().y + 220 - indicator.getGlobalBounds().height / 2.0f);
			}
			else if (p1.position().y > p2.position().y)
			{
				indicator.setPosition(indicator.getPosition().x, window.getView().getCenter().y - 220 + indicator.getGlobalBounds().height / 2.0f);
			}
		}
		else if (yDiff < 260)
		{
			indicator.setPosition(indicator.getPosition().x, p2.position().y);
		}

		//m_p2PosIndSprite.setPosition(window.getView().getCenter().x + 640 + 160 - m_p2PosIndSprite.getGlobalBounds().width / 2.0f, window.getView().getCenter().y);
		indicator.setRotation(angleBetweenVetors(p1.position(), p2.position()) + 90);

		window.draw(indicator);
	}
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

void Level::checkFlagPickup(Player & player, Flag & flag, sf::Clock & flagClock)
{
	//If the player isnt holding a flag
	if (flagClock.getElapsedTime().asSeconds() >= 1 && player.hasFlag() == false)
	{
		if (player.hitBox().getGlobalBounds().intersects(flag.hitBox().getGlobalBounds()))
		{
			flag.setPickedUp(true);
			player.setHasFlag(true);
			std::cout << "Picked up flag" << std::endl;
		}
	}

	else if (player.hasFlag())
	{
		if (player.facingLeft())
			flag.setPosition(player.position().x + 10, player.position().y - 35, 1);

		else
			flag.setPosition(player.position().x - 10, player.position().y - 35, -1);
	}
}

//Checks if a flag has been reset
void Level::checkFlagReset(Player&  player1, Player&  player2, Flag & flag, sf::RectangleShape& base)
{
	if (player2.hasFlag() == false //If player 2 isnt holding player 1's flag
		&& flag.hitBox().getGlobalBounds().intersects(base.getGlobalBounds()) == false //And the flag is not already at the flags base
		&& player1.hitBox().getGlobalBounds().intersects(flag.hitBox().getGlobalBounds())) //And player 1 is intersecting the flag
	{
		//reset the flag
		flag.reset();
	}
}

void Level::checkFlagCapture(Player & player, Flag & flag, sf::RectangleShape & base)
{
	//If the player has a flag and we have dropped the flag at our base, reset the flag
	if (player.hasFlag()
		&& flag.hitBox().getGlobalBounds().intersects(base.getGlobalBounds()))
	{
		player.setHasFlag(false); //Set has flag to flase so the flag does not stick to the playe raftyer it has been captured
		player.captureFlag(); //Increase our capture count

		if (player.getPlayerBody() == m_player1.getPlayerBody())
			m_p1CapLabel.setText(std::to_string(player.captures()), resourceManager.getFontHolder()["arialFont"], sf::Color::White, sf::Vector2f(0, 0));
		else if (player.getPlayerBody() == m_player2.getPlayerBody())
			m_p2CapLabel.setText(std::to_string(player.captures()), resourceManager.getFontHolder()["arialFont"], sf::Color::White, sf::Vector2f(0, 0));

		flag.reset(); //Reset the flag
	}
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
	//Set the players has flag bool to false
	m_player1.setHasFlag(false);
	m_player2.setHasFlag(false);

	//Setup our floors
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

	//Create our walls
	for (int i = 0; i < wallData.size(); i++)
	{
		bool createFloor = false;
		int startY = wallData.at(i)["StartY"]; // Store the Y value here due to weird references in JSON

		for (int j = 0; j < wallData.at(i)["TileAmount"]; j++)
		{
			sf::Sprite tile;
			tile.setTexture(resourceManager.getTextureHolder()["stoneTile"]);
			tile.setOrigin(tile.getLocalBounds().left + tile.getLocalBounds().width / 2.0f, tile.getLocalBounds().top + tile.getLocalBounds().height / 2.0f);
			tile.setPosition(wallData.at(i)["x"], startY + (50 * j));
			m_wallSprites.push_back(tile);
			createFloor = true; //set our bool to true
		}
		//If our bool has changed and our floor has a body then create our floor
		if (createFloor && wallData.at(i)["HasBody"])
		{
			//Get the length of the created floor
			int wallHeight = 50 * wallData.at(i)["TileAmount"];
			//We create an obstacle (a box2d object) with the specified position and size and push it to our floor vector
			m_walls.push_back(Obstacle(sf::Vector2f(wallData.at(i)["x"], (startY - 25) + (wallHeight / 2.0f)), sf::Vector2f(50, wallHeight), "Wall"));
		}
	}

	//If the gamemdoe is CTF set our flags position
	if (m_isCtf)
	{
		auto flagData = m_levelLoader.data()[levelName]["Flag Spawns"];

		m_yellowFlag.startPos(flagData.at(0)["x"], flagData.at(0)["y"]);
		m_blueFlag.startPos(flagData.at(1)["x"], flagData.at(1)["y"]);
	}

	//Set our bg texture
	m_bg = sf::Sprite();
	m_bg.setTexture(resourceManager.getTextureHolder()[levelName + " BG"]);
	m_bg.setOrigin(m_bg.getLocalBounds().width / 2.0f, m_bg.getLocalBounds().height / 2.0f);
	m_bg.setPosition(640, 360);

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
		
		if (deadPlayer.hasFlag())
		{
			if (deadPlayer.getPlayerBody() == m_player1.getPlayerBody())
			{
				m_blueFlag.setPickedUp(false);
				m_blueFlagClock.restart();
			}
			else if (deadPlayer.getPlayerBody() == m_player2.getPlayerBody())
			{
				m_yellowFlag.setPickedUp(false);
				m_yellowFlagClock.restart();
			}
		}
		deadPlayer.setHasFlag(false);
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
	m_p1CapLabel.setText(std::to_string(0), resourceManager.getFontHolder()["arialFont"], sf::Color::White, sf::Vector2f(0, 0));
	m_p2CapLabel.setText(std::to_string(0), resourceManager.getFontHolder()["arialFont"], sf::Color::White, sf::Vector2f(0, 0));


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

	//Reset our flags and player parameters
	m_blueFlag.reset();
	m_yellowFlag.reset();
	m_player1.setHasFlag(false);
	m_player2.setHasFlag(false);

	m_player1.setParameters(m_killLimit);
	m_player2.setParameters(m_killLimit);

	//add the animation to our animation holder with the specified length and name
	m_animationHolder.addAnimation("winAnimation", m_winAnimation, sf::seconds(.25f));
	m_animationHolder.addAnimation("loseAnimation", m_loseAnimation, sf::seconds(.25f));
	m_animationHolder.addAnimation("drawAnimation", m_drawAnimation, sf::seconds(.25f));

	//Stop our animators
	m_winAnimator.stop();
	m_loseAnimator.stop();
	m_draw1Animator.stop();
	m_draw2Animator.stop();

	//reset our win/lose/draw sprite texture rectangles
	m_winSprite.setTextureRect(sf::IntRect(0, 0, 100, 0));
	m_loseSprite.setTextureRect(sf::IntRect(0, 0, 100, 0));
	m_draw1Sprite.setTextureRect(sf::IntRect(0, 0, 100, 0));
	m_draw2Sprite.setTextureRect(sf::IntRect(0, 0, 100, 0));
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
	m_wallSprites.clear();
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

	//Remove all of our wall bodies
	for (auto& wall : m_walls)
		world.DestroyBody(wall.body()); //Destroy all the obstacle bodies

	//Remove all of our killboxes
	for (auto& killBox : m_killBoxes)
		world.DestroyBody(killBox);

	m_killBoxes.clear(); //Clear our killboxes vector
	m_floors.clear(); //Clear all of the floors
	m_walls.clear(); //Clear all of our walls

	m_player1.resetPlayerParameters();
	m_player2.resetPlayerParameters();
}

bool Level::gameOver()
{
	return m_gameOver;
}

float Level::gameOverTime()
{
	if (m_gameOver)
		return m_gameOverClock.getElapsedTime().asSeconds();
	else
		return 0.0f;
}

sf::Vector2f Level::lerp(sf::Vector2f a, sf::Vector2f b, float t)
{
	float x = (1 - t) * a.x + t*b.x;
	float y = (1 - t) * a.y + t*b.y;

	return sf::Vector2f(x, y);
}

float Level::angleBetweenVetors(sf::Vector2f a, sf::Vector2f b)
{

	return thor::toDegree(atan2(b.y - a.y, b.x - a.x));
}
