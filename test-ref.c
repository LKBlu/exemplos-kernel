// SPDX-License-Identifier: GPL-2.0-or-later
// Author: Marcos Paulo de Souza <marcos@mpdesouza.com>
/*
 * Example of correct usage of module_get/put is order to avoid a crash when
 * executing a delayed work.
 *
 * If the module is loaded without any argument, handle_ref default value is 1,
 * which calls module_get before calling scheduling the delayed work, making it
 * impossible to rmmod the module before the delayed work is scheduled. The
 * delayed work calls module_put in the end.
 *
 * When handle_ref=0 is used, it's possible to remove the module before the
 * delayed work is scheduled, default to 10 seconds. When the delayed work is
 * called, it cannot find my_work_handler function, since it isn't available
 * anymore, and so a crash happens.
 */

#define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

#include <linux/module.h>
#include <linux/workqueue.h>

static int handle_ref = 1;
module_param(handle_ref, int, 0644);
MODULE_PARM_DESC(handle_ref, "Ensure the module to hold references until the delayed work is processed. Default enabled");

static void my_work_handler(struct work_struct *work)
{
	pr_info("I guess I didn't crash :)\n");

	if (handle_ref)
		module_put(THIS_MODULE);
}

DECLARE_DELAYED_WORK(delayed_func, my_work_handler);

static int __init test_ref_init(void)
{
	if (handle_ref) {
		__module_get(THIS_MODULE);
	} else {
		pr_warn("A delayed work will be called in 10 seconds.\n");
		pr_warn("Remove the module before it to crash the kernel\n");
	}

	schedule_delayed_work(&delayed_func, 10 * HZ);
	return 0;
}

static void test_ref_exit(void)
{
}

module_init(test_ref_init);
module_exit(test_ref_exit);
MODULE_LICENSE("GPL");
