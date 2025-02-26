#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <string.h>
// MAKEFILE YAZILIP DENENECEK 

#define SERVER_PORT 80

#define MAXLINE 4096
#define SA struct sockaddr

void err_n_die(const char *fmt, ...);

int main(int argc, char **argv){
        int             sockfd, n;
        int             sendbytes;
    struct sockaddr_in  servaddr;
        char            sendline[MAXLINE];
        char            recvline[MAXLINE];

    if(argc !=2){
        err_n_die("usage %s <server address>",argv[0]);
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM,0)) <0) {          /*socket() fonksiyonu, bir TCP/IP soketi oluşturur.
                                                                AF_INET → IPv4 kullanacağını belirtir.
                                                                SOCK_STREAM → TCP protokolü kullanılacağını söyler.
                                                                0 → Varsayılan protokolü (TCP) seçer.*/
        err_n_die("Error while creating the socket!");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        err_n_die("inet_pton error for %s",argv[1]);
    }

    /*struct sockaddr_in {
    sa_family_t    sin_family;   // AF_INET (IPv4)
    in_port_t      sin_port;     // Port (network byte order)
    struct in_addr sin_addr;     // IP adresi (binary format)
    char           sin_zero[8];  // Padding (sıfırlanır)
    };

    struct in_addr {
        uint32_t s_addr; // 32-bit IPv4 adresi (network byte order)
    };*/

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0){ // SOKETİ HEDEF SUNUCUYA BAĞLAMAYA ÇALIŞIYOR SA DA EN BAŞTA YAPILAN DEFINE İŞLEMİ 
        err_n_die("connect failed");
    }

    sprintf(sendline, "GET /HETTP/1.1\r\n\r\n"); //BURADA SENDLINE'A YANDAKİ DATAYI YAZIYORUZ SPRINTF BUFFER OVERFLOW YAPABİLİR BUNUN YERİNE SNPRINTF YAZILABİLİR!!
    sendbytes = strlen(sendline);

    if(write(sockfd, sendline, sendbytes) != sendbytes){ //write fonksiyonu, gönderilen bayt sayısını döndüren bir fonksiyondur
                                                         //eğer gönderilen bayt sayısı sendbytes'a eşit değilse hata meydana gelmiştir
        err_n_die("write error");
    }
    
    memset(recvline, 0, MAXLINE);
    
    while((n = read(sockfd, recvline, MAXLINE -1)) > 0){ // burada n değeri okuma yaptığı sürece 0 dan büyüktür okuma bittiği zaman 0 a eşit olur ve döngüden çıkılır
        printf("%s ",recvline);     // diziyi sonlandırmak için maxline-1 yapılıyor
    }
    if(n < 0){
        err_n_die("read error");
    }

    exit(0);


}