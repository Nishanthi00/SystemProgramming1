#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x14\x00\x00\x00\x4b\x77\x54\x38"
	"kstrtoll\0\0\0\0"
	"\x28\x00\x00\x00\xb3\x1c\xa2\x87"
	"__ubsan_handle_out_of_bounds\0\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x14\x00\x00\x00\x0c\xc0\xe6\x58"
	"single_open\0"
	"\x1c\x00\x00\x00\x81\xde\x9c\x46"
	"remove_proc_entry\0\0\0"
	"\x18\x00\x00\x00\x61\x44\x34\x46"
	"__get_task_comm\0"
	"\x10\x00\x00\x00\xc5\x8f\x57\xfb"
	"memset\0\0"
	"\x14\x00\x00\x00\x6d\x65\x85\x05"
	"seq_printf\0\0"
	"\x14\x00\x00\x00\x3e\x54\x0c\x4e"
	"find_vpid\0\0\0"
	"\x14\x00\x00\x00\x2c\x80\xd1\x81"
	"pid_task\0\0\0\0"
	"\x18\x00\x00\x00\x20\x2e\xd1\x9e"
	"kmalloc_large\0\0\0"
	"\x10\x00\x00\x00\x43\xf4\xc0\xb7"
	"sort\0\0\0\0"
	"\x1c\x00\x00\x00\x20\x06\x0d\xc6"
	"__num_online_cpus\0\0\0"
	"\x14\x00\x00\x00\xb4\x15\x25\xb1"
	"seq_puts\0\0\0\0"
	"\x10\x00\x00\x00\xba\x0c\x7a\x03"
	"kfree\0\0\0"
	"\x14\x00\x00\x00\x18\xe4\xc9\xb4"
	"seq_read\0\0\0\0"
	"\x14\x00\x00\x00\xdb\x38\x4a\xd7"
	"seq_lseek\0\0\0"
	"\x18\x00\x00\x00\x19\xc1\xe2\xfa"
	"single_release\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x14\x00\x00\x00\x20\x81\x79\xcd"
	"proc_create\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\x48\x9f\xdb\x88"
	"__check_object_size\0"
	"\x18\x00\x00\x00\xc2\x9c\xc4\x13"
	"_copy_from_user\0"
	"\x18\x00\x00\x00\x14\x1c\xe7\x99"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "59D977A40BE4778AD50AB4D");
