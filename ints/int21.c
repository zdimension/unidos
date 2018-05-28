/* UniDOS emulator */
/* By Nguyen Anh Quynh, 2015 */

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unicorn/unicorn.h>

#include "../util/uc.h"
#include "int21.h"
#include "../global.h"
#include "../util/dospath.h"

#define FD_TABLE_SIZE 256

enum IOCTL_FUNC
{
    IOCTL_GET_DEVICE_INFORMATION = 0x0,
    IOCTL_SET_DEVICE_INFORMATION = 0x1,
    IOCTL_READ_FROM_CHARACTER_DEVICE = 0x2,
    IOCTL_WRITE_TO_CHARACTER_DEVICE = 0x3,
    IOCTL_READ_FROM_BLOCK_DEVICE = 0x4,
    IOCTL_WRITE_TO_BLOCK_DEVICE = 0x5,
    IOCTL_GET_INPUT_STATUS = 0x6,
    IOCTL_GET_OUTPUT_STATUS = 0x7,
    IOCTL_DEVICE_REMOVABLE_QUERY = 0x8,
    IOCTL_DEVICE_LOCAL_OR_REMOTE_QUERY = 0x9,
    IOCTL_HANDLE_LOCAL_OR_REMOTE_QUERY = 0xA,
    IOCTL_SET_SHARING_RETRY_COUNT = 0xB,
    IOCTL_GENERIC_IO_FOR_HANDLES = 0xC,
    IOCTL_GENERIC_IO_FOR_BLOCK_DEVICES = 0xD,
    IOCTL_GET_LOGICAL_DRIVE = 0xE,
    IOCTL_SET_LOGICAL_DRIVE = 0xF
};

enum IOCTL_INFO
{
    IOCTL_DRIVE_LETTER = 0x3F,
    IOCTL_NOT_WRITTEN = 1 << 6,

    IOCTL_IS_STDIN = 1 << 0,
    IOCTL_IS_STDOUT = 1 << 1,
    IOCTL_IS_NUL = 1 << 2,
    IOCTL_IS_CLOCK = 1 << 3,
    IOCTL_USES_INT_29 = 1 << 4,
    IOCTL_BINARY_MODE = 1 << 5,
    IOCTL_NON_EOF = 1 << 6,
    IOCTL_IS_CHAR = 1 << 7,
    IOCTL_RESERVED_1 = 0x700, // 8 to 10
    IOCTL_NOT_REMOVABLE = 1 << 11,
    IOCTL_NETWORK_DEVICE = 1 << 12,
    IOCTL_RESERVED_2 = 1 << 13,
    IOCTL_SUPPORTS_IOCTL = 1 << 14,
    IOCTL_RESERVED_3 = 1 << 15,

    IOCTL_NON_UPDATE_DATE = 1 << 14,
    IOCTL_REMOTE = 1 << 5
};

static uint8_t str_buf[1024];   // buffer for reading string from memory
static uint16_t dta;    // disk transfer area address
static int fdtable[FD_TABLE_SIZE];
static char buf[64 * 1024];


// read a string of @size bytes
static char *read_str(uc_engine *uc, uint64_t addr, int size)
{
    // do not read exceed the size of str_buf[]
    size = MIN(size, sizeof(str_buf) - 1);
    uc_mem_read(uc, addr, str_buf, size);
    str_buf[size] = '\0';

    return (char *)str_buf;
}


// read a string of @size bytes, or until '$' letter.
static char *read_str_till_char(uc_engine *uc, uint64_t addr, char terminator)
{
    size_t i = 0;

    // read a string from memory until '@terminator
    while(true) {
        uc_mem_read(uc, addr + i, str_buf + i, 1);
        if (str_buf[i] == terminator) {
            str_buf[i] = '\0';
            break;
        }
        i++;
    }

    return (char *)str_buf;
}





// initialize FD table
static void fdtable_init(void)
{
    int i;

    // enable special fd
    fdtable[0] = 0;
    fdtable[1] = 1;
    fdtable[2] = 2;

    // disable other fd
    for(i = 3; i < FD_TABLE_SIZE; i++)
        fdtable[i] = -1;
}


// clear FD
static void fdtable_clear(int fd)
{
    if (fd < 3 || fd >= FD_TABLE_SIZE)
        return;

    fdtable[fd] = -1;
}


// map host FD to dos FD
static int fdtable_set(int hostfd)
{
    int i;

    for(i = 0; i < FD_TABLE_SIZE; i++) {
        if (fdtable[i] == -1) {
            // found a free slot, which is also a dos FD
            fdtable[i] = hostfd;
            return i;
        }
    }

    // not found
    return -1;
}


// map DOS FD to host FD
static int fdtable_get(int dosfd)
{
    if (dosfd > 0 && dosfd < FD_TABLE_SIZE)
        return fdtable[dosfd];

    return -1;
}


void int21_init(void)
{
    fdtable_init();
    memset(cur_path, 0, 26 * sizeof(struct dospath));
    for (int i = 0; i < 26; i++)
    {
        cur_path[i].drive = i;
    }
}


// callback for handling interrupt
void int21(uc_engine *uc)
{
    uint16_t r_ip;
    uint8_t r_ah;

    uc_reg_read(uc, UC_X86_REG_AH, &r_ah);
    uc_reg_read(uc, UC_X86_REG_IP, &r_ip);

    printf(">>> 0x%x: interrupt: %x, AH = %02x\n", r_ip, 0x21, r_ah);

    switch(r_ah) {
        default:
            printf("\n>>> 0x%x: interrupt: %x, AH = %02x\n", r_ip, 0x21, r_ah);
            break;

        case 0x00: //terminate process
            {
                uc_emu_stop(uc);
            }

        case 0x01: // keyboard input without echo
        {
            uint8_t r_al = getchar();

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x02: // character output
            {
                uint8_t r_dl;

                uc_reg_read(uc, UC_X86_REG_DL, &r_dl);

                printf("%c", r_dl);

                break;
            }

        case 0x08: // character input
        {
            uint8_t r_al = getchar();

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x09: // write to screen
            {
                uint16_t r_dx, r_ds;
                char *buf;

                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
                uc_reg_read(uc, UC_X86_REG_DS, &r_ds);
                //printf(">>> 0x%x: interrupt: %x, AH: %02x, DX = %02x, DS = %02x, addr = %x\n\n",
                //        r_ip, 0x21, r_ah, r_dx, r_ds, MK_FP(r_ds, r_dx));

                // read until '$'
                buf = read_str_till_char(uc, MK_FP(r_ds, r_dx), '$');
                printf("%s", buf);

                break;
            }

        case 0x0a: // buffered keyboard input
            {
                uint16_t r_dx, r_ds;
                uint8_t max_buf;
                char *buf, *str;   

                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
                uc_reg_read(uc, UC_X86_REG_DS, &r_ds);
                uc_mem_read(uc, MK_FP(r_ds, r_dx), &max_buf, 1);

                fscanf(stdin, "%ms", &buf);
                str = strndup(buf, max_buf -1);
                str[max_buf] = '$';

                uc_mem_write(uc, MK_FP(r_ds, r_dx) + 2, str, strlen(str));

                free(buf);
                free(str);

                break;
            }

        case 0x0b: // check standard input status
        {
            uint8_t r_al = feof(stdin) ? 0x00 : 0xff;

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x0c: // clear keyboard buffer and invoke keyboard function
        {
            uint8_t r_al;

            uc_reg_read(uc, UC_X86_REG_AL, &r_al);

            r_ah = r_al;
            goto rerun;
        }

        case 0x0e: // select disk
        {
            uint8_t r_dl;

            uc_reg_read(uc, UC_X86_REG_DL, &r_dl);

            cur_drive = r_dl;

            uint8_t r_al = 1;

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x19: // get current default drive
        {
            uint8_t r_al = cur_drive;

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x1a: // set disk transfer area address
            {
                uint16_t r_dx;

                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);

                dta = r_dx;

                break;
            }

        case 0x30: // return DOS version 7.0
            {
                uint8_t r_al = 6;   // major
                uint8_t r_ah = 22;  // minor

                uc_reg_write(uc, UC_X86_REG_AL, &r_al);
                uc_reg_write(uc, UC_X86_REG_AH, &r_ah);

                break;
            }

        case 0x3b: // change current directory
        {
            uint16_t r_dx, r_ds;
            char *chdir;

            uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
            uc_reg_read(uc, UC_X86_REG_DS, &r_ds);

            // read until '$'
            chdir = read_str_till_char(uc, MK_FP(r_ds, r_dx), '$');

            struct dospath res;
            string_to_path(chdir, &res);

            if (res.drive != -1)
            {
                cur_drive = res.drive;
            }
            else
            {
                res.drive = cur_drive;
            }

            cur_path[cur_drive] = res;

            set_flag_C(uc, 0);
        }

        case 0x3c: // create a new file (or truncate existing file)
            {
                uint16_t tmp, r_dx, r_ds;
                uint8_t r_al;
                char *fname;
                int hostfd;

                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
                uc_reg_read(uc, UC_X86_REG_DS, &r_ds);
                uc_reg_read(uc, UC_X86_REG_AL, &r_al);

                // read until '$'
                fname = read_str_till_char(uc, MK_FP(r_ds, r_dx), '$');
                //printf(">>> Trying to create a new file '%s'\n", fname);

                // TODO ignore attributes
                hostfd = open(fname, O_CREAT | O_TRUNC | O_RDWR);
                if (hostfd < 0) {   // failed to open
                    set_flag_C(uc, 1);
                    tmp = errno;    // FIXME
                } else {
                    int dosfd = fdtable_set(hostfd);
                    if (dosfd < 0) {
                        // system table is full
                        close(hostfd);
                        set_flag_C(uc, 1);
                        tmp = ENFILE;
                    } else {
                        set_flag_C(uc, 0);
                        tmp = dosfd;
                        //printf(">>> OK, dosfd = %u\n", dosfd);
                    }
                }
                uc_reg_write(uc, UC_X86_REG_AX, &tmp);

                break;
            }

        case 0x3d: // open existing file
            {
                uint16_t tmp, r_dx, r_ds;
                uint8_t r_al;
                char *fname;
                int hostfd;

                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
                uc_reg_read(uc, UC_X86_REG_DS, &r_ds);
                uc_reg_read(uc, UC_X86_REG_AL, &r_al);

                // read until '$'
                fname = read_str_till_char(uc, MK_FP(r_ds, r_dx), '$');
                //printf(">>> Trying to open file '%s'\n", fname);

                hostfd = open(fname, r_al & 3);
                if (hostfd < 0) {   // failed to open
                    set_flag_C(uc, 1);
                    tmp = errno;    // FIXME
                } else {
                    int dosfd = fdtable_set(hostfd);
                    if (dosfd < 0) {
                        // system table is full
                        close(hostfd);
                        set_flag_C(uc, 1);
                        tmp = ENFILE;
                    } else {
                        set_flag_C(uc, 0);
                        tmp = dosfd;
                        //printf(">>> OK, dosfd = %u\n", dosfd);
                    }
                }
                uc_reg_write(uc, UC_X86_REG_AX, &tmp);

                break;
            }

        case 0x3e: // close file
            {
                uint16_t r_bx, r_ax;
                int fd;

                // read from FD in BX register
                uc_reg_read(uc, UC_X86_REG_BX, &r_bx);
                fd = fdtable_get(r_bx);
                if (fd < 0) {
                    set_flag_C(uc, 1);
                    r_ax = EBADF;
                    uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
                    break;
                }

                if (close(fd)) {
                    set_flag_C(uc, 1);
                    r_ax = errno;   // FIXME
                } else {
                    set_flag_C(uc, 0);
                    fdtable_clear(r_bx);
                }

                //printf(">>> closed file = %u\n", r_bx);

                break;
            }

        case 0x3f: // read from device
            {
                uint16_t r_bx, r_ax, r_cx, r_dx, r_ds;
                int fd;
                ssize_t count;

                // read from FD in BX register
                uc_reg_read(uc, UC_X86_REG_BX, &r_bx);
                fd = fdtable_get(r_bx);
                if (fd < 0) {
                    set_flag_C(uc, 1);
                    r_ax = EBADF;
                    uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
                    break;
                }

                uc_reg_read(uc, UC_X86_REG_CX, &r_cx);
                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
                uc_reg_read(uc, UC_X86_REG_DS, &r_ds);

                //printf(">>> trying to read from devide = %u, len = %u\n", r_bx, r_cx);
                count = read(fd, buf, r_cx);
                if (count < 0) {
                    // failed to read
                    set_flag_C(uc, 1);
                    r_ax = errno;   // FIXME
                } else {
                    // copy read memory to emulator memory
                    uc_mem_write(uc, MK_FP(r_ds, r_dx), buf, count);
                    set_flag_C(uc, 0);
                    r_ax = count;
                }

                uc_reg_write(uc, UC_X86_REG_AX, &r_ax);

                break;
            }

        case 0x40: // Write to device
            {
                uint16_t r_ax, r_bx, r_cx, r_dx, r_ds;
                char *buf;
                int fd;
                ssize_t count;

                // write to FD in BX register
                uc_reg_read(uc, UC_X86_REG_BX, &r_bx);
                //printf(">>> trying to write to devide = %u\n", r_bx);
                fd = fdtable_get(r_bx);
                if (fd < 0) {
                    set_flag_C(uc, 1);
                    r_ax = EBADF;
                    uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
                    break;
                }

                uc_reg_read(uc, UC_X86_REG_CX, &r_cx);
                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
                uc_reg_read(uc, UC_X86_REG_DS, &r_ds);

                buf = read_str(uc, MK_FP(r_ds, r_dx), r_cx);
                //printf("%s", buf);

                count = write(fd, buf, r_cx);
                if (count < 0) {
                    // failed to write
                    set_flag_C(uc, 1);
                    r_ax = errno;   // FIXME
                } else {
                    set_flag_C(uc, 0);
                    r_ax = count;
                }

                uc_reg_write(uc, UC_X86_REG_AX, &r_ax);

                break;
            }

        case 0x41: // delete a file
            {
                uint16_t r_dx, r_ds, r_ax;
                char *fname;

                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
                uc_reg_read(uc, UC_X86_REG_DS, &r_ds);

                // read until '$'
                fname = read_str_till_char(uc, MK_FP(r_ds, r_dx), '$');
                //printf(">>> Trying to delete file '%s'\n", fname);

                if (unlink(fname)) {
                    set_flag_C(uc, 1);
                    r_ax = errno;
                    uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
                } else {
                    set_flag_C(uc, 0);
                }

                break;
            }
        case 0x42: // lseek to set file position
            {
                uint16_t r_bx, r_cx, r_dx, r_ax;
                uint8_t r_al;
                int fd;
                off_t offset;

                // FD in BX register
                uc_reg_read(uc, UC_X86_REG_BX, &r_bx);
                fd = fdtable_get(r_bx);
                if (fd < 0) {
                    set_flag_C(uc, 1);
                    r_ax = EBADF;
                    uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
                    break;
                }

                uc_reg_read(uc, UC_X86_REG_CX, &r_cx);
                uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
                uc_reg_read(uc, UC_X86_REG_AL, &r_al);

                offset = lseek(fd, (r_cx << 16) | r_dx, r_al);
                //printf(">>> trying to seek in devide = %u, offset = %u, where = %u\n", r_bx, offset, r_al);
                if (offset < 0) {
                    // fail to seek
                    set_flag_C(uc, 1);
                    r_ax = errno;   // FIXME
                    uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
                    //printf(">>> FAILED\n");
                } else {
                    set_flag_C(uc, 0);
                    r_ax = offset & 0xffff;
                    r_dx = offset >> 16;
                    uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
                    uc_reg_write(uc, UC_X86_REG_DX, &r_dx);
                    //printf(">>> OK\n");
                }

                break;
            }

        case 0x44: // I/O control for devices
        {
            uint8_t r_al, r_bl;
            uint16_t r_ax, r_bx, r_cx;

            uc_reg_read(uc, UC_X86_REG_AL, &r_al);
            uc_reg_read(uc, UC_X86_REG_BL, &r_bl);
            uc_reg_read(uc, UC_X86_REG_BX, &r_bx);
            uc_reg_read(uc, UC_X86_REG_CX, &r_cx);

            int fd = fdtable_get(r_bx);
            if (fd < 0) {
                set_flag_C(uc, 1);
                r_ax = EBADF;
                uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
                break;
            }

            FILE* fp = fdopen(fd, "r");

            printf("\n>>> 0x%x: interrupt: %x, IOCTL (44), func = %02x, fp = %02x, dev = %02d, bs = %02x\n", r_ip, 0x21, r_al, r_bx, r_bl, r_cx);

            switch (r_al)
            {
                case IOCTL_GET_DEVICE_INFORMATION:
                {
                    uint16_t r_dx = 0;
                    struct stat stat;
                    fstat(fd, &stat);

                    if (S_ISCHR(stat.st_mode))
                    {
                        if (fd == 0)
                            r_dx |= IOCTL_IS_STDIN;
                        else if (fd == 1)
                            r_dx |= IOCTL_IS_STDOUT;

                        if (!feof(fp))
                            r_dx |= IOCTL_NON_EOF;

                        r_dx |= IOCTL_IS_CHAR;
                        r_dx |= IOCTL_SUPPORTS_IOCTL;
                    }
                    else if (S_ISBLK(stat.st_mode))
                    {

                    }

                    r_dx |= IOCTL_NOT_REMOVABLE;

                    break;
                }
            }

            break;
        }

        case 0x4c: // exit
            {
                uint8_t r_al;

                uc_reg_read(uc, UC_X86_REG_AL, &r_al);
                //printf(">>> 0x%x: interrupt: %x, AH: %02x = EXIT. quit with code = %02x!\n\n",
                //        r_ip, 0x21, r_ah, r_al);

                uc_emu_stop(uc);
                break;
            }

        case 0x57: // get data & time of last write to file
            {
                uint32_t r_eflags;

                //printf(">>> UNIMPLEMENTED datetime\n");
                uc_reg_read(uc, UC_X86_REG_EFLAGS, &r_eflags);
                r_eflags &= 0xfffffffe; // eflags_C = 0
                uc_reg_write(uc, UC_X86_REG_EFLAGS, &r_eflags);

                break;
            }

        case 0x2a: // get system date
        {
            time_t t = time_fix();
            struct tm tm = *localtime(&t);

            uint16_t r_cx = tm.tm_year - 80;
            uint8_t r_dh = tm.tm_mon + 1;
            uint8_t r_dl = tm.tm_mday;
            uint8_t r_al = tm.tm_wday;

            uc_reg_write(uc, UC_X86_REG_CX, &r_cx);
            uc_reg_write(uc, UC_X86_REG_DH, &r_dh);
            uc_reg_write(uc, UC_X86_REG_DL, &r_dl);
            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x2b: // set system date
        {
            time_t t = time_fix();
            struct tm tm = *localtime(&t);

            uint16_t r_cx;
            uint8_t r_dh, r_dl;

            uc_reg_read(uc, UC_X86_REG_CX, &r_cx);
            uc_reg_read(uc, UC_X86_REG_DH, &r_dh);
            uc_reg_read(uc, UC_X86_REG_DL, &r_dl);

            tm.tm_year = r_cx;
            tm.tm_mon = r_dh;
            tm.tm_mday = r_dl;

            time_t n = mktime(&tm);

            uint8_t r_al = 0x00;

            if (n == -1) // invalid date
            {
                r_al = 0xFF;
            }
            else
            {
                time_offset = n - t;
            }

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x2c: // get system time
        {
            struct timeval tval;
            ptime_fix(&tval);
            struct tm tm = *localtime(&tval.tv_sec);

            uint8_t r_ch = tm.tm_hour;
            uint8_t r_cl = tm.tm_min;
            uint8_t r_dh = tm.tm_sec;
            uint8_t r_dl = tval.tv_usec / 10000;

            uc_reg_write(uc, UC_X86_REG_CH, &r_ch);
            uc_reg_write(uc, UC_X86_REG_CL, &r_cl);
            uc_reg_write(uc, UC_X86_REG_DH, &r_dh);
            uc_reg_write(uc, UC_X86_REG_DL, &r_dl);

            break;
        }

        case 0x2d: // set system time
        {
            struct timeval tval;
            ptime_fix(&tval);
            struct tm tm = *localtime(&tval.tv_sec);
            time_t t = mktime(&tm);

            uint8_t r_ch, r_cl, r_dh, r_dl;

            uc_reg_read(uc, UC_X86_REG_CH, &r_ch);
            uc_reg_read(uc, UC_X86_REG_CL, &r_cl);
            uc_reg_read(uc, UC_X86_REG_DH, &r_dh);
            uc_reg_read(uc, UC_X86_REG_DL, &r_dl);

            tm.tm_hour = r_ch;
            tm.tm_min = r_cl;
            tm.tm_sec = r_dh;

            time_t n = mktime(&tm);

            uint8_t r_al = 0x00;

            if (n == -1) // invalid date
            {
                r_al = 0xFF;
            }
            else
            {
                time_offset = n - t;
            }

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x2e: // set/reset verify switch
        {
            uint8_t r_al;

            uc_reg_read(uc, UC_X86_REG_AL, &r_al);

            verify_switch = (bool)r_al;
        }

        case 0x33: // get/set system values (Ctrl+Break, boot drive)
        {
            uint8_t r_al, r_dl;

            uc_reg_read(uc, UC_X86_REG_AL, &r_al);
            uc_reg_read(uc, UC_X86_REG_DL, &r_dl);

            switch(r_al)
            {
                case 0x02:
                    ext_ctrl_break_check = (bool)r_dl;
                case 0x00:
                    r_dl = (ctrl_break_check || ext_ctrl_break_check) ? 1 : 0;
                    break;

                case 0x01:
                    ctrl_break_check = (bool)r_dl;
                    break;

                case 0x05:
                    r_dl = 3; // C: always
                    break;
            }

            uc_reg_write(uc, UC_X86_REG_DL, &r_dl);
        }

        case 0x47: // get current directory
        {
            uint8_t r_dl;
            uc_reg_read(uc, UC_X86_REG_DL, &r_dl);

            if (r_dl == 0)
                r_dl = cur_drive;
            else
                r_dl--;

            char* buf = malloc(66);
            path_to_string(cur_path[r_dl], buf);

            uint16_t r_ds, r_si;

            uc_reg_read(uc, UC_X86_REG_DS, &r_ds);
            uc_reg_read(uc, UC_X86_REG_SI, &r_si);

            uc_mem_write(uc, MK_FP(r_ds, r_si), buf + 2, 64);

            break;
        }

        case 0x48: // allocate memory
        {
            uint16_t r_ax = 0, r_bx = 0;

            uc_reg_read(uc, UC_X86_REG_BX, &r_bx);

            uint8_t err = mem_alloc(r_bx * 16, &r_ax);

            if (err)
            {
                set_flag_C(1);

                r_ax = err;
            }
            else
            {
                set_flag_C(0);
            }

            uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
            uc_reg_write(uc, UC_X86_REG_BX, &r_bx);
        }

        case 0x49: // free allocated memory
        {
            uint16_t r_ax = 0, r_es;

            uc_reg_read(uc, UC_X86_REG_ES, &r_es);

            uint8_t err = mem_free(r_es);

            if (err)
            {
                set_flag_C(1);

                r_ax = err;
            }
            else
            {
                set_flag_C(0);
            }

            uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
        }

        case 0x4a: // modify allocated memory
        {
            uint16_t r_ax = 0, r_bx, r_es;

            uc_reg_read(uc, UC_X86_REG_BX, &r_bx);
            uc_reg_read(uc, UC_X86_REG_ES, &r_es);

            uint8_t err = mem_realloc(r_bx * 16, r_es);

            if (err)
            {
                set_flag_C(1);

                r_ax = err;
            }
            else
            {
                set_flag_C(0);
            }

            r_bx = 0;

            uc_reg_write(uc, UC_X86_REG_AX, &r_ax);
            uc_reg_write(uc, UC_X86_REG_BX, &r_bx);
        }

        case 0x50: // set current process ID
        {
            uint16_t r_bx;

            uc_reg_read(uc, UC_X86_REG_BX, &r_bx);

            printf("trying to set PSP to %04x\n", r_bx);

            break;
        }
    }
}
