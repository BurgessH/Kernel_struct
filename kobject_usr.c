
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

static struct *kobject_hello;

static int kobject_hello_init(void)
{
    kobject_hello =  kobject_create_and_add("hello", kobj_kernel);
    if(!kobj_hello)
    {
        return -ENOMEM;
    }
}

static void kobject_hello_exit(void)
{
    kobject_put(kobject_hello);
}
module_init(kobject_hello_init);
module_exit(kobject_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("astonm@gmail.com");
MODULE_DESCRIPTION("create and add a okject to kernel");