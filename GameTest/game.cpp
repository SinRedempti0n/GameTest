#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

global_variable float camera_pos_x = 0.f;
global_variable float camera_pos_y = 0.f;

#include <cmath>
#include <algorithm>


class Entity {
public:
	//Position
	float pos_x;
	float pos_y;
	Object platform;

	//Size
	float half_size_x;
	float half_size_y;
	Entity() {}

	Entity(float X, float Y, float hSizeX, float hSizeY) {
		this->pos_x = X;
		this->pos_y = Y;
		this->half_size_x = hSizeX;
		this->half_size_y = hSizeY;
	}
};

class Player: public Entity {
private:
	//Horizontal movement params
	const float max_speed = 125.f;
	float vel_x = 0.f;
	float acc = 1;
	bool direction = true;

	//Vertical movement params
	float vel_y = 0.f;
	float grav = 1000.f;
	bool on_ground = true;
	bool dJump = true;

	void movement(Input* input, float dt, std::vector<Object> objects, int count) {
		//Acceleration vertical
		if (pressed(BUTTON_UP))
			if (on_ground) {
				vel_y = 400;
				on_ground = false;
			}
			else if (dJump) {
				vel_y = 300;
				dJump = false;
			}
		//If u fall from platform
		if (pos_x - half_size_x > platform.pos_x + platform.half_size_x ||
			pos_x + half_size_x < platform.pos_x - platform.half_size_x)
			for (Object obj : objects) {
				if (pos_x + half_size_x < obj.pos_x + obj.half_size_x &&
					pos_x - half_size_x > obj.pos_x - obj.half_size_x &&
					pos_y - half_size_y >= obj.pos_y + obj.half_size_y) {
					platform = obj;
					break;
				}
			}
		//If u flying seek for platform under u
		if (!on_ground)
			find_platform(objects);

		//Movement vertical
		pos_y += vel_y * dt;

		//Decceleration vertical
		if (pos_y > platform.pos_y + platform.half_size_y + half_size_y) {
			vel_y -= grav * dt;
		}
		else {
			vel_y = 0;
			pos_y = platform.pos_y + platform.half_size_y + half_size_y;
			on_ground = true;
			dJump = true;
		}
		
		//Acceleration horizontal
		if (is_down(BUTTON_RIGHT)) {
			vel_x < max_speed ? vel_x += acc : vel_x = max_speed;
			for (Object obj : objects)
				if (pos_x + half_size_x > obj.pos_x - obj.half_size_x &&
					pos_y - half_size_y < obj.pos_y + obj.half_size_y &&
					pos_y + half_size_y > obj.pos_y - obj.half_size_y) {
					if (pos_x < obj.pos_x) {
						pos_x = obj.pos_x - obj.half_size_x - half_size_x;
						vel_x = 0;
					}
				}
		}
		if (is_down(BUTTON_LEFT)) {
			vel_x > max_speed * -1 ? vel_x -= acc : vel_x = max_speed * -1;
			for (Object obj : objects)
				if (pos_x - half_size_x < obj.pos_x + obj.half_size_x &&
					pos_y - half_size_y < obj.pos_y + obj.half_size_y &&
					pos_y + half_size_y > obj.pos_y - obj.half_size_y) {
					if (pos_x > obj.pos_x) {
						pos_x = obj.pos_x + obj.half_size_x + half_size_x;
						vel_x = 0;
					}
				}
		}

		//Decceleration horizontal
		if (!(is_down(BUTTON_RIGHT) || is_down(BUTTON_LEFT))) {
			if (vel_x > 0) {
				vel_x -= acc * 1.5;
				if (vel_x <= 0)
					vel_x = 0;
			}
			else if (vel_x < 0) {
				vel_x += acc * 1.5;
				if (vel_x >= 0)
					vel_x = 0;
			}
		}

		//Movement horizontal
		pos_x += vel_x * dt;
	}

	void camera() {
		if (pos_x - camera_pos_x > 0.2 / render_scale)
			camera_pos_x = pos_x - 0.2 / render_scale;
		if (pos_x - camera_pos_x < -0.2 / render_scale)
			camera_pos_x = pos_x + 0.2 / render_scale;

		float cam_height = 0.1 / render_scale;

		if (pos_y + cam_height != camera_pos_y) {
			if (camera_pos_y > pos_y + cam_height) {
				camera_pos_y -= grav * 0.9;
				if (camera_pos_y <= pos_y + cam_height)
					camera_pos_y = pos_y + cam_height;
			}
			else if (camera_pos_y < pos_y + cam_height && on_ground) {
				camera_pos_y += acc;
				if (camera_pos_y >= pos_y + cam_height)
					camera_pos_y = pos_y + cam_height;
			}
		}
	}

public:
	Player() {}

	Player(float X, float Y, float hSizeX, float hSizeY):Entity(X, Y, hSizeX, hSizeY) {}

	void find_platform(std::vector<Object> objects) {
		for (int i = 0; i < objects.size(); i++) {
			if (pos_x - half_size_x < objects[i].pos_x + objects[i].half_size_x &&
				pos_x + half_size_x > objects[i].pos_x - objects[i].half_size_x &&
				pos_y - half_size_y >= objects[i].pos_y + objects[i].half_size_y) {
				platform = objects[i];
				break;
			}
		}
	}

	void simulate(Input* input, float dt, std::vector<Object> objects, int count) {

		movement(input, dt, objects, count);

		camera();

		draw_rect(pos_x - camera_pos_x, pos_y - camera_pos_y, half_size_x, half_size_y, 0x00FF00);
	}
};

class Level {
private:
	std::vector<Object> objects;
	Player player = Player(100, 425, 10.f, 25.f);

public:
	Level() {}

	void load() {
		objects.push_back(Object(0, 0, 50, 400, 0xFF5500));
		objects.push_back(Object(50, 0, 200, 250, 0xFF5500));
		objects.push_back(Object(250, 0, 50, 200, 0xFF5500));
		objects.push_back(Object(300, 0, 50, 150, 0xFF5500));
		objects.push_back(Object(350, 0, 200, 100, 0xFF5500));
		objects.push_back(Object(550, 0, 200, 250, 0xFF5500));
		objects.push_back(Object(750, 0, 50, 400, 0xFF5500));

		objects.push_back(Object(300, 250, 100, 30, 0xFF5500));
		objects.push_back(Object(450, 250, 50, 30, 0xFF5500));

		std::sort(objects.begin(), objects.end(),
			[](const Object& a, Object& b) {
				return a.pos_y > b.pos_y;
			});
		player.find_platform(objects);
	}

	void run(Input* input, float dt) {
		clear_screen(0xFFFFFF);

		//Draw all obj
		draw_objects(camera_pos_x, camera_pos_y, objects, objects.size());

		//Simulate player
		player.simulate(input, dt, objects, objects.size());
	}


};