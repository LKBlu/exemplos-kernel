// SPDX-License-Identifier: GPL-2.0-or-later
// Author: Marcos Paulo de Souza <marcos@mpdesouza.com>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/wait.h>

static DECLARE_WAIT_QUEUE_HEAD(waitq);
static char *msg = "ok";
#define BUFLEN strlen(msg)

static bool ready;

static ssize_t wait_read(struct file *f, char __user *buffer, size_t count,
			loff_t *fpos)
{
	/* The read position is bigger than what we provide, return 0 (EOF) */
	if (*fpos >= BUFLEN)
		return 0;

	/*
	 * If trying to read more data than we provide, cut the length to what
	 * we provide
	 */
	if (*fpos + count > BUFLEN) {
		pr_info("fpos %lld + count %ld > BUFLEN %lu, reducing count to %lu",
				*fpos, count, BUFLEN, BUFLEN);
		count = BUFLEN - *fpos;
	}

	if (wait_event_interruptible(waitq, ready))
		return -ERESTARTSYS;

	ready = false;
	if (copy_to_user(buffer, msg + *fpos, count))
		return -ERESTARTSYS;

	*fpos += count;

	pr_info("Read %lu bytes", count);
	return count;
}

static ssize_t wait_write(struct file *f, const char __user *buffer,
				size_t count, loff_t *fpos)
{
	ready = true;

	/* Wakes any thread that is being waiting */
	wake_up_interruptible(&waitq);

	/*
	 * Return the number of bytes asked to write just to make the caller
	 * happy.
	 */
	return count;
}

static const struct file_operations wait_fops = {
	.read = wait_read,
	.write = wait_write,
};

static struct miscdevice wait_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "wait_queue",
	.fops = &wait_fops,
	.mode = S_IRWXUGO,
};

module_misc_device(wait_misc);
MODULE_LICENSE("GPL");
