#include"declerations.h"

int gpio_ctrl(int x) {
	printf("Begin %s \n",__func__);
	int fd;
	struct gpiohandle_request led;
	struct gpiohandle_data data;

	/* Open the gpiochip device file */
	fd = open("/dev/gpiochip0", O_RDWR);
	if(fd < 0) {
		perror("Error opening gpiochip0");
		return -1;
	}

	/* Setup LED to output */
	led.flags = GPIOHANDLE_REQUEST_OUTPUT;
	strcpy(led.consumer_label, "LED");
	memset(led.default_values, 0, sizeof(led.default_values));
	led.lines = 1;
	led.lineoffsets[0] = 25;

	if(ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &led) < 0) {
		perror("Error setting GPIO 25 to output");
		close(fd);
		return -1;
	}
	/* Setup button to input */
/*	button.flags = GPIOHANDLE_REQUEST_INPUT;
	strcpy(button.consumer_label, "LED");
	memset(button.default_values, 0, sizeof(button.default_values));
	button.lines = 1;
	button.lineoffsets[0] = 20;

	if(ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &button) < 0) {
		perror("Error setting GPIO 20 to input");
		close(led.fd);
		close(fd);
		return -1;
	}
*/
	/* Let's set the LED */
	data.values[0] = x;
	if(ioctl(led.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0) 
		perror("Error setting GPIO to 1");

	/* Reading button's state */
	//if(ioctl(button.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0) 
	//	perror("Error setting GPIO to 1");

	//printf("Button is %s\n", (data.values[0] > 0) ? "pressed" : "not pressed");

//	sleep(10);

	close(fd);
	close(led.fd);
	//close(button.fd);
	printf(" Exit %s \n",__func__);
	return 0;
}
