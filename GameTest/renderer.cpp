#include "Object.cpp"
#include <math.h> 

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

global_variable float render_scale = 0.0025f;

static void
clear_screen(uint color) {
	uint* pixel = (uint*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}


void
draw_fone(std::vector<std::vector<uint>> map) {
	void* point = render_state.memory;
	memcpy(point, map[0].data(), map[0].size() * sizeof(uint));
}

static void
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


static void
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


static void
draw_obj(float x, float y, float half_x, float half_y, std::vector<uint> texture, int tSize) {
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
	int x_offset = x0 - clamp(0, x0, render_state.width);
	int y_offset = y0 - clamp(0, y0, render_state.height);

	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	int count = 0;

	for (int i = y0; i < y1; i += 1) {
		uint* pixel = (uint*)render_state.memory + x0 + i * render_state.width;
		for (int naxui = x0; naxui < x1; naxui += 1) {
			int a = (i - y0 - y_offset) % (int)(tSize * render_state.height * render_scale);
			int b = (naxui - x0 - x_offset) % (int)(tSize * render_state.width * render_scale+1);
			int cord = a * (tSize * render_state.width * render_scale + 1) + b;
			*pixel++ = texture[cord];
		}
	}
}


void textures_gen(std::vector<std::string> textures, std::vector<std::vector<uint>> &map) {
	for(std::vector<uint> m: map)
		m.clear();
	//Fone
	Magick::InitializeMagick("C:\Program Files\ImageMagick-7.1.0-Q16-HDRI");
	Magick::Image fone(textures[0]);
	std::string size = "4000x";
	size.append(std::to_string(render_state.height));
	fone.resize(size);
	fone.flip();
	unsigned char* rgb = static_cast<unsigned char*>(malloc(3 * render_state.width * render_state.height));

	fone.write(0, 0, render_state.width, render_state.height, "RGB", Magick::CharPixel, rgb);
	//static std::vector<std::vector<uint>> tmp;
	std::vector<uint> a;
	map.push_back(a);
	for (int i = 0; i < render_state.height; i++) {
		for (int j = 0; j < render_state.width; j++) {
			uint pixel = (uint)(*rgb++) << 16;
			pixel |= (uint)(*rgb++) << 8;
			pixel |= (uint)(*rgb++);
			map[0].push_back(pixel);
		}
	}
	
	//Block
	Magick::Image image(textures[1]);

	int width = 50 * render_state.width * render_scale + 1;
	int height = 50 * render_state.height * render_scale;

	Magick::Geometry geo(width, height);
	image.scale(geo);
	image.flip();
	image.write("out.png");
	
	rgb = static_cast<unsigned char*>(malloc(3 * width * height));
	image.write(0, 0, width, height, "RGB", Magick::CharPixel, rgb);
	map.push_back(a);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			uint pixel = (uint)(*rgb++) << 16;
			pixel |= (uint)(*rgb++) << 8;
			pixel |= (uint)(*rgb++);
			map[1].push_back(pixel);
		}
	}

	//map.push_back(a);
	//for (int n = 0; n < height; n++)
	//	map[1].insert(map[1].end(), tmp[image.rows() - n - 1].begin(), tmp[image.rows() - n - 1].end());

	/*for (int count = 0; count < this->textures.size(); count++) {
		Magick::Image fone(textures[count]);
		std::string size = "4000x";
		size.append(std::to_string(render_state.height));
		fone.resize(size);
		unsigned char* rgb = static_cast<unsigned char*>(malloc(sizeof(char) * 3 * render_state.width * render_state.height));
		fone.write(0, 0, render_state.width, render_state.height, "RGB", Magick::CharPixel, rgb);
		uint* pixel = (uint*)render_state.memory;
		for (int y = 0; y < render_state.height; y++) {
			for (int x = 0; x < render_state.width; x++) {
				*pixel++ = (uint)(*rgb++) << 16 | (uint)(*rgb++) << 8 | (uint)(*rgb++);
			}
		}
	}*/
	//HBITMAP hBitmap = (HBITMAP)LoadImage(hInstance, (LPCWSTR)path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

static void
draw_objects(float cam_x, float cam_y, std::vector<Object> &objects, std::vector<std::vector<uint>> map) {
	for (Object obj: objects)
		if(obj.map_count == 0)
			draw_rect(obj.pos_x - cam_x, obj.pos_y - cam_y, obj.half_size_x, obj.half_size_y, obj.color);
		else
			draw_obj(obj.pos_x - cam_x, obj.pos_y - cam_y, obj.half_size_x, obj.half_size_y, map[obj.map_count], obj.text_size);
}