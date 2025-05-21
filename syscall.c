#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/fd.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "vm/mmap.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *f);
struct file *process_get_file (int fd);
void shutdown_power_off(void);

void
syscall_init (void) {
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) {
  int syscall_number = *(int *)(f->esp);

  switch (syscall_number) {
    case SYS_HALT:
      shutdown_power_off();
      break;

    case SYS_MMAP: {
      int fd = *(int *)(f->esp + 4);
      void *addr = (void *)(*(int *)(f->esp + 8));
      struct file *file = process_get_file(fd);
      void *mapped = do_mmap(addr, file_length(file), true, file, 0);
      f->eax = (uint32_t)mapped;
      break;
    }

    case SYS_MUNMAP: {
      int mapid = *(int *)(f->esp + 4);
      do_munmap(mapid);
      break;
    }

    default:
      printf ("Unknown syscall: %d\n", syscall_number);
      thread_exit();
  }
}
