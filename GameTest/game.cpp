#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_pos_x = 0.f;
float player_pos_y = 0.f;

class Player {
public:
	float player_pos_x = 0.f;
	float player_pos_y = 0.f;

	const float max_speed = 0.6f;
	float vel_x = 0.f;
	float acc = 2;
	bool direction = true;

	float vel_y = 0.f;
	float grav = 0.05f;

	bool on_ground = true;
	
	void simulate(Input* input, float dt) {
		if (pressed(BUTTON_UP))
			if (on_ground) {
				vel_y = 2;
				on_ground = false;
			}

		player_pos_y += vel_y;
		if (player_pos_y > 0)
			vel_y -= grav;
		else {
			player_pos_y = 0;
			on_ground = true;
		}

		//if (is_down(BUTTON_DOWN)) player_pos_y -= grav * dt;



		if (is_down(BUTTON_RIGHT) || is_down(BUTTON_LEFT)) {
			vel_x < max_speed ? vel_x += acc * dt : vel_x = max_speed;
			if (is_down(BUTTON_RIGHT))
				direction = true;
			else
				direction = false;
		}else vel_x > 0 ? vel_x -= acc * 1.5 * dt : vel_x = 0;

		if (direction) {
			player_pos_x += vel_x;
		}else
			player_pos_x -= vel_x;
	}
};

Player player;

internal void
simulate_game(Input* input, float dt) {
	clear_screen(0xFFFFFF);
	draw_rect(0, -5, 500, 5, 0xFF5500);
	draw_rect(50, 50, 5, 50, 0xFF5500);
	

	
	float speed = 50.f;

	player.simulate(input, dt);
	
	draw_rect(player.player_pos_x, player.player_pos_y + 15, 5, 15, 0x00FF00);
	//draw_rect(-20, -20, 10, 10, 0xFF5500);
}