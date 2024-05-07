/*
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
*/

#include"declerations.h"

uint8_t mode = 0;
uint32_t speed = 2000000;
uint8_t bits = 8;

/**
 * @brief function makes a SPI transfer
 *
 * @param fd		File descriptor to SPI bus
 * @param data		Data array with output (write) data
 *					will be overwritten with the received input data
 *	@param length	Length of the data array
 */
void spi_transfer(int fd, uint8_t *data, int length) {

	gpio_ctrl(0);
	struct spi_ioc_transfer spi[length];
	int i;

	/* Setup transfer struct */
	for (i=0; i<length; i++) {
		memset(&spi[i], 0, sizeof(struct spi_ioc_transfer));
		spi[i].tx_buf = (unsigned long) (data+i);
		spi[i].rx_buf = (unsigned long) (data+i);
		spi[i].len = 1;
		spi[i].speed_hz = speed;
		spi[i].bits_per_word = bits;
	}

	/* Let's do the transfer */
	if(ioctl(fd, SPI_IOC_MESSAGE(length), spi) < 0) {
		perror("Error transfering data over SPI bus");
		close(fd);
		exit(-1);
	}
	gpio_ctrl(1);
}

int main(int argc, char * argv[]) {

	printf("%s Begin \n",__func__);
	
	int fd;
	uint8_t data[2];

	/* Open the SPI bus file descriptor */
	fd = open("/dev/spidev0.0", O_RDWR);
	if(fd < 0) {
		perror("Error opening SPI Bus");
		return -1;
	}

	/* Setup of the SPI Bus */
	if(ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
		perror("Error setting the SPI mode");
		close(fd);
		return -1;
	}
	if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
		perror("Error setting the SPI speed");
		close(fd);
		return -1;
	}
	if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
		perror("Error setting the SPI wordlength");
		close(fd);
		return -1;
	}

	/* Ready for SPI accesses */

	/* Setup GPIO's direction */
	gpio_ctrl(0);
	data[0] = 0x0C;
	data[1] = 0x01;
	spi_transfer(fd, data, 2);

	data[0] = 0x0B;
	data[1] = 0x07;
	spi_transfer(fd, data, 2);

	data[0] = 0x0A;
	data[1] = 0x00;
	spi_transfer(fd, data, 2);

	data[0] = 0x09;
	data[1] = 0x00;
	spi_transfer(fd, data, 2);

	data[0] = 0x0F;
	data[1] = 0x00;
	spi_transfer(fd, data, 2);

	data[0] = 0x01;
	data[1] = 0xF0;
	spi_transfer(fd, data,2);


	data[0] = 0x02;
	data[1] = 0x88;
	spi_transfer(fd, data, 2);


	data[0] = 0x03;
	data[1] = 0x88;
	spi_transfer(fd, data, 2);

	data[0] = 0x04;
	data[1] = 0xF0;
	spi_transfer(fd, data, 2);


	data[0] = 0x05;
	data[1] = 0x88;
	spi_transfer(fd, data, 2);

	data[0] = 0x06;
	data[1] = 0x88;
	spi_transfer(fd, data, 2);

	data[0] = 0x07;
	data[1] = 0x88;
	spi_transfer(fd, data, 2);

	data[0] = 0x08;
	data[1] = 0xF0;
	spi_transfer(fd, data, 2);

	close(fd); 
	return 0;
}
