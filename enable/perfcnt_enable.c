#include <linux/module.h>
#include <linux/kernel.h>

static int OldState;

int __init init_module(void)
{

  /* enable user-mode access */
  asm ("MCR p15, 0, %0, C9, C14, 0\n\t" :: "r"(1));

  /* disable counter overflow interrupts (just in case)*/
  asm ("MCR p15, 0, %0, C9, C14, 2\n\t" :: "r"(0x8000000f));

  printk(KERN_INFO "user-mode access to performance registers enabled\n");

  return 0;
}


void cleanup_module(void)
{
}
