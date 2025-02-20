
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
	struct kref		    kref;

#ifdef CONFIG_DEBUG_KOBJECT_RELEASE
	struct delayed_work	release;
#endif
	unsigned int state_initialized:1;
	unsigned int state_in_sysfs:1;
	unsigned int state_add_uevent_sent:1;
	unsigned int state_remove_uevent_sent:1;
	unsigned int uevent_suppress:1;
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




