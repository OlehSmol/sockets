#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


const int SRV_PORT = 2233;

std::string cur_date(){
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    std::string day = std::to_string(aTime->tm_mday);
    std::string month = std::to_string(aTime->tm_mon + 1);
    std::string year = std::to_string(aTime->tm_year + 1900);

    if(day.length() == 1)
        day = "0" + day;
    if(month.length() == 1)
        month = "0" + month;

    return day + "-" + month + "-" + year;
}

std::string cur_time(){
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    std::string hour = std::to_string(aTime->tm_hour);
    std::string minute = std::to_string(aTime->tm_min);
    std::string second = std::to_string(aTime->tm_sec);

    if(hour.length() == 1)
        hour = "0" + hour;
    if(minute.length() == 1)
        minute = "0" + minute;
    if(second.length() == 1)
        second = "0" + second;

    return hour + ":" + minute + ":" + second;
}


int main() {
    std::string a = cur_time(), b = cur_date();
    struct sockaddr_in server;
    char buf [1024];
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    if( sd == -1 ) {
        std::cout << "Can`t create socket\n";
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(SRV_PORT);

    int res = bind(sd, (struct sockaddr *) &server, sizeof( server ) );
    if(res == -1) {
        std::cout << "Can`t bind socket\n";
        return -1;
    }

    listen (sd ,1);

    while(true){
        int psd = accept(sd, 0, 0);
        if (psd == -1) {
            std::cout << "Connection failed\n";
            continue;
        }
        close(sd);
        int cc = recv(psd, buf, sizeof(buf, 0), 0);
        if (cc == -1) {
            std::cout << "Receiving failed\n";
            continue;
        }
        buf[cc] = '\0';
        std::string msg;
        if(buf[0] == 'd'){
            std::cout << "D - query\n";
            msg = cur_date();
        } else if(buf[0] == 't'){
            std::cout << "T - query\n";
            msg = cur_time();
        } else if(buf[0] == 'h'){
            std::cout << "H - query\n";
            msg = "Hello";
        } else if(buf[0] == 'm'){
            std::cout << "M - query\n";
            //TODO
        }

        std::cout << msg << std::endl;
        if (send(psd, msg.c_str(), msg.length(), 0) == -1) {
            std::cout << "Message wasn`t sent\n";
            continue;
        }
    }

    return 0;
}