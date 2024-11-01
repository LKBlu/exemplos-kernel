// SPDX-License-Identifier: GPL-2.0-or-later
// Author: Marcos Paulo de Souza <marcos@mpdesouza.com>
#include <linux/module.h>
#include <linux/kernel.h>

#include "atomic_helper.h"

static int mod_init(void)
{
	pr_info("Checking atomic operations\n");

	counter_inc();

	pr_info("counter is non zero? %d\n", counter_nonzero());

	counter_dec();

	pr_info("counter is non zero? %d\n", counter_nonzero());

	return 0;
}

static void mod_exit(void)
{
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
