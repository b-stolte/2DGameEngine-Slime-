#pragma once

struct Animation {

	int index;
	int frames;
	int speed;
	bool repeat;

	Animation() {}


	Animation(int i, int f, int s, bool r) {
		index = i;
		frames = f;
		speed = s;
		repeat = r;
	}
};