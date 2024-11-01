// SPDX-License-Identifier: GPL-2.0-or-later
// Author: Marcos Paulo de Souza <marcos@mpdesouza.com>

#include <linux/ftrace.h>
#include <linux/module.h>
#include <linux/seq_file.h>

static char symbol[KSYM_NAME_LEN] = "cmdline_proc_show";
module_param_string(symbol, symbol, KSYM_NAME_LEN, 0644);

static int another_proc_show(struct seq_file *m, void *v)
{
	pr_info("New stack:\n");
	dump_stack();

        seq_printf(m, "%s\n", "Hijacked!");
        return 0;
}

static void callback_func(unsigned long ip, unsigned long parent_ip,
                   struct ftrace_ops *op, struct ftrace_regs *regs)
{
	pr_info("callback_func called. Ignoring %s and calling our own function (like a livepatch)!!\n", symbol);
	pr_info("Current stack:\n");
	dump_stack();

	ftrace_regs_set_instruction_pointer(regs, (unsigned long)another_proc_show);
}

struct ftrace_ops ops = {
	.func = callback_func,
	.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_IPMODIFY | FTRACE_OPS_FL_PERMANENT,
};

static int init_ftrace_hijack(void)
{
	ftrace_set_filter(&ops, symbol, strlen(symbol), 1);
	register_ftrace_function(&ops);

	return 0;
}

static void exit_ftrace_hijack(void)
{
	unregister_ftrace_function(&ops);
}

module_init(init_ftrace_hijack);
module_exit(exit_ftrace_hijack);
MODULE_LICENSE("GPL");
