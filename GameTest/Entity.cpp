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
	int map_count = 2;
	


	//Horizontal movement params
	const float max_speed = 125.f;
	float vel_x = 0.f;
	float acc = 120;
	bool direction = true;
	float lPos_x;
	int xAnim = 12;

	//Vertical movement params
	float vel_y = 0.f;
	float grav = 1000.f;
	bool on_ground = true;
	bool dJump = true;

	float pos_y2;
	bool up = true;

	void collision(std::vector<Object> objects) {
		for (Object obj : objects)
			if (!obj.floating &&
				pos_x + half_size_x > obj.pos_x - obj.half_size_x &&
				pos_x - half_size_x < obj.pos_x - obj.half_size_x &&
				pos_y < obj.pos_y + obj.half_size_y &&
				pos_y > obj.pos_y - obj.half_size_y) {
				if (pos_x < obj.pos_x) {
					pos_x = obj.pos_x - obj.half_size_x - half_size_x;
					vel_x = 0;
				}
			}
		for (Object obj : objects)
			if (!obj.floating &&
				pos_x - half_size_x < obj.pos_x + obj.half_size_x &&
				pos_x + half_size_x > obj.pos_x + obj.half_size_x &&
				pos_y < obj.pos_y + obj.half_size_y &&
				pos_y > obj.pos_y - obj.half_size_y) {
				if (pos_x > obj.pos_x) {
					pos_x = obj.pos_x + obj.half_size_x + half_size_x;
					vel_x = 0;
				}
			}
	}

	void collision_y(std::vector<Object> objects) {
		for (Object obj : objects)
			if (!obj.floating) { 
				if (!up && pos_x - half_size_x < obj.pos_x + obj.half_size_x &&
					pos_x + half_size_x > obj.pos_x - obj.half_size_x &&
					pos_y - half_size_y < obj.pos_y + obj.half_size_y) {
					if (pos_y > obj.pos_y) {
						pos_y = obj.pos_y + obj.half_size_y + half_size_y;
						vel_y = 0;
						on_ground = true;
					}
				}
				if (up && pos_x - half_size_x < obj.pos_x + obj.half_size_x &&
					pos_x + half_size_x > obj.pos_x - obj.half_size_x &&
					pos_y + half_size_y > obj.pos_y - obj.half_size_y) {
					if (pos_y < obj.pos_y) {
						pos_y = obj.pos_y - obj.half_size_y - half_size_y;
						vel_y = 0;
					}
				}
			}

		for (Object obj : objects)
			if (obj.floating && !up && pos_x - half_size_x < obj.pos_x + obj.half_size_x &&
				pos_x + half_size_x > obj.pos_x - obj.half_size_x &&
				pos_y - half_size_y < obj.pos_y + obj.half_size_y) {
				if (pos_y - half_size_y > obj.pos_y) {
					pos_y = obj.pos_y + obj.half_size_y + half_size_y;
					vel_y = 0;
					on_ground = true;
				}
			}
	}

	void movement(Input* input, float dt, std::vector<Object> objects, std::vector<Object>& buttons) {

		//Movement vertical
		pos_y2 = pos_y + vel_y * dt;

		if (pos_y < pos_y2) {
			up = true;
		}
		else {
			up = false;
		}

		pos_y = pos_y2;
		
		find_platform(objects);

		//Decceleration vertical
		if (pos_y - half_size_y > platform.pos_y + platform.half_size_y) {
			vel_y -= grav * dt;
		}
		else {
			vel_y = 0;
			pos_y = platform.pos_y + platform.half_size_y + half_size_y;
			on_ground = true;
			dJump = true;
		}

		if (vel_x < 12.5 && vel_x > -12.5 && on_ground) {
			lPos_x = pos_x;
			map_count = 2;
		}

		//Acceleration horizontal
		if (is_down(BUTTON_RIGHT)) {
			this->direction = true;
			vel_x < max_speed ? vel_x += acc * dt : vel_x = max_speed;
			if (lPos_x + xAnim < pos_x && on_ground) {
				lPos_x = pos_x;
				if (map_count < 5)
					map_count++;
				else
					map_count = 2;
			}
		}
		if (is_down(BUTTON_LEFT)) {
			this->direction = false;
			vel_x > max_speed * -1 ? vel_x -= acc * dt : vel_x = max_speed * -1;
			if (lPos_x - xAnim > pos_x && on_ground) {
				lPos_x = pos_x;
				if (map_count < 5)
					map_count++;
				else
					map_count = 2;
			}
		}

		//Decceleration horizontal
		if (!(is_down(BUTTON_RIGHT) || is_down(BUTTON_LEFT))) {
			if (vel_x > 0) {
				if (lPos_x + xAnim < pos_x && on_ground) {
					lPos_x = pos_x;
					if (map_count < 5)
						map_count++;
					else
						map_count = 2;
				}
				vel_x -= acc * 2 * dt;
				if (vel_x <= 0)
					vel_x = 0;
			}
			else if (vel_x < 0) {
				if (lPos_x - xAnim > pos_x && on_ground) {
					lPos_x = pos_x;
					if (map_count < 5)
						map_count++;
					else
						map_count = 2;
				}
				vel_x += acc * 2 * dt;
				if (vel_x >= 0)
					vel_x = 0;
			}
		}
		//Movement horizontal
		pos_x += vel_x * dt;

		//Acceleration vertical
		if (pressed(BUTTON_UP)) {
			if (on_ground) {
				vel_y = 400;
				on_ground = false;
			}
			else if (dJump) {
				vel_y = 300;
				dJump = false;
			}

		}
		collision(objects);
		collision_y(objects);

		if (pressed(BUTTON_ACTION)) {
			for (Object& obj : buttons)
				/*if ((pos_x + half_size_x > obj.pos_x - obj.half_size_x &&
					pos_y - half_size_y < obj.pos_y + obj.half_size_y &&
					pos_y + half_size_y > obj.pos_y - obj.half_size_y) ||
					(pos_x - half_size_x < obj.pos_x + obj.half_size_x &&
						pos_y - half_size_y < obj.pos_y + obj.half_size_y &&
						pos_y + half_size_y > obj.pos_y - obj.half_size_y)) {
					obj.flip();
				}*/
				if (abs(pos_x - obj.pos_x) < 30 && abs(pos_y - obj.pos_y) < 30) {
					obj.flip();
				}

		}

	}

	void camera(float dt) {
		float x_scale = 0.1;
		if (pos_x - camera_pos_x > x_scale / render_scale)
			camera_pos_x = pos_x - x_scale / render_scale;
		if (pos_x - camera_pos_x < -1 * x_scale / render_scale)
			camera_pos_x = pos_x + x_scale / render_scale;

		float cam_height = 0.1 / render_scale;

		if (pos_y + cam_height != camera_pos_y) {
			if (camera_pos_y > pos_y + cam_height) {
				camera_pos_y -= grav * 0.9 * dt;
				if (camera_pos_y <= pos_y + cam_height)
					camera_pos_y = pos_y + cam_height;
			}
			else if (camera_pos_y < pos_y + cam_height && on_ground) {
				camera_pos_y += 2 * acc * dt;
				if (camera_pos_y >= pos_y + cam_height)
					camera_pos_y = pos_y + cam_height;
			}
		}

		/*camera_pos_x = pos_x;
		camera_pos_y = pos_y*/
	}

public:
	Player() {}

	Player(float X, float Y, float hSizeX, float hSizeY) :Entity(X, Y, hSizeX, hSizeY) {
		lPos_x = X;
	}

	void find_platform(std::vector<Object> objects) {
		for (int i = 0; i < objects.size(); i++) {
			if (pos_x + half_size_x < objects[i].pos_x + objects[i].half_size_x &&
				pos_x - half_size_x > objects[i].pos_x - objects[i].half_size_x &&
				pos_y - half_size_y >= objects[i].pos_y + objects[i].half_size_y) {
				platform = objects[i];
				break;
			}
		}
	}

	void set_pos(float X, float Y) {
		this->pos_x = X;
		this->pos_y = Y;
		camera_pos_x = X;
		camera_pos_y = Y;
	}

	int get_health() { return health; }

	void simulate(Input* input, float dt, std::vector<Object> objects, std::vector<Object>& buttons, std::vector<std::vector<uint>> map) {

		movement(input, dt, objects, buttons);

		camera(dt);

		draw_player(pos_x - camera_pos_x, pos_y - camera_pos_y, half_size_x, half_size_y, map[map_count], 16, 27, this->direction);
	}
};