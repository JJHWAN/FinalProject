#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/version.h>

#include "devices.h"

// #define DEBUG // un-comment this for debug prints

// # 개발일지
// ~ 05/07 12시까지 기본적인 init, exit 함수 구현
// cls create에서 THIS MODULE은? -> cdev.h 내부의 cdev 구조체에 struct module *owner

// FND, LCD, LED, DOT의 경우, 예제로 주어진 파일들을 export해서 사용하는 방안으로
// DOT의 경우, 미리미리 값을 만들어두면 편할듯?

// 아니면 그냥 module에서 각각 open하는 방법도..

// open, close, ioctl, dino_func
static int dino_device_open(struct inode *, struct file *);
static int dino_device_release(struct inode *, struct file *);
static int dino_device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static void dino_blink(unsigned long timeout);

// set values with data given from user
static void set_value(void);
// update values for every time intervals
static void update_value(void);
// update lcd strings starting positions
static void move_lcd(void);

// for writing devices (using outw)
static void write_fnd(int cur_num);
static void write_dot(int cur_num);
static void write_lcd(void);
static void write_led(int cur_num);

static struct class *cls;

// mapping syscall with driver functions
static struct file_operations dino_device_fops =
    {.open = dino_device_open, .unlocked_ioctl = dino_device_ioctl, .release = dino_device_release};

static int driver_usage = 0; // data for ensuring single use to user program
static int flag_state = 0;     // flag for game state

// object for datas
struct device_addr addr;


// IOCTL_
static int dino_device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch (_IOC_NR(cmd))
    {
    case IOCTL_SET_OPTION:
        // 입력으로 들어온 argument로 timer_data를 설정해준다.
#ifdef DEBUG
        printk("In ioctl, with SET_OPTION cmd\n");
#endif

#ifdef DEBUG
        printk("Data copy sucess in setting options\n");
        printk("timer_interval : %d, timer_cnt : %d, init_num : %d\n", data.timer_interval, data.timer_cnt, data.init_num);
#endif

        break;
    case IOCTL_START:

        break;
    }

    return 1;
}

// function that inits flag_state, and output devices
static void init_board()
{
    flag_state = 0;    // set flag_end to 0
    write_fnd(0);
    write_dot(0);
    write_lcd(0);
    write_led(0);
}

// function for writing at FND device
// write cur_num on fnd, if cur_num = 1000, then write 1000
static void write_fnd(int cur_num)
{
    int i, tmp = 1000;

    unsigned char val[4] = {
        0,
    };
    unsigned short int val_short = 0;

    for (i = 0; i < 3; i++)
    {
        val[i] = (unsigned char)(cur_num / tmp);
        cur_num = cur_num % tmp;
    }
    val[3] = (unsigned char)cur_num;
    val_short = val[0] << 12 | val[1] << 8 | val[2] << 4 | val[3];
    outw(val_short, (unsigned int)addr.fnd_addr);
}

// function for writing at DOT device
// if flag == GAME_START, write Dino
// else write blank
static void write_dot(int flag)
{

    int i;
    unsigned short int _s_value = 0;

    switch (flag)
    {
    case GAME_START:
        for (i = 0; i < 10; i++)
        {
            _s_value = fpga_set_dino[i] & 0x7F;
            outw(_s_value, (unsigned int)addr.dot_addr + i * 2);
        }
        break;
    default:
        for (i = 0; i < 10; i++)
        {
            _s_value = fpga_set_blank[i] & 0x7F;
            outw(_s_value, (unsigned int)addr.dot_addr + i * 2);
        }
        break;
    }
}

// function for writing at TEXT LCD device
static void write_lcd(int flag)
{

    // lcd 방향 및 시작 위치 조정

    int i, len = length_lcd[flag];
    int len_up, len_down;
    unsigned short int _s_value = 0;

    unsigned char value_up[16], value_down[16];

    for (i = 0; i < 16; i++)
    {
        value_up[i] = ' ';
        value_down[i] = ' ';
    }

    if(len > 16){
        len_ up =16;
        len_down = len - 16;
    }
    else {
        len_up = len;
        len_down = 0;
    }

    switch (flag)
    {
    case GAME_READY:
        for (i = 0; i < len_up; i++)
            value_up[i] = lcd_game_ready[i];
        for(i=0;i<len_down;i++)
            value_down[i] = lcd_game_ready[i + 16];
        break;
    case GAME_START:
        /* code */
        for (i = 0; i < len_up; i++)
            value_up[i] = lcd_game_start[i];
        for(i=0;i<len_down;i++)
            value_down[i] = lcd_game_start[i + 16];
        
        break;
    case GAME_EXIT:
        /* code */
        for (i = 0; i < len_up; i++)
            value_up[i] = lcd_game_end[i];
        for(i=0;i<len_down;i++)
            value_down[i] = lcd_game_end[i + 16];
        break;
    }

    for (i = 0; i < 16; i += 2)
    {
        _s_value = (value_up[i] & 0xFF) << 8 | (value_up[i + 1] & 0xFF);
        outw(_s_value, (unsigned int)addr.lcd_addr + i);
    }

    for (i = 0; i < 16; i += 2)
    {
        _s_value = (value_down[i] & 0xFF) << 8 | (value_down[i + 1] & 0xFF);
        outw(_s_value, (unsigned int)addr.lcd_addr + i + 16);
    }
    
}

// function for writing at TEXT LED device
static void write_led(int cur_num)
{
    unsigned short _s_value;

    _s_value = (unsigned short)(1 << (8 - cur_num));
    if (flag_end)
        _s_value = 0x00;
    outw(_s_value, (unsigned int)addr.led_addr);
}

static int dino_device_open(struct inode *tinode, struct file *tfile)
{
    if (driver_usage != 0)
        return -EBUSY;

#ifdef DEBUG
    printk("Device Opened\n");
#endif
    driver_usage = 1;
    return 0;
}

static int dino_device_release(struct inode *tinode, struct file *tfile)
{
    driver_usage = 0;
#ifdef DEBUG
    printk("Device Released\n");
#endif
    return 0;
}

// init module and ioremap devices
static int __init dino_device_init(void)
{
    int result;

    // register device
    result = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &dino_device_fops);
    if (result < 0)
    {
        printk("error %d\n", result);
        return result;
    }

    // for replacing mknod
    cls = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cls, NULL, MKDEV(DEVICE_MAJOR, DEVICE_MINOR), NULL, DEVICE_NAME);


    // map physical addr of devices to virtual kernel addr
    addr.fnd_addr = ioremap(IOM_FND_ADDRESS, 0x4);
    addr.dot_addr = ioremap(IOM_FPGA_DOT_ADDRESS, 0x10);
    addr.led_addr = ioremap(IOM_LED_ADDRESS, 0x1);
    addr.lcd_addr = ioremap(IOM_FPGA_TEXT_LCD_ADDRESS, 0x32);

#ifdef DEBUG
    printk("dev_file : /dev/%s , major : %d\n", DEVICE_NAME, DEVICE_MAJOR);
    printk("init module\n");
#endif
    init_board();
    
    write_dot(1);
    write_lcd(0);
    return 0;
}

// exit module and iounmap devices
static void __exit dino_device_exit(void)
{

    // unmap physical addr of devices and virtual kernel addr
    iounmap(addr.fnd_addr);
    iounmap(addr.dot_addr);
    iounmap(addr.led_addr);
    iounmap(addr.lcd_addr);

    // for replacing rmnod
    device_destroy(cls, MKDEV(DEVICE_MAJOR, DEVICE_MINOR));
    class_destroy(cls);

    /* Unregister the device */
    unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
#ifdef DEBUG
    printk("exit module\n");
#endif
}

module_init(dino_device_init);
module_exit(dino_device_exit);

MODULE_LICENSE("GPL");