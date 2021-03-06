#ifndef _vfs_h
#define _vfs_h

#include <system.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <phapi.h>

#define PATH_SEPARATOR '/'
#define PATH_SEPARATOR_STRING "/"
#define PATH_UP ".."
#define PATH_CURRENT "."

#define FS_FILE       0x01
#define FS_DIRECTORY  0x02
#define FS_DEVICE     0x04
#define FS_PIPE       0x08
#define FS_SYMLINK    0x10
#define FS_MOUNTPOINT 0x20
#define FS_META       0x40

#define MAX_KERNEL_FD 256

struct super_block;
struct inode;
struct dirent;
struct stat;

typedef int (*inode_read_t)(struct inode *node, size_t sz, int n, char *buf);
typedef int (*inode_write_t)(struct inode *node, size_t sz, int n, char *buf);
typedef int (*inode_rewind_t)(struct inode *node);
typedef int (*inode_open_t)(struct inode *node, uint32_t flags);
typedef int (*inode_close_t)(struct inode *node);
typedef struct dirent *(*inode_readdir_t)(struct inode *node);
typedef struct inode *(*inode_finddir_t)(struct inode *node, char *name);
typedef int (*inode_create_t)(struct inode *parent, char *name, uint32_t flags);
typedef int (*inode_mkdir_t)(struct inode *parent, char *name, uint32_t flags);
typedef int (*inode_ioctl_t)(struct inode *node, int req, void *argp);
typedef size_t (*inode_getsize_t)(struct inode *node);
typedef int (*inode_chmod_t)(struct inode *node, uint32_t mode);
typedef int (*inode_unlink_t)(struct inode *node);
typedef int (*inode_symlink_t)(struct inode *node, struct inode *link);
typedef int (*inode_readlink_t)(struct inode *node);

typedef struct inode {
    void *device;         // Device where node is found
    uint32_t mask;        // Node mask
    uint32_t uid;         // User ID
    uint32_t gid;         // Group ID
    uint32_t flags;       // Node flags
    uint32_t inode;       // Node ID
    size_t length;        // Node content length
    uint32_t nblocks;     // Number of blocks
    uint32_t blocksz;     // Block size
    uint32_t block;       // Phys. block
    uint32_t open_flags;  // Flags for open
    uint32_t lock;        // Inode is locked or not
    size_t offset;        // Cursor position

    uint32_t atime;       // Accessed time
    uint32_t mtime;       // Modified time
    uint32_t ctime;       // Created time

    // Functions
    inode_read_t      read;
    inode_write_t     write;
    inode_rewind_t    rewind;
    inode_open_t      open;
    inode_close_t     close;
    inode_readdir_t   readdir;
    inode_finddir_t   finddir;
    inode_create_t    create;
    inode_mkdir_t     mkdir;
    inode_ioctl_t     ioctl;
    inode_getsize_t   getsize;
    inode_chmod_t     chmod;
    inode_unlink_t    unlink;
    inode_symlink_t   symlink;
    inode_readlink_t  readlink;

    // Link and reference
    struct inode *link;
    int32_t refcount;
    uint32_t nlink;
} inode_t;

typedef struct {
    void *to_be_implemented; // TODO: implement this
} file_system_type;

typedef struct super_block {
    void *device;
    inode_t *inode;
    uint32_t block;
    file_system_type *fs_type;
    void *fs_data;

    // TODO: add operations
} super_block_t;

struct dirent {
    uint32_t ino;
    char name[256];
};

struct stat {
    uint16_t st_dev;
    uint16_t st_ino;
    uint32_t st_mode;
    uint16_t st_nlink;
    uint16_t st_uid;
    uint16_t st_gid;
    uint16_t st_rdev;
    uint32_t st_size;
    uint32_t st_atime;
    uint32_t __unused1;
    uint32_t st_mtime;
    uint32_t __unused2;
    uint32_t st_ctime;
    uint32_t __unused3;
};

void init_vfs();
int inode_init(struct inode *node, uint32_t flags, uint32_t id, uint32_t length,
                uint32_t nblocks, uint32_t blocksz, uint32_t block);
int inode_read(struct inode *node, size_t sz, int n, char *buf);
int inode_write(struct inode *node, size_t sz, int n, char *buf);
int inode_rewind(struct inode *node);
int inode_open(struct inode *node, uint32_t flags);
int inode_close(struct inode *node);
struct dirent *inode_readdir(struct inode *node);
struct inode *inode_finddir(struct inode *node, char *name);
int inode_create(struct inode *node, char *name, uint32_t flags);
int inode_mkdir(struct inode *node, char *name, uint32_t flags);
int inode_ioctl(struct inode *node, int req, void *argp);
size_t inode_getsize(struct inode *node);
int inode_chmod(struct inode *node, uint32_t mode);
int inode_unlink(struct inode *node);
int inode_symlink(struct inode *node, struct inode *link);
int inode_readlink(struct inode *node);
#endif
