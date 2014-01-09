#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int set_f(int fd, int flags);

int main() {

    int fd1, fd2, fd3;      // file descriptors
    int flag1, flag2;       // file status flag
    char buf1[] = "foo foo";
    char buf2[] = "bar bar";

    // open new file or create (O_CREAT) it
    // if it doesn't exist
    // file is open for read and write (O_RDWR)
    // O_APPEND flag is sets (before each write()
    // the offset in the file table entry is set to
    // current file size)
    // owner can read & write to file (S_IRUSR | S_IWUSR)
    flag1 = O_RDWR | O_CREAT | O_APPEND;
    if ((fd1 = open("testfile", flag1, S_IRUSR | S_IWUSR)) == -1) {
        printf("fd1 open error\n");
        return -1;
    }

    // start writing at the end of the file
    // if file doesn't exist, it's content
    // after write is foo foo
    if (write(fd1, buf1, sizeof(buf1)) != sizeof(buf1)) {
        printf("buf1 write error\n");
        return -1;
    }

    // open same file at new descriptor
    flag2 = O_RDWR;
    if ((fd2 = open("testfile", flag2)) == -1) {
        printf("fd2 open error\n");
        return -1;
    }

    // start writing at the begining of the file
    // content of file after write is bar bar
    if (write(fd2, buf2, sizeof(buf2)) != sizeof(buf2))  {
        printf("buf2 write error\n");
        return -1;
    }

    // duplicate file descriptor
    // new descriptor share the 
    // same file table as fd1
    if ((fd3 = dup(fd1)) == -1) {
        printf("fd3 duplicate error\n");
        return -1;
    }

    // file content after write is 
    // bar barfoo foo
    if (write(fd3, buf1, sizeof(buf1)) != sizeof(buf1)) {
        printf("buf1fd3 write error\n");
        return -1;
    }

    // set file offset  at the end of file
    if (lseek(fd2, 0, SEEK_END) == -1) {
        printf("can't seek\n");
        return -1;
    }

    // file content after write is 
    // bar barfoo foobar bar
    if (write(fd2, buf2, sizeof(buf2)) != sizeof(buf2)) {
        printf("buf2end write error\n");
        return -1;
    }

    // turn off O_APPEND flag
    if (set_f(fd1, O_APPEND)) {
        printf("can't turn off flag\n");
    }

    // file content after write is 
    // bar barfoo foofoo foo
    if (write(fd3, buf1, sizeof(buf1)) != sizeof(buf1)) { 
        printf("buf1off write error\n");
        return -1;
    }

    return 0;

}

// turn off one or more file status flag
int set_f(int fd, int flags) {

    int val;

    // with fcntl function can change 
    // the properties of a file
    // before changing flags, we must
    // fetch the existing flag (F_GETFL)
    if ((val = fcntl(fd, F_GETFL, 0)) == -1) {
        return -1;
    }

    val &= ~flags;   // turn off flags

    if ((fcntl(fd, F_SETFL, val)) == -1) {
        return -1;
    }

    return 0;
}
