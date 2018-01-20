#include "Button.h"

Button::Button(sf::Vector2f position, std::string name, std::string iconName = "Sword Icon"):
	m_alpha(255), //alpha starts at 255, so the button is completely opaque
	m_fadeIn(false),
	m_fadeOut(false),
	m_position(position),
	m_name(name),
	m_label(name, m_position),
	m_buttonAnimator(m_animationHolder),
	m_iconAnimator(m_animationHolder)
{
	m_sprite.setTexture(resourceManager.getTextureHolder()["Button Spritesheet"]);
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

	//Setup our select/deselect animations
	setUpAnimation();
}

void Button::update()
{
	//Update and animate our button
	m_buttonAnimator.update(m_animationClock.restart());
	m_buttonAnimator.animate(m_sprite);

	//Update and animate our icon
	m_iconAnimator.update(m_iconAnimationClock.restart());
	m_iconAnimator.animate(m_icon);

	//If our bool is true call the fade in method
	if (m_fadeIn)
		fade(255);
	else if (m_fadeOut)
		fade(0);
}

void Button::render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
	window.draw(m_icon);
	m_label.draw(window);
}


void Button::fade(int desiredAlpha)
{
	//If our alpha is below our desired alpha then increase our alpha
	if (m_alpha < desiredAlpha)
	{
		m_alpha += 3;

		if (m_alpha >= 255)
		{
			m_alpha = 255;
			m_fadeIn = false;
		}

	}
	//else if our alpha is above our ddesired alpha then decrease our alpha
	else if(m_alpha > desiredAlpha)
	{
		m_alpha -= 3;

		if (m_alpha <= 0)
		{
			m_alpha = 0;
			m_fadeOut = false;
		}
	}

	applyAlpha(m_alpha);
}

void Button::select()
{
	m_label.select();
	m_buttonAnimator.play() << "selected";
	m_iconAnimator.play() << "iconSelected";
	m_sprite.setTextureRect(m_selectedRect);
	m_icon.setTextureRect(m_iconSelectedRect);
	m_selected = true;
}

void Button::deSelect()
{
	m_label.deSelect();
	m_buttonAnimator.play() << "deselected";
	m_iconAnimator.play() << "iconDeselected";
	m_sprite.setTextureRect(m_unselectedRect);
	m_icon.setTextureRect(m_iconUnselectedRect);
	m_selected = false;
}

void Button::setUpAnimation()
{
	m_animationClock.restart(); //starting our animation clock

	//the size of one frame in our spritesheet is 400 x 55 pixels
	auto sourceSize = sf::Vector2i(400, 55);
	//the size of our icon frame size
	auto iconSourceSize = sf::Vector2i(55, 55);

	m_unselectedRect = sf::IntRect(0, 0, sourceSize.x, sourceSize.y); //the first frame in our spritesheet
	m_iconUnselectedRect = sf::IntRect(0, 0, iconSourceSize.x, iconSourceSize.y); //the first frame of our icon
	m_selectedRect = sf::IntRect(4000, 0, sourceSize.x, sourceSize.y); //the last frame in our animation
	m_iconSelectedRect = sf::IntRect(550, 0, iconSourceSize.x, iconSourceSize.y); //the last frame in our icon animation

	m_sprite.setTextureRect(m_unselectedRect);
	m_icon.setTextureRect(m_iconUnselectedRect);

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
	}

	m_animationHolder.addAnimation("selected", m_selectAnimation, sf::seconds(0.15f));
	m_animationHolder.addAnimation("deselected", m_deselectAnimation, sf::seconds(0.075f));
	m_animationHolder.addAnimation("iconSelected", m_iconSelectAnimation, sf::seconds(0.15f));
	m_animationHolder.addAnimation("iconDeselected", m_iconDeselectAnimation, sf::seconds(0.075f));
}

void Button::applyAlpha(float value)
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

std::string& Button::getName()
{
	return m_name;
}

float Button::getAlpha()
{
	return m_alpha;
}

bool Button::getSelected()
{
	return m_selected;
}

void Button::setAlpha(float a)
{
	m_alpha = a;
	applyAlpha(m_alpha); //apply our new alpha value to our sprites
}

void Button::setFadeIn(bool fade)
{
	m_fadeIn = fade;
}

void Button::setFadeOut(bool fade)
{
	m_fadeOut = fade;
}
