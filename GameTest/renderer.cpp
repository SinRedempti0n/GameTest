class Object {
public:
	float	pos_x;
	float	pos_y;
	float	half_size_x;
	float	half_size_y;
	uint	color;

	bool state;

	Object() {}

	void flip () {
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
	}
	
	Object(float X, float Y, float SizeX, float SizeY, uint color, bool state) {
		this->pos_x = X + SizeX / 2;
		this->pos_y = Y + SizeY / 2;
		this->half_size_x = SizeX / 2;
		this->half_size_y = SizeY / 2;
		this->color = color;
		this->state = state;
	}
};

class Objects {
private:
	Object* objects;
	int count;

public:
	int load() {
		count = 2;
		objects = new Object[count];
		objects[0] = Object(50, 50, 5, 50, 0xFF5500);
		objects[1] = Object(0, -5, 500, 5, 0xFF5500);
		return count;
	}

	Object* get() { return objects; }
	int getCount() { return count; }

};


internal void 
clear_screen(uint color) {
	uint* pixel = (uint*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void 
draw_rect_in_pixels(int x0, int y0, int x1, int y1, uint color) {
	
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);
	
	for (int y = y0; y < y1; y++) {
		uint* pixel = (uint*)render_state.memory + x0 +	y*render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.0025f;

internal void
draw_rect(float x, float y, float half_x, float half_y, uint color) {
	x *= render_state.width * render_scale;
	y *= render_state.height * render_scale;
	half_x *= render_state.width * render_scale;
	half_y *= render_state.height * render_scale;
	
	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	//Change to pixels
	int x0 = x - half_x;
	int y0 = y - half_y;
	int x1 = x + half_x;
	int y1 = y + half_y;


	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void
draw_objects(float cam_x, float cam_y, std::vector<Object> &objects) {
	for (Object obj: objects)
		draw_rect(obj.pos_x - cam_x, obj.pos_y - cam_y, obj.half_size_x, obj.half_size_y, obj.color);
}