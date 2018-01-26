#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include "TEST.h"

void executeCMD(const char *cmd, char *result)   
{   
    char buf_ps[1024];   
    char ps[1024]={0};   
    FILE *ptr;   
    strcpy(ps, cmd);  
    std::cout<< "the cmd is " <<ps <<std::endl; 
    if((ptr=popen(ps, "r"))!=NULL)   
    {
        int time = 0;
        while(fgets(buf_ps, 1024, ptr)!=NULL)   
        {   
            printf("current time = [%d]\n",++time);
           strcat(result, buf_ps);   
           if(strlen(result)>1024)   
               break;   
        }   
        pclose(ptr);   
        ptr = NULL;   
    }   
    else  
    {   
        printf("popen %s error\n", ps);   
    }   
}

std::string exec(const char* cmd) {
    // std::array<char, 128> buffer;
    std::string result;
    // std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    // if (!pipe) throw std::runtime_error("popen() failed!");
    // while (!feof(pipe.get())) {
    //     if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
    //         result += buffer.data();
    // }
    return result;
}
int exec1(const char* cmd, std::string &result) {
    char buffer[128];
    result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) 
    {
        result = "popen() failed!";
        return -2;
    }
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        return -1;
    }
    pclose(pipe);

    printf("print in func : \n%s",result.c_str());
    return 0;
}

// int main()
// {
//     char result[1024];
//     executeCMD( "ip route add 172.31.0.0/16 dev 209_201 metric 20", result);
//     int a = strlen(result);
//     printf("%s\n strlen = %d\n", result, a);
//     for(int i=0;i<1000;i++)
//         printf("%s\n",result[i]);
//     // char* strb= "helloworld";
//     // printf("%s\n strlen = %d\n", strb, strlen(strb));
//     return 0;
// }

int main(int argc, char* argv[])
{

    StdCapture::Init();

    char result[1024];
    StdCapture::BeginCapture();
    executeCMD( argv[1], result);
    StdCapture::EndCapture();
    printf("--executeCMD------------------------------\n");
    int a = strlen(result);
    printf("%s\n strlen = %d\n", result, a);
    printf("--executeCMD--end-------------------------\n");

    std::string captureStr = StdCapture::GetCapture();
    std::cout<< captureStr << std::endl;
    printf("--captureStr--end-------------------------\n");
    // int a = strlen(result);
    // printf("%s\n strlen = %d\n", result, a);
    // // for(int i=0;i<a;i++)
    // //     printf("%c",result[i]);
    // printf("--exec------------------------------------\n");
    // std::cout<<exec(argv[1])<<std::endl;

    // printf("--exec1-----------------------------------\n");
    // std::string result;
    // bool flag= false;
    // if (0==exec1(argv[1], result))
    // {
    //     if (result == "")
	// 	{
    //         printf("result is []\n");
	// 		flag = true;
	// 	}
    //     else if (result == "\n")
    //     {
    //         flag = true;
    //         printf("result is [\\n]\n");
    //     }
	// 	else
	// 	{

	// 		flag = false;
	// 	}
    // }
    // printf("--exec1-result-----------------------------\n");

    // std::cout<<result<<std::endl;

    // throw std::runtime_error("test runtime_error");
    // printf("!test runtime_error!\n");
    perror("test perror");
    fprintf( stderr, "test stderr\n" ); 

    return 0;
}