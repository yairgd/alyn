/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/dma.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#include "shell.h"


#include <zephyr/fs/fs.h>
#include <zephyr/drivers/gpio/gpio_emul.h>

#include <stdlib.h>

#include "lauxlib.h"
#include "lualib.h"
#include "hwctl.h"
#include "system_model.h"
#include "game.h"
#include "display.h"

#include "effects/src/led_matrix.h"
int _open() {
	return 0;
}


int _link() {
	return 0;
}
int _unlink() {
	return 0;
}
int _times() {
	return 0;
}

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/fs/fs.h>
//#include <zephyr/fs/littlefs.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(main);

#if 0
/* Matches LFS_NAME_MAX */
#define MAX_PATH_LEN 255
#define TEST_FILE_SIZE 547

static uint8_t file_test_pattern[TEST_FILE_SIZE];
static int lsdir(const char *path)
{
	int res;
	struct fs_dir_t dirp;
	static struct fs_dirent entry;

	fs_dir_t_init(&dirp);

	/* Verify fs_opendir() */
	res = fs_opendir(&dirp, path);
	if (res) {
		LOG_ERR("Error opening dir %s [%d]\n", path, res);
		return res;
	}

	LOG_PRINTK("\nListing dir %s ...\n", path);
	for (;;) {
		/* Verify fs_readdir() */
		res = fs_readdir(&dirp, &entry);

		/* entry.name[0] == 0 means end-of-dir */
		if (res || entry.name[0] == 0) {
			if (res < 0) {
				LOG_ERR("Error reading dir [%d]\n", res);
			}
			break;
		}

		if (entry.type == FS_DIR_ENTRY_DIR) {
			LOG_PRINTK("[DIR ] %s\n", entry.name);
		} else {
			LOG_PRINTK("[FILE] %s (size = %zu)\n",
				   entry.name, entry.size);
		}
	}

	/* Verify fs_closedir() */
	fs_closedir(&dirp);

	return res;
}

static int littlefs_increase_infile_value(char *fname)
{
	uint8_t boot_count = 0;
	struct fs_file_t file;
	int rc, ret;

	fs_file_t_init(&file);
	rc = fs_open(&file, fname, FS_O_CREATE | FS_O_RDWR);
	if (rc < 0) {
		LOG_ERR("FAIL: open %s: %d", fname, rc);
		return rc;
	}

	rc = fs_read(&file, &boot_count, sizeof(boot_count));
	if (rc < 0) {
		LOG_ERR("FAIL: read %s: [rd:%d]", fname, rc);
		goto out;
	}
	LOG_PRINTK("%s read count:%u (bytes: %d)\n", fname, boot_count, rc);

#if 0
	rc = fs_seek(&file, 0, FS_SEEK_SET);
	if (rc < 0) {
		LOG_ERR("FAIL: seek %s: %d", fname, rc);
		goto out;
	}
#endif
	boot_count += 1;
	for (int i = 0;i<200;i++) { 
		rc = fs_write(&file, &boot_count, sizeof(boot_count));
	if (rc < 0) {
		LOG_ERR("FAIL: write %s: %d", fname, rc);
		goto out;
	}
	}
	LOG_PRINTK("%s write new boot count %u: [wr:%d]\n", fname,
		   boot_count, rc);

 out:
	ret = fs_close(&file);
	if (ret < 0) {
		LOG_ERR("FAIL: close %s: %d", fname, ret);
		return ret;
	}

	return (rc < 0 ? rc : 0);
}

static void incr_pattern(uint8_t *p, uint16_t size, uint8_t inc)
{
	uint8_t fill = 0x55;

	if (p[0] % 2 == 0) {
		fill = 0xAA;
	}

	for (int i = 0; i < (size - 1); i++) {
		if (i % 8 == 0) {
			p[i] += inc;
		} else {
			p[i] = fill;
		}
	}

	p[size - 1] += inc;
}

static void init_pattern(uint8_t *p, uint16_t size)
{
	uint8_t v = 0x1;

	memset(p, 0x55, size);

	for (int i = 0; i < size; i += 8) {
		p[i] = v++;
	}

	p[size - 1] = 0xAA;
}

static void print_pattern(uint8_t *p, uint16_t size)
{
	int i, j = size / 16, k;

	for (k = 0, i = 0; k < j; i += 16, k++) {
		LOG_PRINTK("%02x %02x %02x %02x %02x %02x %02x %02x ",
			   p[i], p[i+1], p[i+2], p[i+3],
			   p[i+4], p[i+5], p[i+6], p[i+7]);
		LOG_PRINTK("%02x %02x %02x %02x %02x %02x %02x %02x\n",
			   p[i+8], p[i+9], p[i+10], p[i+11],
			   p[i+12], p[i+13], p[i+14], p[i+15]);

		/* Mark 512B (sector) chunks of the test file */
		if ((k + 1) % 32 == 0) {
			LOG_PRINTK("\n");
		}
	}

	for (; i < size; i++) {
		LOG_PRINTK("%02x ", p[i]);
	}

	LOG_PRINTK("\n");
}

static int littlefs_binary_file_adj(char *fname)
{
	struct fs_dirent dirent;
	struct fs_file_t file;
	int rc, ret;

	/*
	 * Uncomment below line to force re-creation of the test pattern
	 * file on the littlefs FS.
	 */
	/* fs_unlink(fname); */
	fs_file_t_init(&file);

	rc = fs_open(&file, fname, FS_O_CREATE | FS_O_APPEND);
	if (rc < 0) {
		LOG_ERR("FAIL: open %s: %d", fname, rc);
		return rc;
	}

	rc = fs_stat(fname, &dirent);
	if (rc < 0) {
		LOG_ERR("FAIL: stat %s: %d", fname, rc);
		goto out;
	}

	/* Check if the file exists - if not just write the pattern */
	if (rc == 0 && dirent.type == FS_DIR_ENTRY_FILE && dirent.size == 0) {
		LOG_INF("Test file: %s not found, create one!",
			fname);
		init_pattern(file_test_pattern, sizeof(file_test_pattern));
	} else {
		rc = fs_read(&file, file_test_pattern,
			     sizeof(file_test_pattern));
		if (rc < 0) {
			LOG_ERR("FAIL: read %s: [rd:%d]",
				fname, rc);
			goto out;
		}
		incr_pattern(file_test_pattern, sizeof(file_test_pattern), 0x1);
	}

	LOG_PRINTK("------ FILE: %s ------\n", fname);
	print_pattern(file_test_pattern, sizeof(file_test_pattern));

	rc = fs_seek(&file, 0, FS_SEEK_SET);
	if (rc < 0) {
		LOG_ERR("FAIL: seek %s: %d", fname, rc);
		goto out;
	}

	rc = fs_write(&file, file_test_pattern, sizeof(file_test_pattern));
	if (rc < 0) {
		LOG_ERR("FAIL: write %s: %d", fname, rc);
	}

 out:
	ret = fs_close(&file);
	if (ret < 0) {
		LOG_ERR("FAIL: close %s: %d", fname, ret);
		return ret;
	}

	return (rc < 0 ? rc : 0);
}


#define CONFIG_APP_LITTLEFS_STORAGE_FLASH
#ifdef CONFIG_APP_LITTLEFS_STORAGE_FLASH
static int littlefs_flash_erase(unsigned int id)
{
	const struct flash_area *pfa;
	int rc;

	rc = flash_area_open(id, &pfa);
	if (rc < 0) {
		LOG_ERR("FAIL: unable to find flash area %u: %d\n",
			id, rc);
		return rc;
	}

	LOG_PRINTK("Area %u at 0x%x on %s for %u bytes\n",
		   id, (unsigned int)pfa->fa_off, pfa->fa_dev->name,
		   (unsigned int)pfa->fa_size);

	/* Optional wipe flash contents */
	if (IS_ENABLED(CONFIG_APP_WIPE_STORAGE)) {
		rc = flash_area_erase(pfa, 0, pfa->fa_size);
		LOG_ERR("Erasing flash area ... %d", rc);
	}

	flash_area_close(pfa);
	return rc;
}
#define PARTITION_NODE DT_NODELABEL(lfs1)

#if DT_NODE_EXISTS(PARTITION_NODE)
FS_FSTAB_DECLARE_ENTRY(PARTITION_NODE);
#else /* PARTITION_NODE */
FS_LITTLEFS_DECLARE_DEFAULT_CONFIG(storage);
static struct fs_mount_t lfs_storage_mnt = {
	.type = FS_LITTLEFS,
	.fs_data = &storage,
	.storage_dev = (void *)FIXED_PARTITION_ID(storage_partition),
	.mnt_point = "/lfs",
};
#endif /* PARTITION_NODE */

	struct fs_mount_t *mp =
#if DT_NODE_EXISTS(PARTITION_NODE)
		&FS_FSTAB_ENTRY(PARTITION_NODE)
#else
		&lfs_storage_mnt
#endif
		;

static int littlefs_mount(struct fs_mount_t *mp)
{
	int rc;

	rc = littlefs_flash_erase((uintptr_t)mp->storage_dev);
	if (rc < 0) {
		return rc;
	}

	/* Do not mount if auto-mount has been enabled */
#if !DT_NODE_EXISTS(PARTITION_NODE) ||						\
	!(FSTAB_ENTRY_DT_MOUNT_FLAGS(PARTITION_NODE) & FS_MOUNT_FLAG_AUTOMOUNT)
	rc = fs_mount(mp);
	if (rc < 0) {
		LOG_PRINTK("FAIL: mount id %" PRIuPTR " at %s: %d\n",
		       (uintptr_t)mp->storage_dev, mp->mnt_point, rc);
		return rc;
	}
	LOG_PRINTK("%s mount: %d\n", mp->mnt_point, rc);
#else
	LOG_PRINTK("%s automounted\n", mp->mnt_point);
#endif

	return 0;
}
#endif /* CONFIG_APP_LITTLEFS_STORAGE_FLASH */

#ifdef CONFIG_APP_LITTLEFS_STORAGE_BLK_SDMMC
struct fs_littlefs lfsfs;
static struct fs_mount_t __mp = {
	.type = FS_LITTLEFS,
	.fs_data = &lfsfs,
	.flags = FS_MOUNT_FLAG_USE_DISK_ACCESS,
};
struct fs_mount_t *mp = &__mp;

static int littlefs_mount(struct fs_mount_t *mp)
{
	static const char *disk_mount_pt = "/"CONFIG_SDMMC_VOLUME_NAME":";
	static const char *disk_pdrv = CONFIG_SDMMC_VOLUME_NAME;

	mp->storage_dev = (void *)disk_pdrv;
	mp->mnt_point = disk_mount_pt;

	return fs_mount(mp);
}
#endif /* CONFIG_APP_LITTLEFS_STORAGE_BLK_SDMMC */


#endif
int main1(void)
{
#if 0
	char fname1[MAX_PATH_LEN];
	char fname2[MAX_PATH_LEN];
	char fname3[MAX_PATH_LEN];

	struct fs_statvfs sbuf;
	int rc;

	LOG_PRINTK("Sample program to r/w files on littlefs\n");

	rc = littlefs_mount(mp);
	if (rc < 0) {
	//	return 0;
	}

	snprintf(fname1, sizeof(fname1), "%s/boot_count", mp->mnt_point);
	snprintf(fname2, sizeof(fname2), "%s/pattern.bin", mp->mnt_point);
	snprintf(fname3, sizeof(fname2), "%s/pattern11.bin", mp->mnt_point);

	rc = fs_statvfs(mp->mnt_point, &sbuf);
	if (rc < 0) {
		LOG_PRINTK("FAIL: statvfs: %d\n", rc);
		goto out;
	}

	printf("%s: bsize = %lu ; frsize = %lu ;"
		   " blocks = %lu ; bfree = %lu\n",
		   mp->mnt_point,
		   sbuf.f_bsize, sbuf.f_frsize,
		   sbuf.f_blocks, sbuf.f_bfree);

	rc = lsdir(mp->mnt_point);
	if (rc < 0) {
		LOG_PRINTK("FAIL: lsdir %s: %d\n", mp->mnt_point, rc);
		goto out;
	}

	rc = littlefs_increase_infile_value(fname1);
	if (rc) {
		goto out;
	}

	rc = littlefs_binary_file_adj(fname2);
	if (rc) {
		goto out;
	}



	for (int i=0;i<0;i++) {
	snprintf(fname3, sizeof(fname2), "%s/pattern-%d.bin", mp->mnt_point,i);
	rc = littlefs_binary_file_adj(fname3);
	if (rc) {
		goto out;
	}
	}
out:

printf("%s: bsize = %lu ; frsize = %lu ;"
		   " blocks = %lu ; bfree = %lu\n",
		   mp->mnt_point,
		   sbuf.f_bsize, sbuf.f_frsize,
		   sbuf.f_blocks, sbuf.f_bfree);


	//rc = fs_unmount(mp);
	//LOG_PRINTK("%s unmount: %d\n", mp->mnt_point, rc);
#endif
	game_init();
	init_shell();
	
	return 0;
}

#include "luasrc.h"




//https://blog.devgenius.io/how-to-add-your-c-library-in-lua-46fd246f0fa8
//lua_State *L;
int main_org(void)
{


	

	led_matrix_init(led_matrix_get(), 64,32);

	game_init();
	init_shell();
	

	uint32_t millis_counter = 0;


	//https://chat.openai.com/c/540b150d-0eea-49ae-8f3d-a4fed08e1012
    while (1) {
        // Get the current uptime in milliseconds
//        uint32_t current_time = k_uptime_get();
        
        // Print the current time (optional)
      //  printk("Current Time: %u ms\n", current_time);

        // Do other tasks if needed

        // Wait for 1 millisecond
        k_sleep(K_MSEC(1000));

        // Increment the millisecond counter
        millis_counter++;
    }


	return 0;
}


// -----------------------spi
//https://community.st.com/t5/stm32-mcus-embedded-software/use-spi-on-stm32f410rb-via-zephyr/m-p/397545
//#include <zephyr.h>
//#include <device.h>
//#include <drivers/spi.h>

/* Define the SPI device */
#define SPI_DEVICE_NAME DT_LABEL(DT_ALIAS(spi_screen))

/* Define the DMA channel */
#define DMA_DEVICE_NAME DT_LABEL(DT_ALIAS(dma_screen))

/* DMA configuration */
#define DMA_CHANNEL 1

/* Buffer size */
#define BUFFER_SIZE 256

//static uint8_t tx_buffer[BUFFER_SIZE]={1,2,3,4,5,6,7,8,9};
//static uint8_t rx_buffer[BUFFER_SIZE];

static struct k_sem spi_sem;

void spi_dma_callback(const struct device *dev, void *user_data, uint32_t channel, int status)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(user_data);
    ARG_UNUSED(channel);

    if (status != 0) {
        printk("SPI DMA Transfer failed with status %d\n", status);
        return;
    }

    /* SPI DMA transfer completed */
    k_sem_give(&spi_sem);
}
#if 0
void main11(void)
{
    const struct device *spi_dev;
    const struct device *dma_dev;
    struct spi_config spi_cfg = {
        .frequency = 1000000,
        .operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8),
        .slave = 0,
        .cs = NULL
    };
    struct spi_buf tx_buf = {
        .buf = tx_buffer,
        .len = BUFFER_SIZE
    };
    struct spi_buf rx_buf = {
        .buf = rx_buffer,
        .len = BUFFER_SIZE
    };
    struct spi_buf_set tx_rx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };

    spi_dev = device_get_binding(SPI_DEVICE_NAME);
    if (!spi_dev) {
        printk("Cannot find SPI device!\n");
        return;
    }

    printk("SPI start!\n");
#if 0    
    dma_dev = device_get_binding(DMA_DEVICE_NAME);
    if (!dma_dev) {
        printk("Cannot find DMA device!\n");
        return;
    }

    /* Initialize semaphore */
    k_sem_init(&spi_sem, 0, 1);

    /* Set up SPI DMA configuration */
    struct dma_config dma_cfg = {
        .channel_direction = MEMORY_TO_PERIPHERAL,
        //.source_transfer_width = 1,
        //.dest_transfer_width = 1,
        .source_burst_length = 1,
        .dest_burst_length = 1,
        .complete_callback_en = 1,
        .complete_callback_en = spi_dma_callback,
    };

    /* Set up DMA channel */
    if (dma_config(dma_dev, DMA_CHANNEL, &dma_cfg) != 0) {
        printk("DMA channel configuration failed!\n");
        return;
    }
#endif
    /* Perform SPI DMA transfer */
  //  if (spi_transceive_dma(spi_dev, &spi_cfg, &tx_rx_bufs, 1, DMA_CHANNEL) != 0) {
    if (spi_write(spi_dev, &spi_cfg, &tx_rx_bufs) != 0) {

        printk("SPI DMA transfer failed!\n");
       // return;
    }

    printk("SPI stop!\n");

    /* Wait for transfer to complete */
    k_sem_take(&spi_sem, K_FOREVER);

    printk("SPI DMA transfer completed successfully!\n");
}
#endif

#if 0
void main(void)
{
//	game_init();
//	led_matrix_init(led_matrix_get(), 64,32);

//	display__init();
}
#endif
