#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    VFS_TYPE_FILE,
    VFS_TYPE_DIR,
    VFS_TYPE_DEVICE
} vfs_node_type_t;

struct vfs_node;

typedef uint32_t (*vfs_read_func)(struct vfs_node* node, uint32_t offset, uint32_t size, uint8_t* buffer);
typedef uint32_t (*vfs_write_func)(struct vfs_node* node, uint32_t offset, uint32_t size, uint8_t* buffer);

typedef struct vfs_node {
    char name[128];
    vfs_node_type_t type;
    uint32_t length;
    void* device_ptr; // Points to ATA or other driver data
    vfs_read_func read;
    vfs_write_func write;
} vfs_node_t;

void vfs_init();
vfs_node_t* vfs_get_mount_point(const char* path);

#endif
