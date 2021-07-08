typedef unsigned int uint;

//#define internal static
#define global_variable static

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

#define	process_button(b, vk)\
case vk: {\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
}break;
