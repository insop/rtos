#ifndef __BIND486_H__
#define __BIND486_H__

/* Format of the bindfile:

Offset  Size
~~~~~~  ~~~~
0       (4 bytes)   offset of bindtable, relative to first file start
4       (4 bytes)   number of entries in table.
4       (4 bytes)   stack size of first file in bindfile (the kernel, ususally)
4       (4 bytes)   magic value (and paragraph alignment)

16      {first file} (offset measured from here)
	(0-15 bytes) paragraph alignment padding

x1      {second file}
	(0-15 bytes) paragraph alignment padding

	...
xn      {nth file}
	(0-15 bytes) paragraph alignment padding

	(sizeof(bindEntry) * n)

Note:
- All offset are relative to start of the first file in the
  bindfile. That is, the offset of the first file is 
  always zero. To convert offsets into offsets into the
  the on-disk version, you must add sizeof(hdr) to them.
- All values are uint == 32 bit integers, 4 bytes.
- Each element of the bindfile is paragraph aligned.
- Aside from the first file, each file need not be executable
  data. The "module" value in the bindtable may be used to 
  communicate datatype information to the kernel.
- The bindfile header is defined as "bindHeader" and each
  element of the bindtable is a "bindEntry". See below.
- The header is consumed by the loader during download so
  it is not available during run-time.
*/
#include <sys/types.h>          /* for uint */

#define BIND_MAGIC_VAL      0x06c6c6957

#define BIND_FNAME_LEN      16
#define BIND_MODNAME_LEN    32


/* This structure is found at the beginning of the file. */
typedef struct {
    uint        offset;             /* offset into the file of the bindtable */
    uint        numFiles;           /* number of files, # entries in table   */
    uint        firstStackSize;     /* size of stack for first file          */
    uint        magic;              /* padding: 4 = 16 - (3 * sizeof(uint))  */
} bindHeader;

/* Each entry in the bindtable is one instance of this structure. */
typedef struct {
    int             offset;         /* relative end of header of first file */
    int             stackSize;
    int             size;           /* size of original file before alignment */
    char            filename[BIND_FNAME_LEN];
    char            module[BIND_MODNAME_LEN];
} bindEntry;

#endif
