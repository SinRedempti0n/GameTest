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

class Player : public Entity {
private:
	//Param
	int health = 8;

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

	void collision(std::vector<Object> objects) {
		for (Object obj : objects)
			if (pos_x + half_size_x > obj.pos_x - obj.half_size_x &&
				pos_y - half_size_y < obj.pos_y + obj.half_size_y &&
				pos_y + half_size_y > obj.pos_y - obj.half_size_y && !obj.floating) {
				if (pos_x < obj.pos_x) {
					pos_x = obj.pos_x - obj.half_size_x - half_size_x;
					vel_x = 0;
				}
			}
		for (Object obj : objects)
			if (pos_x - half_size_x < obj.pos_x + obj.half_size_x &&
				pos_y - half_size_y < obj.pos_y + obj.half_size_y &&
				pos_y + half_size_y > obj.pos_y - obj.half_size_y && !obj.floating) {
				if (pos_x > obj.pos_x) {
					pos_x = obj.pos_x + obj.half_size_x + half_size_x;
					vel_x = 0;
				}
			}
	}

	void movement(Input* input, float dt, std::vector<Object> objects, std::vector<Object>& buttons) {
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
		if (!on_ground) {
			find_platform(objects);
			for (Object obj : objects)
				if ((pos_x - half_size_x < obj.pos_x + obj.half_size_x ||
					pos_x + half_size_x > obj.pos_x - obj.half_size_x) &&
					pos_y + half_size_y > obj.pos_y - obj.half_size_y && !obj.floating) {
					pos_y = obj.pos_y - obj.half_size_y - half_size_y;
					vel_y = 0;
				}
		}

		//Movement vertical
		pos_y += vel_y * dt;

		//Decceleration vertical
		if (pos_y > platform.pos_y + platform.half_size_y + half_size_y) { vel_y -= grav * dt; }
		else {
			vel_y = 0;
			pos_y = platform.pos_y + platform.half_size_y + half_size_y;
			on_ground = true;
			dJump = true;
		}

		//Acceleration horizontal
		if (is_down(BUTTON_RIGHT)) {
			vel_x < max_speed ? vel_x += acc : vel_x = max_speed;
			collision(objects);
		}
		if (is_down(BUTTON_LEFT)) {
			vel_x > max_speed * -1 ? vel_x -= acc : vel_x = max_speed * -1;
			collision(objects);
		}

		//Decceleration horizontal
		if (!(is_down(BUTTON_RIGHT) || is_down(BUTTON_LEFT))) {
			if (vel_x > 0) {
				collision(objects);
				vel_x -= acc * 1.5;
				if (vel_x <= 0)
					vel_x = 0;
			}
			else if (vel_x < 0) {
				collision(objects);
				vel_x += acc * 1.5;
				if (vel_x >= 0)
					vel_x = 0;
			}
		}

		if (pressed(BUTTON_ACTION)) {
			for (Object& obj : buttons)
				if ((pos_x + half_size_x > obj.pos_x - obj.half_size_x &&
					pos_y - half_size_y < obj.pos_y + obj.half_size_y &&
					pos_y + half_size_y > obj.pos_y - obj.half_size_y) ||
					(pos_x - half_size_x < obj.pos_x + obj.half_size_x &&
						pos_y - half_size_y < obj.pos_y + obj.half_size_y &&
						pos_y + half_size_y > obj.pos_y - obj.half_size_y)) {
					obj.flip();
				}

		}

		//Movement horizontal
		pos_x += vel_x * dt;
	}

	void camera() {
		float x_scale = 0.1;
		if (pos_x - camera_pos_x > x_scale / render_scale)
			camera_pos_x = pos_x - x_scale / render_scale;
		if (pos_x - camera_pos_x < -1 * x_scale / render_scale)
			camera_pos_x = pos_x + x_scale / render_scale;

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

		/*camera_pos_x = pos_x;
		camera_pos_y = pos_y*/
	}

public:
	Player() {}

	Player(float X, float Y, float hSizeX, float hSizeY) :Entity(X, Y, hSizeX, hSizeY) {}

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

	void set_pos(float X, float Y) {
		this->pos_x = X;
		this->pos_y;
	}

	int get_health() { return health; }

	void simulate(Input* input, float dt, std::vector<Object> objects, std::vector<Object>& buttons) {

		movement(input, dt, objects, buttons);

		camera();

		draw_rect(pos_x - camera_pos_x, pos_y - camera_pos_y, half_size_x, half_size_y, 0x00FF00);
	}
};