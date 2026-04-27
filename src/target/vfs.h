#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>

#define VFS_FILE      0x01
#define VFS_DIRECTORY 0x02

struct vfs_node;

typedef uint32_t (*read_type_t)(struct vfs_node*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(struct vfs_node*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(struct vfs_node*);
typedef void (*close_type_t)(struct vfs_node*);

typedef struct vfs_node {
    char name[128];
    uint32_t type;
    uint32_t inode;
    uint32_t size;
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    struct vfs_node* ptr; // FS specific pointer
} vfs_node_t;

void vfs_init();
uint32_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
uint32_t vfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);

#endif
