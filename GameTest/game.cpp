#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

global_variable float camera_pos_x = 0.f;
global_variable float camera_pos_y = 0.f;

#include <cmath>
#include <algorithm>

#include "Entity.cpp"

BITMAPFILEHEADER hdr;


class Level {
private:
	std::vector<Object> objects;
	std::vector<Object> platforms;
	std::vector<Object> buttons;
	std::vector<Object> thorns;

	std::vector<std::string> textures;
	std::vector<std::vector<uint>> map;

	Player player = Player(100, 425, 10.f, 25.f);

public:
	Level() {}

	void load(int level) {
		switch (level) {
		case 1: {
			textures.push_back("Textures\\Objects\\Fone.png");
			textures.push_back("Textures\\Objects\\Block.png");

			textures_gen(textures, map);

			player.set_pos(200, 325);
			objects.push_back(Object(-50, -200, 50, 1200, 0xFF5500, 1, 50));//left
			buttons.push_back(Object(200, 325, 25, 25, 0xFF0000, false));


			//objects.push_back(Object(250, 375, 50, 50, 0xFF5500, 1, 50));
			objects.push_back(Object(0, -200, 550, 500, 0xFF5500, 1, 50));
			objects.push_back(Object(400, -200, 50, 550, 0xFF5500, 1, 50));
			objects.push_back(Object(450, -200, 100, 500, 0xFF5500, 1, 50));
			objects.push_back(Object(550, -200, 50, 450, 0xFF5500, 1, 50));
			objects.push_back(Object(600, -200, 100, 500, 0xFF5500, 1, 50));
			objects.push_back(Object(700, -200, 100, 350, 0xFF5500, 1, 50));
			objects.push_back(Object(800, -200, 350, 300, 0xFF5500, 1, 50));

			objects.push_back(Object(800, 200, 50, 10, 0x501412, true));
			objects.push_back(Object(850, 250, 50, 10, 0x501412, true));
			objects.push_back(Object(800, 300, 50, 10, 0x501412, true));
			objects.push_back(Object(850, 350, 50, 10, 0x501412, true));
			objects.push_back(Object(1000, 200, 50, 10, 0x501412, true));
			objects.push_back(Object(1050, 250, 50, 10, 0x501412, true));
			objects.push_back(Object(1100, 300, 50, 10, 0x501412, true));

			objects.push_back(Object(1150, -200, 100, 550, 0xFF5500, 1, 50));
			objects.push_back(Object(1250, -200, 50, 500, 0xFF5500, 1, 50));
			objects.push_back(Object(1300, -200, 200, 550, 0xFF5500, 1, 50));
			objects.push_back(Object(1500, -200, 50, 600, 0xFF5500, 1, 50));
			objects.push_back(Object(1550, -200, 50, 500, 0xFF5500, 1, 50));
			objects.push_back(Object(1600, -200, 150, 550, 0xFF5500, 1, 50));
			objects.push_back(Object(1750, -200, 400, 400, 0xFF5500, 1, 50));

			objects.push_back(Object(1800, 340, 50, 10, 0x501412, true));
			objects.push_back(Object(1900, 300, 150, 50, 0xFF5500, 1, 50));
			objects.push_back(Object(2100, 200, 150, 100, 0xFF5500, 1, 50));
			objects.push_back(Object(2150, -200, 150, 300, 0xFF5500, 1, 50));
			objects.push_back(Object(2050, 240, 50, 10, 0x501412, true));

			objects.push_back(Object(2300, 290, 50, 10, 0x501412, true));
			objects.push_back(Object(2350, 240, 100, 10, 0x501412, true));

			objects.push_back(Object(2450, -200, 100, 350, 0xFF5500, 1, 50));
			objects.push_back(Object(2550, -200, 100, 300, 0xFF5500, 1, 50));
			objects.push_back(Object(2650, -200, 100, 350, 0xFF5500, 1, 50));
			objects.push_back(Object(2750, -200, 200, 600, 0xFF5500, 1, 50));
			objects.push_back(Object(2450, 250, 300, 100, 0xFF5500, 1, 50));
			objects.push_back(Object(2700, 350, 100, 300, 0xFF5500, 1, 50));

			buttons.push_back(Object(2145, 130, 15, 30, 0xFF0000, false));


		}break;
		case 0:
		default: {

			player.set_pos(200, 325);
			//objects.push_back(Object(-50, -200, 50, 1200, 0xFF5500, 1, 50));//left
			buttons.push_back(Object(200, 325, 25, 25, 0xFF0000, false));


			objects.push_back(Object(0, -200, 550, 500, 0xFF5500, 1, 50));
			/*objects.push_back(Object(0, 0, 50, 400, 0xFF5500, 1, 50));
			objects.push_back(Object(50, 0, 200, 250, 0xFF5500, 1, 50));
			objects.push_back(Object(250, 0, 50, 200, 0xFF5500, 1, 50));
			objects.push_back(Object(300, 0, 50, 150, 0xFF5500, 1, 50));
			objects.push_back(Object(350, 0, 200, 100, 0xFF5500, 1, 50));
			objects.push_back(Object(550, 0, 200, 250, 0xFF5500, 1, 50));
			objects.push_back(Object(750, 0, 50, 400, 0xFF5500, 1, 50));

			objects.push_back(Object(300, 250, 100, 30, 0xFF5500, 1, 50));
			objects.push_back(Object(450, 250, 50, 30, 0xFF5500, 1, 50));*/

			
		}break;
		}

		std::sort(objects.begin(), objects.end(),
			[](const Object& a, Object& b) {
				return a.pos_y > b.pos_y;
			});
		player.find_platform(objects);
	}

	void draw_interface(Player player) {
		draw_rect(-165, 175, 27, 7, 0x808080);
		draw_rect(-165, 175, 25, 5, 0xFF0000);
		float hp = 2.5 * player.get_health();
		draw_rect(-190 + hp, 175, hp, 5, 0x00FF00);
	}

	void run(Input* input, float dt) {
		//clear_screen(0xFFFFFF);
		draw_fone(map);

		//Draw all obj
		draw_objects(camera_pos_x, camera_pos_y, objects, map);
		draw_objects(camera_pos_x, camera_pos_y, buttons, map);

		//Simulate player
		player.simulate(input, dt, objects, buttons);
		draw_interface(player);
	}
	void update_textures(){textures_gen(textures, map);}
};