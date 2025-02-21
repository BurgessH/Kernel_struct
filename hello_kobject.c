
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

static struct kobject *kobject_hello;


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

struct sysfs_ops buf_sysfs_ops ={
    .show = hello_buf_show,
    .store = hello_buf_store,
};


static struct attribute buf_attr ={
    .name = "buf",
    .mode = "0644",
};

static struct attribute *buf_attr_arry[]{
    &buf_attr,
    NULL,
};


static ssize_t value_buf_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
{
    strncpy(buf, value_buf, strlen(value_buf));
    return strlen(value_buf);
}
static ssize_t value_buf_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count);
{
    strncpy(value_buf, buf, count);
    return count;
}

struct sysfs_ops value_sysfs_ops ={
    .show = value_buf_show,
    .store = value_buf_store,
};

static struct attribute value_attr ={
    .name = "buf",
    .mode = "0644",
};

static struct attribute *value_attr_arry[]{
    &value_attr,
    NULL,
};


void my_obj_relese(struct kobject *kobj)
{
    printk("%s: kfree %s\n", __func__, kobj->name);
    kfree(kobj);
}

struct kobject *kobj_hello;
static struct kset *kset_hello;

struct kobject *kobj_value, *kobj_buf;
static struct kobj_type value_type, buf_type;




//* value buf kobject的kset指向kset_hello,运行后在/sys/目录下hello目录、kset_hello目录；
//* kset_hello目录下生成buf、value两个文件,且根据属性下创建buf、value两个属性文件，可以对文件操作读和写属性；
//*
value_type.relese           = my_obj_relese;
static int kobject_hello_init(void)
{
    int retval;

    //*1.创建kobject_hello
    kobject_hello =  kobject_create_and_add("hello", NULL);
    if(!kobj_hello)
    {
        return -ENOMEM;
    }
    
    //*2.kset_hello
    kset_hello=kset_create_and_add("kset_hello",NULL, NULL); //*kset 未指定父节点 kobject*parent在顶层目录下创建；指定后在该目录下创建kset_hello目录
    if(!kset_hello)
    {
        kobject_put(kobj_hello);
        return -ENOMEM;
    }

    kobj_value = kzalloc(sizeof(struct kobject), GFP_KERNEL)
    kobj_value->kset            = kset_hello;   
    value_type.default_attrs    = value_attr_arry;
    value_type.sysfs_ops        = &value_sysfs_ops;

    kobj_buf                    = kzalloc(sizeof(struct kobject), GFP_KERNEL)
    kobj_buf->kset              = kset_hello;
    buf_type.relese             = my_obj_relese;
    buf_type.default_attrs      = buf_attr_arry;
    buf_type.sysfs_ops          = &buf_sysfs_ops; 
    
    retval = kobject_init_and_add(kobj_value, &value_type, NULL, "value");//* kobject未指定父节点，在当前目录下创建

    retval = kobject_init_and_add(kobj_buf, &buf_type, NULL, "buf");
     
return 0;
}

static void kobject_hello_exit(void)
{
    kobj_del(kobj_value);
    kobject_put(kobj_value);
    kobject_put(kobj_buf);

    kobj_del(kobject_hello);
    kobject_put(kobject_hello);
    kset_unregister(kset_hello);
}

module_init(kobject_hello_init);
module_exit(kobject_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("astonm@gmail.com");
MODULE_DESCRIPTION("create and add a okject to kernel");