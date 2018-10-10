#include <linux/kernel.h> // for kernel
#include <linux/module.h> // for module
#include <linux/proc_fs.h> // for proc fs
#include <linux/seq_file.h> // for seq_file
#include <linux/sched.h> // for task_struct
#include <linux/jiffies.h> // for get_jiffies_64()
#include <linux/time.h> // for jiffies_to_msecs()

#define procfs_name "hw1" // for /proc/hw1

struct proc_dir_entry *proc_file; // for proc fs

static void print_bar(struct seq_file *s)
{
	int i;
	for (i = 0; i < 120; ++i) seq_printf(s, "-");
	seq_printf(s, "\n");
}

static void *my_seq_start(struct seq_file *s, loff_t *pos) // called at the beginning of a sequence
{
	static unsigned long counter = 0;
	if (*pos == 0) return &counter; // if beginning a new sequence, then return actual value
	else // else then it's the end of the sequence, so stop reading
	{
		*pos = 0;
		return NULL;
	}
}

static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos) // called for next value
{
	unsigned long *tmp_v = (unsigned long *)v;
	(*tmp_v)++;
	(*pos)++;
	return NULL;
}

static void my_seq_stop(struct seq_file *s, void *v) // called at the end of a sequence
{
}

static int my_seq_show(struct seq_file *s, void *v) // called for each step of a sequence; actually shows everything in this function
{
	int total_task_num = 0;
	struct task_struct *task;
	print_bar(s);
	seq_printf(s, "CURRENT SYSTEM INFORMATION >\n");
	for_each_process (task) total_task_num++; // count tasks by iterating each process
	seq_printf(s, "Total %d task\n", total_task_num);
	seq_printf(s, "%dHz, %ld ms after system boot time\n", HZ, jiffies_to_msecs(get_jiffies_64() - INITIAL_JIFFIES)); // HZ is a defined constant, getting uptime by jiffies
	print_bar(s);
	seq_printf(s, "%19s%8s%13s%13s%13s%13s\n", "command", "pid", "start(s)", "total(s)", "user(s)", "kernel(s)");
	print_bar(s);
	for_each_process (task) // iterating each process for print information
	{
		long utime = jiffies_to_msecs(task->utime); // user mode time
		long stime = jiffies_to_msecs(task->stime); // system(kernel) mode time
		long ttime = utime + stime; // total time is always user mode + system mode
		seq_printf(s, "%19s%8d%9ld.%03ld%9ld.%03ld%9ld.%03ld%9ld.%03ld\n", task->comm, task->pid, task->start_time.tv_sec, task->start_time.tv_nsec / NSEC_PER_MSEC, ttime / MSEC_PER_SEC, ttime % MSEC_PER_SEC, utime / MSEC_PER_SEC, utime % MSEC_PER_SEC, stime / MSEC_PER_SEC, stime % MSEC_PER_SEC);
	}
	return 0;
}

static struct seq_operations my_seq_ops = {.start = my_seq_start, .next = my_seq_next, .stop = my_seq_stop, .show = my_seq_show}; // struct with sequence managing functions

static int my_open(struct inode *inode, struct file *file) // called when the /proc/hw1 file is in reading
{
	return seq_open(file, &my_seq_ops); // register sequence managing functions
}

static struct file_operations my_file_ops = {.owner = THIS_MODULE, .open = my_open, .read = seq_read, .llseek = seq_lseek, .release = seq_release}; // structure with file managing functions

int init_module() // called when the module is loaded
{
	struct proc_dir_entry *entry = create_proc_entry(procfs_name, 0, NULL); // create proc entry
	if (entry) entry->proc_fops = &my_file_ops; // register file managing functions
	return 0;
}

void cleanup_module() // called when the module is unloaded
{
	remove_proc_entry(procfs_name, NULL);
}
