#include <unistd.h>
#include <fcntl.h>
char* getLinePrefixStr(char* dest, const int num, int* strSize){
    int digitsSize = 0;
    int digits[10];
    for(int i = num; i > 0; i /= 10){
        digits[digitsSize++] = i % 10;
    }
    for(int i = digitsSize - 1; i >= 0; i--){
        dest[digitsSize - 1 - i] = digits[i] + '0';
    }
    dest[digitsSize] = dest[digitsSize + 2] = ' ';
    dest[digitsSize + 1] = '|';
    dest[digitsSize + 3] = '\0';
    if(strSize != NULL) *strSize = digitsSize + 3;
    return dest;
}
void writeLineNumber(const int fd_dest, const int fd_src){
    char buffer[4096] = { 0 };
    char prefixStr[20] = { 0 };
    int bufferSize;
    int prefixSize;
    int linenum = 1;
    getLinePrefixStr(prefixStr, linenum++, &prefixSize);
    write(fd_dest, prefixStr, prefixSize);
    while ((bufferSize = read(fd_src, buffer, 4000)) > 0) {
        int left = 0, right;
        for (right = 0; right < bufferSize; right++) {
            if (buffer[right] == '\n') {
                write(fd_dest, &buffer[left], right - left + 1);
                getLinePrefixStr(prefixStr, linenum++, &prefixSize);
                write(fd_dest, prefixStr, prefixSize);
                left = right + 1;
            }
        }
        if (left != right - 1) {
            write(fd_dest, &buffer[left], bufferSize - left);
        }
    }
}
int main(){
    const char *path1 = "./Aladdin.txt";
    const char *path2 = "./Aladdin_num.txt";
    int fd1, fd2;
    fd1 = open(path1, O_RDONLY);
    fd2 = open(path2, O_WRONLY | O_CREAT | O_TRUNC, 755); 
    if (fd1 < 0 || fd2 < 0) _exit(1);
    writeLineNumber(fd2, fd1);
    if (close(fd1) < 0 || close(fd2) < 0) _exit(1);
}