//
// Created by zhu on 24-9-9.
//
#include "get_data.h"
#include <thread>
int key=0,n=0;
char q;
void get_key()
{
    while(1)
    {
        scanf("%c",&q);

        key = 1;
        //std::cout<<key<<std::endl;

    }

}
void get_data(GetData raw)
{
    while(1)
    {
        if(q == 'q')
            exit(0);
        if(key == 1)
        {
            n=1;

            key = 0;
        }
        raw.get_data(n);
        n=0;

    }
}
int main()
{
    GetData raw;
    std::thread t(get_key);
    std::thread t1(get_data,raw);
    t.join();
    t1.join();

    return 0;

}