// SPDX-License-Identifier: GPL-2.0-or-later
// Author: Marcos Paulo de Souza <marcos@mpdesouza.com>
/* Example of module notification mechanism */
#define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

#include <linux/module.h>

static int module_notify(struct notifier_block *nb, unsigned long action,
		void *data)
{
	struct module *mod = data;

	if (action == MODULE_STATE_LIVE)
		pr_warn("Module running: %s\n", mod->name);
	else if (action == MODULE_STATE_COMING)
		pr_warn("Module on module_init: %s\n", mod->name);
	else if (action == MODULE_STATE_GOING)
		pr_warn("Module being removed: %s\n", mod->name);
	else if (action == MODULE_STATE_UNFORMED)
		pr_warn("Module still on setup: %s\n", mod->name);

	return 0;
}

static struct notifier_block module_nb= {
	.notifier_call = module_notify,
	.priority = INT_MIN + 1,
};

static int mod_notify_init(void)
{
	int ret;

	ret = register_module_notifier(&module_nb);
	if (ret)
		return ret;

	return 0;
}

static void mod_notify_exit(void)
{
	unregister_module_notifier(&module_nb);
}

module_init(mod_notify_init);
module_exit(mod_notify_exit);
MODULE_LICENSE("GPL");
