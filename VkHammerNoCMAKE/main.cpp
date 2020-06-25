#include "main.h"


int main() {
	VkHammer App;

	App.VHinit("VkHammer", 1920, 1080);

	App.VHupdate();

	App.Cleanup();
	

	system("pause");
	return 0;
}