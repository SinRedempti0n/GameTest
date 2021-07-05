class Object {
public:
	float	pos_x;
	float	pos_y;
	float	half_size_x;
	float	half_size_y;
	uint	color;

	bool floating;
	bool state;

	Object() {}

	void flip() {
		this->state = !state;
		if (state == true) {
			this->color = 0x00FF00;
		}
		else {
			this->color = 0xFF0000;
		}
	}

	bool isActive() {
		return state;
	}

	Object(float X, float Y, float SizeX, float SizeY, uint color) {
		this->pos_x = X + SizeX / 2;
		this->pos_y = Y + SizeY / 2;
		this->half_size_x = SizeX / 2;
		this->half_size_y = SizeY / 2;
		this->color = color;
		this->floating = false;
	}

	Object(float X, float Y, float SizeX, float SizeY, uint color, bool type) {
		this->pos_x = X + SizeX / 2;
		this->pos_y = Y + SizeY / 2;
		this->half_size_x = SizeX / 2;
		this->half_size_y = SizeY / 2;
		this->color = color;
		if (type == true) {
			this->floating = true;
		}
		else {
			this->state = false;
		}
	}


};

