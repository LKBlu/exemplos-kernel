// SPDX-License-Identifier: GPL-2.0-or-later
// Author: Marcos Paulo de Souza <marcos@mpdesouza.com>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

static char symbol[KSYM_NAME_LEN] = "cmdline_proc_show";
module_param_string(symbol, symbol, KSYM_NAME_LEN, 0644);

static bool has_post_handler = true;
module_param(has_post_handler, bool, 0444);

static int __kprobes pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	pr_info("%s on %s\n", __func__, p->symbol_name);
	return 0;
}

static void __kprobes post_handler(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
	pr_info("%s on %s\n", __func__, p->symbol_name);
}

static struct kprobe kp = {
	.symbol_name	= symbol,
	.pre_handler = pre_handler,
};

static int __init kprobe_init(void)
{
	int ret;

	if (has_post_handler)
		kp.post_handler = post_handler;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}

	pr_info("Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
