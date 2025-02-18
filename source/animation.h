#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class animation
{
public:
	animation(Texture* texture, Vector2u imageCount, float switchTime);
	~animation();

	void changeTexture(Texture* texture, Vector2u imageCount, float switchTime);
	void update(int row, float deltaTime);

public:
	IntRect uvRect;

private:
	Vector2u imageCount;
	Vector2u currentImage;

	float totalTime;
	float switchTime;
};

