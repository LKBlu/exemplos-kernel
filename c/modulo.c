#include <linux/module.h>
#include <linux/kernel.h>

static int modulo_init(void)
{
	pr_info("Init chamado! Hello world!\n");
	pr_info("Deste ponto em diante, o módulo está carregado.\n");
	pr_info("Verifique os modulos carregados com o comando lsmod\n");
	return 0;
}

static void modulo_exit(void)
{
	pr_info("Exit chamado! Tchau!\n");
	pr_info("Deste ponto em diante o comando lsmod nao deve mais mostrar este modulo carregado\n");
}

module_init(modulo_init);
module_exit(modulo_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Módulo de exemplo");
