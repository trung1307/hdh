#include "vm/mmap.h"
#include "threads/vaddr.h"
#include "threads/thread.h"
#include "userprog/pagedir.h"
#include "filesys/file.h"
#include "threads/malloc.h"

static int next_mapid = 0;

void *do_mmap(void *addr, size_t length, int writable, struct file *file, off_t offset) {
    if (file == NULL || addr == NULL || pg_ofs(addr) != 0 || length == 0) return NULL;

    struct mmap_file *mmap = malloc(sizeof *mmap);
    mmap->mapid = next_mapid++;
    mmap->addr = addr;
    mmap->file = file_reopen(file);
    list_init(&mmap->pages);
    list_push_back(&thread_current()->mmap_list, &mmap->elem);

    // Không cài đặt chi tiết phân trang ở đây – bạn cần thêm code quản lý SPT nếu có
    return addr;
}

void do_munmap(int mapid) {
    struct thread *cur = thread_current();
    struct list_elem *e;
    for (e = list_begin(&cur->mmap_list); e != list_end(&cur->mmap_list); e = list_next(e)) {
        struct mmap_file *m = list_entry(e, struct mmap_file, elem);
        if (m->mapid == mapid) {
            file_close(m->file);
            list_remove(&m->elem);
            free(m);
            return;
        }
    }
}
