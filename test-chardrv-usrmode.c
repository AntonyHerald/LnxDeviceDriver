/* This is Usermode driver code 
 * which refers to /dev/AntoCharDevice
 */

#include <unistd.h>
#include <fcntl.h>        
#include <stdio.h>
#include <string.h>

int main()
{
    int fd;
    int ret = 0;
    char buff[100];
    char strwrite[]="ANTONIO-HERALD-IGNATIUS \0";

    fd = open("/dev/TestFinalDr",O_RDWR);

    if(fd < 0) {
        printf("/dev/TestFinalDr (ret = %d)\n",fd);
        return 1;   
    }

    ret = read(fd,buff,10);
    printf("%s\n",buff);    

    ret = write(fd, strwrite, strlen(strwrite)); 

    close(fd);

    return 1;
}
