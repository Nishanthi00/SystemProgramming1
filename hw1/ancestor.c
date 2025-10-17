// ancestor.c

#include <linux/module.h>       // Needed for all kernel modules
#include <linux/kernel.h>       // Needed for KERN_INFO
#include <linux/proc_fs.h>      // Needed for proc file system functions
#include <linux/seq_file.h>     // Needed for seq_file operations
#include <linux/sched/signal.h> // Needed for task_struct and process iteration
#include <linux/uaccess.h>      // Needed for copy_from_user()

#define PROC_NAME "ancestor"    // The name of the proc file to be created


#define STUDENT_ID  "2022148038"
#define STUDENT_NAME "Nishanthi Rengasamy"

// Variable to store the PID entered by the user
static pid_t stored_pid = -1;

//It prints the student's info and the list of ancestor processes
static int ancestor_show(struct seq_file *m, void *v)
{
    struct task_struct *task;

    // Print header
    seq_printf(m, "ID: %s\n", STUDENT_ID);
    seq_printf(m, "Name: %s\n", STUDENT_NAME);
    seq_printf(m, "----------------------------------------\n");

    // Check if a PID has been written before reading
    if (stored_pid <= 0) {
        seq_puts(m, "No PID stored. Use: echo <PID> > /proc/ancestor\n");
        return 0;
    }

    // Find the task_struct of the process using its PID
    task = pid_task(find_vpid(stored_pid, PIDTYPE_PID);
    if (!task) {
        seq_printf(m, "PID %d not found.\n", stored_pid);
        return 0;
    }

    // Traverse the ancestor chain
    // Each process has a pointer to its parent in task->real_parent
    while (task) {
        seq_printf(m, "[%d] %s\n", task->pid, task->comm);
        if (task->pid == 1) // Stop at init process
            break;
        task = task->real_parent; // Move to parent
    }

    return 0;
}


 //It links the 'show' function above with seq_file operations.
 
static int ancestor_open(struct inode *inode, struct file *file)
{
    return single_open(file, ancestor_show, NULL);
}


//Reads the PID value from user space and stores it.
static ssize_t ancestor_write(struct file *file, const char __user *buf,
                              size_t count, loff_t *pos)
{
    char kbuf[32];   // Buffer to temporarily store input from user
    long pid_input;  // Variable to hold the converted PID value

    // If the input is too long, return error
    if (count >= sizeof(kbuf))
        return -EINVAL;

    // Copy data from user space to kernel space
    if (copy_from_user(kbuf, buf, count))
        return -EFAULT;

    // Null-terminate the input string
    kbuf[count] = '\0';

    // Convert string input (e.g. "1234") to a number
    if (kstrtol(kbuf, 10, &pid_input) < 0)
        return -EINVAL;

    // Store the PID for later use in ancestor_show()
    stored_pid = (pid_t)pid_input;

    // Return the number of bytes written (standard for write handlers)
    return count;
}

//Define which functions handle each file operation.
static const struct proc_ops ancestor_ops = {
    .proc_open    = ancestor_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
    .proc_write   = ancestor_write,
  
};

//Creates the /proc/ancestor entry with read/write permissions (0666)
static int __init ancestor_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &ancestor_ops);
    pr_info("/proc/%s created\n", PROC_NAME);
    return 0;
}


//Remove the /proc/ancestor entry.
static void __exit ancestor_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("/proc/%s removed\n", PROC_NAME);
}

// Register the init and exit functions
module_init(ancestor_init);
module_exit(ancestor_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nishanthi");
MODULE_DESCRIPTION("Kernel module to print ancestor processes of a given PID");
