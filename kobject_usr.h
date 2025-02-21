
//*struct kobject 数据结构：（对应sysfs-内核空间下）的一个目录
	//*struct kobj_attribute 对应kobject目录下的一个文件

 //*目录：
struct kobject {
	const char		    *name;
	struct list_head	entry;
	struct kobject		*parent;
	struct kset		    *kset;
	struct kobj_type	*ktype;
	struct kernfs_node	*sd;  

	struct kref		    kref; //引用计数

#ifdef CONFIG_DEBUG_KOBJECT_RELEASE
	struct delayed_work	release;
#endif
	unsigned int state_initialized:1;
	unsigned int state_in_sysfs:1;
	unsigned int state_add_uevent_sent:1;
	unsigned int state_remove_uevent_sent:1;
	unsigned int uevent_suppress:1;
};

struct kobj_type {
    void (*release)(struct kobject *kobj);
    const struct sysfs_ops *sysfs_ops;
    const struct attribute_group **default_groups;
    const struct kobj_ns_type_operations *(*child_ns_type)(struct kobject *kobj);
    const void *(*namespace)(struct kobject *kobj);
};






//* 属性：
struct attribute {
	const char		*name;
	umode_t			mode;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	bool			ignore_lockdep:1;
	struct lock_class_key	*key;
	struct lock_class_key	skey;
#endif
};

struct kobj_attribute {
	struct attribute attr;
	ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
	ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
};

#define __ATTR(_name, _mode, _show, _store) { \
    .attr = {.name = __stringify(_name), .mode = _mode }, \
    .show = _show, \
    .store = _store, \
}


struct bin_attribute {
	struct attribute	attr;
	size_t			size;
	void			*private;
	ssize_t (*read)(struct file *, struct kobject *, struct bin_attribute *, char *, loff_t, size_t);
	ssize_t (*write)(struct file *, struct kobject *, struct bin_attribute *, char *, loff_t, size_t);
	int (*mmap)(struct file *, struct kobject *, struct bin_attribute *attr, struct vm_area_struct *vma);
};

//*ksetkset 是一个重要的数据结构，用于管理一组内核对象（kobject）
struct kset {
    struct list_head list;        // 用于链接所有属于该kset的kobject
    spinlock_t list_lock;         // 保护list的自旋锁
    struct kobject kobj;          // 嵌入的kobject，表示kset本身
    const struct kset_uevent_ops *uevent_ops; // 用于处理uevent的回调函数
};






//*kobject 接口定义：
	struct kobject * __must_check kobject_create(void);
	struct kobject * __must_check kobject_create_and_add(const char *name, struct kobject *parent);

	void kobject_init(struct kobject *kobj, struct kobj_type *ktype);
	int kobject_add(struct kobject *kobj,   struct kobject *parent, const char *fmt, ...);
	int kobject_init_and_add(struct kobject *kobj, struct kobj_type *ktype, struct kobject *parent, const char *fmt, ...);

	void kobject_del(struct kobject *kobj);
	struct kobject *kobject_get(struct kobject *kobj);
	void kobject_put(struct kobject *kobj);



//*kobj_attribute 接口定义：
	static inline int __must_check sysfs_create_file(struct kobject *kobj, const struct attribute *attr)
	static inline void sysfs_remove_file(struct kobject *kobj, const struct attribute *attr)

	int __must_check sysfs_create_bin_file(struct kobject *kobj, const struct bin_attribute *attr);
	void sysfs_remove_bin_file(struct kobject *kobj, const struct bin_attribute *attr);

	int __must_check sysfs_create_group(struct kobject *kobj,const struct attribute_group *grp);
	void sysfs_remove_group(struct kobject *kobj, const struct attribute_group *grp);



//*kset 接口定义：
void kset_init(struct kset *kset);
int __must_check kset_register(struct kset *kset);
void kset_unregister(struct kset *kset);
struct kset * __must_check kset_create_and_add(const char *name, const struct kset_uevent_ops *u, struct kobject *parent_kobj);

//* sysfs文件系统与kobject关联：