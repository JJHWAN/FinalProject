#define DEVICE_MAJOR 242 // Device Major Number
#define DEVICE_MINOR 0  // Device Minor Number
#define DEVICE_NAME "driver" // Device name

#define IOM_FND_ADDRESS 0x08000004 // Physical address of FND device
#define IOM_FPGA_DOT_ADDRESS 0x08000210  // Physical address of DOT device
#define IOM_LED_ADDRESS 0x08000016 // Physical address of LED device
#define IOM_FPGA_TEXT_LCD_ADDRESS 0x08000090 // Physical address of TEXT LCD device

#define GAME_READY 0
#define GAME_START 1
#define GAME_EXIT 2

#define KEY_PRESSED 0
#define KEY_RELEASED 1

// struct for data from user
struct data_from_user{
	int data;
};

// struct for saving virtual address of Devices
struct device_addr{
	unsigned char *fnd_addr; // Virtual address of FND device
	unsigned char *dot_addr; // Virtual address of FND device
	unsigned char *led_addr; // Virtual address of FND device
	unsigned char *lcd_addr; // Virtual address of FND device
};

unsigned char fpga_set_dino[10] = {
	// memset(array,0x00,sizeof(array));
	0x07,0x05,0x07,0x46,0x47,0x26,0x3E,0x1E,0x0C,0x00
};

unsigned char fpga_set_blank[10] = {
	// memset(array,0x00,sizeof(array));
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// strings used in TEXT LCD
int length_lcd[3] = {25, 20, 9};
unsigned char lcd_game_ready[] = "Press Start to Play Game";
unsigned char lcd_game_start[] = "Player playing Game";
unsigned char lcd_game_end[] = "Game End";

// values for IOCTL
#define IOCTL_UPDATE_SCORE 0  // Ioctl func - update score (from Android)
#define IOCTL_WAIT_INTR 1  // Ioctl func - wait for interrput (To Android)
#define IOCTL_MSG 2

