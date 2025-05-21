#ifndef USERPROG_MMAP_H
#define USERPROG_MMAP_H

#include <list.h>
#include "filesys/file.h"

struct mmap_file {
    int mapid;
    void *addr;                  // Địa chỉ ánh xạ vào không gian người dùng
    struct file *file;          // File được ánh xạ
    struct list_elem elem;      // Dùng để nối vào list mmap_list
    struct list pages;          // Danh sách các trang (nếu bạn cần quản lý trang riêng)
};

void *do_mmap(void *addr, size_t length, int writable, struct file *file, off_t offset);
void do_munmap(int mapid);

#endif /* USERPROG_MMAP_H */
