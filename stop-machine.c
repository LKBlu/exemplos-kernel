// SPDX-License-Identifier: GPL-2.0-or-later
// Author: Marcos Paulo de Souza <marcos@mpdesouza.com>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/stop_machine.h>

static int run_stop_machine(void *)
{
	pr_info("Running on cpu %d while the others are stopped\n",
			smp_processor_id());
	// Run msleep here would trigger a bug because we are scheduling while
	// atomic
	//msleep(5000);
	pr_info("Done!\n");
	return 0;
}

static int stop_machine_init(void)
{
	stop_machine(run_stop_machine, NULL, NULL);

	return 0;
}

static void stop_machine_exit(void)
{
}

module_init(stop_machine_init);
module_exit(stop_machine_exit);
MODULE_LICENSE("GPL");
