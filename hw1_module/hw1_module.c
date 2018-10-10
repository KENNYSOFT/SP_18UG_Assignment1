#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>

#define procfs_name "hw1"
//#define JIFFIES_TO_NS(TIME) ((TIME) * (NSEC_PER_SEC / HZ))

unsigned long long system_uptime;
int total_task_num;

struct proc_dir_entry *proc_file;
struct timespec boot_time;
struct sysinfo s_info;
struct task_struct *task;

static void print_bar(struct seq_file *s)
{
	int i;
	for(i = 0; i < 120; ++i) seq_printf(s, "-");
	seq_printf(s, "\n");
}

static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
	static unsigned long counter = 0;
	if (*pos == 0) return &counter;
	else
	{
		*pos = 0;
		return NULL;
	}
}

static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	unsigned long *tmp_v = (unsigned long *)v;
	(*tmp_v)++;
	(*pos)++;
	return NULL;
}

static void my_seq_stop(struct seq_file *s, void *v)
{
}

static int my_seq_show(struct seq_file *s, void *v)
{
	print_bar(s);
	seq_printf(s, "CURRENT SYSTEM INFORMATION >\n");
	total_task_num = 0;
	for_each_process(task) total_task_num++;
	seq_printf(s, "Total %d task\n", total_task_num);
	//do_posix_clock_monotonic_gettime(&boot_time);
	//ktime_get_ts(&boot_time);
	//getboottime(&boot_time);
	system_uptime = jiffies_to_msecs(get_jiffies_64());
	seq_printf(s, "%dHz, %ld ms after system boot time\n", HZ, system_uptime /*boot_time.tv_sec * MSEC_PER_SEC + boot_time.tv_nsec / NSEC_PER_MSEC*/);
	print_bar(s);
	seq_printf(s, "%19s%8s%13s%13s%13s%13s\n", "command", "pid", "start(s)", "total(s)", "user(s)", "kernel(s)");
	print_bar(s);
	for_each_process(task)
	{
		//unsigned long gtime = JIFFIES_TO_NS(task->gtime);
		unsigned long utime = jiffies_to_msecs(task->utime);
		unsigned long stime = jiffies_to_msecs(task->stime);
		unsigned long gtime = utime + stime;
		seq_printf(s, "%19s%8d%9d.%03d%9d.%03d%9d.%03d%9d.%03d\n", task->comm, task->pid, task->start_time.tv_sec, task->start_time.tv_nsec / NSEC_PER_MSEC, gtime / MSEC_PER_SEC, gtime % MSEC_PER_SEC, utime / MSEC_PER_SEC, utime % MSEC_PER_SEC, stime / MSEC_PER_SEC, stime % MSEC_PER_SEC);
	}
	return 0;
}

static struct seq_operations my_seq_ops = {.start = my_seq_start, .next = my_seq_next, .stop = my_seq_stop, .show = my_seq_show};

static int my_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &my_seq_ops);
}

static struct file_operations my_file_ops = {.owner = THIS_MODULE, .open = my_open, .read = seq_read, .llseek = seq_lseek, .release = seq_release};

int init_module()
{
	struct proc_dir_entry *entry = create_proc_entry(procfs_name, 0, NULL);
	if (entry) entry->proc_fops = &my_file_ops;
	return 0;
}

void cleanup_module()
{
	remove_proc_entry(procfs_name, NULL);
}

/*
int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);
	system_uptime = sched_clock();
	//getboottime(&boot_time);
	if (offset > 0) ret = 0;
	else ret = sprintf(buffer, "Hello world! Ontime = %ld\n", system_uptime);
	return ret;
	for_each_process(task)
	{
	}
}

int init_module()
{
	proc_file = create_proc_entry(procfs_name, 0644, NULL);
	if (proc_file == NULL)
	{
		remove_proc_entry(procfs_name, &proc_root);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}
	proc_file->read_proc = procfile_read;
	proc_file->owner = THIS_MODULE;
	proc_file->mode = S_IFREG | S_IRUGO;
	proc_file->uid = 0;
	proc_file->gid = 0;
	proc_file->size = 37;
	printk(KERN_INFO "/proc/%s created\n", procfs_name);
	return 0;
}

void cleanup_module()
{
	remove_proc_entry(procfs_name, &proc_root);
	printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
*/
