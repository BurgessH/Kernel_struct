#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

char hello_buf[100];
unsigned long hello_value;


static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
{
    return sprintf(buf, "hello_value = %lu\n", hello_value);
}
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count);
{
    char tmp_buf[10] = {0};
    strncpy(tmp_buf, buf, count);
    //*将字符串转换为无符号长整型（unsigned long）,将用户空间传递的字符串参数转换为内核可以使用的数值类型。
    hello_value = simple_strtoul(tmp_buf, NULL, 0);
    return count;
}
static struct kobj_attribute value_attribute = {
    .attr = {
        .name = "value",
        .mode = 0644,
    },
    .show = value_show,
    .store = value_store,
};

static ssize_t hello_buf_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
{
    strncpy(buf, hello_buf, strlen(hello_buf));
    return strlen(hello_buf);
}
static ssize_t hello_buf_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count);
{
    strncpy(hello_buf, buf, count);
    return count;
}

static struct kobj_attribute foo_attribute = __ATTR(buf, 0644, hello_buf_show, hello_buf_store);

static struct kobject *kobject_hello;
static struct kobject *kobject_child;

static int kobject_hello_init(void)
{
    int reval;

    kobject_hello =  kobject_create_and_add("hello", NULL);
    if(!kobj_hello)
    {
        return -ENOMEM;
    }
    reval = sysfs_create_file(kobject_hello, &foo_attribute.attr);
    if(reval)
    {
        printk(KERNEL_ALERT "%s: sysfs create file failed\n", __func__ );
        kobject_put(kobject_hello);
        return -1;
    }
    printk("kobject_hello.refcount = %d\n", kobject_hello->kref.refcount.refs.counter);


    reval = sysfs_create_file(kobject_hello, &value_attribute.attr);
    if(reval)
    {
        printk(KERNEL_ALERT "%s: sysfs create file failed\n", __func__ );
        kobject_put(kobject_hello);
        return -1;
    }
    printk("kobject_hello.refcount = %d\n", kobject_hello->kref.refcount.refs.counter);

    kobject_child = kobject_create_and_add("child", kobject_hello);
    if(!kobject_child)
    {
        printk(KERNEL_ALERT "%s: sysfs create file failed\n", __func__ );
        kobject_put(kobject_hello);
        return -ENOMEM;
    }

    printk("kobject_hello.refcount = %d\n", kobject_hello->kref.refcount.refs.counter);
    printk("kobject_hello.refcount = %d\n", kobject_child->kref.refcount.refs.counter);
    
    return 0;
}

static void kobject_hello_exit(void)
{
    kobject_put(kobject_hello);
    kobject_put(kobject_child);
}

module_init(kobject_hello_init);
module_exit(kobject_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("astonm@gmail.com");
MODULE_DESCRIPTION("create and add a okject to kernel");
