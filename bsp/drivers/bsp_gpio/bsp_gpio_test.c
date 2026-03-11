#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init test_init(void)
{
    printk(KERN_INFO "Test module loaded\n");
    return 0;
}

static void __exit test_exit(void)
{
    printk(KERN_INFO "Test module unloaded\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Test");
MODULE_DESCRIPTION("A simple test module");