KDIR ?= /lib/modules/$(shell uname -r)/build

atomic-m := atomic_main.o atomic_helper.o

obj-m := test-ref.o mod-notify.o stop-machine.o lists.o kprobe.o ftrace-hijack.o wait-queue.o atomic.o modulo.o

all:
	make LLVM=1 W=1 -C $(KDIR) M=$(PWD) modules

clean:
	make LLVM=1 W=1 -C $(KDIR) M=$(PWD) clean
