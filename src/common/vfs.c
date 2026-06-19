#include "vfs.h"
#include "string.h"
#include "kheap.h"
#include "initrd.h"
#include "stdio.h"

vfs_node_t* vfs_root = NULL;

void vfs_init() {
    vfs_root = (vfs_node_t*)kmalloc(sizeof(vfs_node_t));
    memset(vfs_root, 0, sizeof(vfs_node_t));
    strcpy(vfs_root->name, "/");
    vfs_root->type = VFS_TYPE_DIR;
    kprintf("[VFS] Initialized Virtual File System\n");
}

uint32_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    if (node && node->read) {
        return node->read(node, offset, size, buffer);
    }
    
    // Fallback: If it's a file mapped directly from initrd
    if (node && node->device_ptr) {
        uint8_t *file_data = (uint8_t*)node->device_ptr;
        if (offset >= node->length) return 0;
        uint32_t to_read = size;
        if (offset + size > node->length) {
            to_read = node->length - offset;
        }
        memcpy(buffer, file_data + offset, to_read);
        return to_read;
    }
    
    return 0;
}

uint32_t vfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    if (node && node->write) {
        return node->write(node, offset, size, buffer);
    }
    return 0; // Read-only by default
}

vfs_node_t* vfs_open(const char* filename, uint32_t flags) {
    (void)flags;
    
    // Check if path has /bin/ prefix and strip it for initrd search
    const char *search_name = filename;
    if (strncmp(filename, "/bin/", 5) == 0) {
        search_name = filename + 5;
    }
    
    size_t file_size = 0;
    const uint8_t *binary = initrd_get_file(search_name, &file_size);
    if (!binary) {
        binary = initrd_get_file(filename, &file_size);
    }
    
    if (binary) {
        vfs_node_t* node = (vfs_node_t*)kmalloc(sizeof(vfs_node_t));
        memset(node, 0, sizeof(vfs_node_t));
        strncpy(node->name, filename, 127);
        node->type = VFS_TYPE_FILE;
        node->length = file_size;
        node->device_ptr = (void*)binary; // Store the direct memory pointer
        return node;
    }
    
    return NULL;
}

void vfs_close(vfs_node_t* node) {
    // In a real VFS, we'd manage refcounts and free memory
    if (node) {
        // We can't safely free it if it's shared, but for this simple 
        // bridge, memory leaks are acceptable for now.
    }
}
