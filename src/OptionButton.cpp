#include "OptionButton.h"

OptionButton::OptionButton(sf::Vector2f position, std::string name, sf::Font& font, std::string iconName = "Sword Icon", std::string textureName = "Button SpriteSheet"):
	m_position(position),
	m_name(name),
	m_label(name, m_position, font),
	m_buttonAnimator(m_animationHolder),
	m_iconAnimator(m_animationHolder),
	m_pressed(false),
	m_pressedAnimator(m_animationHolder)
{
	m_sprite.setTexture(resourceManager.getTextureHolder()[textureName]);
	m_sprite.setOrigin(m_sprite.getLocalBounds().left + 200, m_sprite.getLocalBounds().top + m_sprite.getLocalBounds().height / 2.0f); //setting the origin of our button to the center of our texture
	m_sprite.setPosition(m_position);

	try 
	{
		m_icon.setTexture(resourceManager.getTextureHolder()[iconName]);
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	m_icon.setOrigin(m_icon.getLocalBounds().left + 55 / 2.0f, m_icon.getLocalBounds().top + m_icon.getLocalBounds().height / 2.0f); //setting the origin of our button to the center of our texture
	m_icon.setPosition(m_sprite.getGlobalBounds().left + 27.5f, m_position.y);

	m_pressedSprite.setTexture(resourceManager.getTextureHolder()["Button Selected"]);
	m_pressedSprite.setOrigin(m_pressedSprite.getLocalBounds().left + 207.5, m_pressedSprite.getLocalBounds().top + m_pressedSprite.getLocalBounds().height / 2.0f); //setting the origin of our button to the center of our texture
	m_pressedSprite.setPosition(m_position);

	//Setup our select/deselect animations
	setUpAnimation();
}

void OptionButton::update()
{
	//Update and animate our button
	m_buttonAnimator.update(m_animationClock.restart());
	m_buttonAnimator.animate(m_sprite);

	//Update and animate our icon
	m_iconAnimator.update(m_iconAnimationClock.restart());
	m_iconAnimator.animate(m_icon);

	//Update and animate our pressed sprite
	m_pressedAnimator.update(m_pressedAnimationClock.restart());
	m_pressedAnimator.animate(m_pressedSprite);

	if (m_pressed && m_loopClock.getElapsedTime().asSeconds() > 1.0f)
	{
		m_loopClock.restart(); //restart the loop clock
		m_pressedAnimator.play() << "pressed";
	}
}

void OptionButton::render(sf::RenderWindow & window)
{
	if(m_pressed)
		window.draw(m_pressedSprite);
	window.draw(m_sprite);
	window.draw(m_icon);
	m_label.draw(window);
}


void OptionButton::select()
{
	m_label.select();
	m_buttonAnimator.play() << "selected";
	m_iconAnimator.play() << "iconSelected";
	m_sprite.setTextureRect(m_selectedRect);
	m_icon.setTextureRect(m_iconSelectedRect);
	m_selected = true;
}

void OptionButton::deSelect()
{
	m_label.deSelect();
	m_buttonAnimator.play() << "deselected";
	m_iconAnimator.play() << "iconDeselected";
	m_sprite.setTextureRect(m_unselectedRect);
	m_icon.setTextureRect(m_iconUnselectedRect);
	m_selected = false;
}

void OptionButton::pressed(bool isPressed)
{
	//If the bool is true then play our pressed animation
	if (isPressed)
	{
		m_pressed = true;
		m_loopClock.restart(); //start our loop clock
		m_pressedAnimator.play() << "pressed";
	}
	//Else stop oour pressed animation
	else
	{
		m_pressed = false;
		m_pressedAnimator.stop();
	}
}

void OptionButton::setUpAnimation()
{
	//the size of one frame in our spritesheet is 400 x 55 pixels
	auto sourceSize = sf::Vector2i(400, 55);
	//the size of our icon frame size
	auto iconSourceSize = sf::Vector2i(55, 55);
    //The size our one frame in our pressed spritesheet
	auto pressedSourceSize = sf::Vector2i(415, 60);

	m_unselectedRect = sf::IntRect(0, 0, sourceSize.x, sourceSize.y); //the first frame in our spritesheet
	m_iconUnselectedRect = sf::IntRect(0, 0, iconSourceSize.x, iconSourceSize.y); //the first frame of our icon
	m_selectedRect = sf::IntRect(4000, 0, sourceSize.x, sourceSize.y); //the last frame in our animation
	m_iconSelectedRect = sf::IntRect(550, 0, iconSourceSize.x, iconSourceSize.y); //the last frame in our icon animation

	m_sprite.setTextureRect(m_unselectedRect);
	m_icon.setTextureRect(m_iconUnselectedRect);
	m_pressedSprite.setTextureRect(sf::IntRect(0, 0, pressedSourceSize.x, pressedSourceSize.y));

	//loop for 11 frames
	for (int i = 0; i < 11; i++)
	{
		auto frame = sf::IntRect(0 + (sourceSize.x * i), 0, sourceSize.x, sourceSize.y);
		auto oppositeFrame = sf::IntRect(4000 - (sourceSize.x * i), 0, sourceSize.x, sourceSize.y);
		m_selectAnimation.addFrame(1.f, frame); //add a frame to our selected animation
		m_deselectAnimation.addFrame(1.f, oppositeFrame); //add a frae to our deselect animation

		//Setting our icon frame
		frame = sf::IntRect(0 + (iconSourceSize.x * i), 0, iconSourceSize.x, iconSourceSize.y);
		oppositeFrame = sf::IntRect(550 - (iconSourceSize.x * i), 0, iconSourceSize.x, iconSourceSize.y);
		m_iconSelectAnimation.addFrame(1.f, frame);
		m_iconDeselectAnimation.addFrame(1.f, oppositeFrame);

		//Setting our pressed frame
		frame = sf::IntRect(0 + (pressedSourceSize.x * i), 0, pressedSourceSize.x, pressedSourceSize.y);
		m_pressedAnimation.addFrame(1.f, frame);
	}

	//loop for 11 frames
	for (int i = 0; i < 11; i++)
	{
		//Setting our pressed frame
		auto frame = sf::IntRect(4150 - (pressedSourceSize.x * i), 0, pressedSourceSize.x, pressedSourceSize.y);
		m_pressedAnimation.addFrame(1.f, frame);
	}

	m_animationHolder.addAnimation("selected", m_selectAnimation, sf::seconds(0.15f));
	m_animationHolder.addAnimation("deselected", m_deselectAnimation, sf::seconds(0.075f));
	m_animationHolder.addAnimation("iconSelected", m_iconSelectAnimation, sf::seconds(0.15f));
	m_animationHolder.addAnimation("iconDeselected", m_iconDeselectAnimation, sf::seconds(0.075f));
	m_animationHolder.addAnimation("pressed", m_pressedAnimation, sf::seconds(1.0f));
}

void OptionButton::applyAlpha(float value)
{
	auto iColor = m_icon.getColor(); //get the color of the icon
	auto sColor = m_sprite.getColor(); //get the color of the button sprite
	auto tColor = m_label.getText().getFillColor(); //get the color of our text on the button

	//Change the alpha of our sprites and text
	iColor.a = value;
	sColor.a = value;
	tColor.a = value;

	//Set the colour of our sprites and text with their new alpha value
	m_icon.setColor(iColor);
	m_sprite.setColor(sColor);
	m_label.getText().setFillColor(tColor);
}

std::string& OptionButton::getName()
{
	return m_name;
}

bool OptionButton::getSelected()
{
	return m_selected;
}

void OptionButton::setText(std::string text)
{
	m_label.setText(text);
}

void OptionButton::setText(std::string text, sf::Font & font)
{
	m_label.setText(text, font);
}
