// proc_analyzer.c

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched/signal.h>  // for_each_process, task_struct
#include <linux/uaccess.h>       // copy_from_user
#include <linux/sort.h>          // for sort()
#include <linux/slab.h>          // kmalloc / kfree

#define PROC_NAME "proc_analyzer"
#define STUDENT_ID  "2022148038"
#define STUDENT_NAME "Nishanthi Rengasamy"

static pid_t stored_pid = -1;


//Each entry holds the process ID, name, CPU number, and vruntime value
struct proc_info {
    pid_t pid;
    char comm[TASK_COMM_LEN];
    int cpu;
    u64 vruntime;
};

/* Comparator for sorting by vruntime (ascending) */
static int cmp_vruntime(const void *a, const void *b)
{
    const struct proc_info *pa = a;
    const struct proc_info *pb = b;
    if (pa->vruntime < pb->vruntime)
        return -1;
    else if (pa->vruntime > pb->vruntime)
        return 1;
    else
        return 0;
}


// Recursively collect all descendants of a process and include the given process itself if it is on the CFS runqueue
static int collect_tasks(struct task_struct *task,
                         struct proc_info *arr, int *count, int max)
{
    struct task_struct *child;

    // Check if current task is valid and on the CFS runqueue
    if (task->se.on_rq && task->se.cfs_rq && *count < max) {
        struct proc_info *info = &arr[*count];
        info->pid = task->pid;
        get_task_comm(info->comm, task);
        info->cpu = task_cpu(task);
        info->vruntime = task->se.vruntime;
        (*count)++;
    }

    // Visit all child processes recursively
    list_for_each_entry(child, &task->children, sibling) {
        if (*count >= max)
            break;
        collect_tasks(child, arr, count, max);
    }

    return *count;
}


// It prints student info, PID, and scheduling information
static int analyzer_show(struct seq_file *m, void *v)
{
    struct task_struct *start;
    struct proc_info *proc_list;
    int total = 0;
    int i, cpu, cpu_count;
    int count_per_cpu[NR_CPUS] = {0};

    seq_printf(m, "ID: %s\n", STUDENT_ID);
    seq_printf(m, "Name: %s\n", STUDENT_NAME);
    seq_printf(m, "PID: %d\n", stored_pid);
    seq_printf(m, "----------------------------------------\n");

    if (stored_pid <= 0) {
        seq_puts(m, "No PID stored. Use: echo <PID> > /proc/proc_analyzer\n");
        return 0;
    }

    // Find the starting process
    start = pid_task(find_vpid(stored_pid, PIDTYPE_PID);
    if (!start) {
        seq_printf(m, "PID %d not found.\n", stored_pid);
        return 0;
    }

    // Allocate memory to store up to 1024 processes (enough for this assignment)
    proc_list = kmalloc_array(1024, sizeof(struct proc_info), GFP_KERNEL);
    if (!proc_list)
        return -ENOMEM;

    // Collect all descendant processes (and the process itself)
    total = collect_tasks(start, proc_list, &total, 1024);

    // If nothing was collected, print message and return
    if (total == 0) {
        seq_puts(m, "No processes found in the CFS runqueue.\n");
        kfree(proc_list);
        return 0;
    }

    // Sort the processes by vruntime (ascending order)
    sort(proc_list, total, sizeof(struct proc_info), cmp_vruntime, NULL);

    cpu_count = num_online_cpus();

    // Count processes per CPU for display
    for (i = 0; i < total; i++) {
        if (proc_list[i].cpu < NR_CPUS)
            count_per_cpu[proc_list[i].cpu]++;
    }

    // Print information grouped by CPU
    for (cpu = 0; cpu < cpu_count; cpu++) {
        int printed = 0;
        if (count_per_cpu[cpu] == 0)
            continue; // skip CPUs with no tasks

        seq_printf(m, "[CPU #%d] Running processes: %d\n",
                   cpu, count_per_cpu[cpu]);

        for (i = 0; i < total; i++) {
            if (proc_list[i].cpu == cpu) {
                seq_printf(m, "[%d] %s %llu\n",
                           proc_list[i].pid,
                           proc_list[i].comm,
                           (unsigned long long)proc_list[i].vruntime);
                printed++;
            }
        }

        // Separator line between CPU groups
        seq_puts(m, "----------------------------------------\n");
    }

    kfree(proc_list);
    return 0;
}


// Links show function with seq_file operations 
static int analyzer_open(struct inode *inode, struct file *file)
{
    return single_open(file, analyzer_show, NULL);
}


 // Stores the PID in the module
static ssize_t analyzer_write(struct file *file, const char __user *buf,
                              size_t count, loff_t *pos)
{
    char kbuf[32];
    long pid_input;

    if (count >= sizeof(kbuf))
        return -EINVAL;

    if (copy_from_user(kbuf, buf, count))
        return -EFAULT;

    kbuf[count] = '\0';

    if (kstrtol(kbuf, 10, &pid_input) < 0)
        return -EINVAL;

    stored_pid = (pid_t)pid_input;
    return count;
}


 // Defines what happens when /proc/proc_analyzer is used.
static const struct proc_ops analyzer_ops = {
    .proc_open    = analyzer_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
    .proc_write   = analyzer_write,
};

// Called when module is to be inserted or removed
static int __init analyzer_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &analyzer_ops);
    pr_info("/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit analyzer_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("/proc/%s removed\n", PROC_NAME);
}

module_init(analyzer_init);
module_exit(analyzer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nishanthi");
MODULE_DESCRIPTION("Kernel module to analyze CFS scheduling info by CPU");

