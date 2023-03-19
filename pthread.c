#include "head.h"



char paths[8][128] = {"/root/myuser/photo/messi.bmp", "/root/myuser/photo/snow.bmp",
                      "/root/myuser/photo/firegoose.bmp", "/root/myuser/photo/spy.bmp",
                      "/root/myuser/photo/qinghai.bmp", "/root/myuser/photo/food.bmp",
                      "/root/myuser/photo/maserati.bmp", "/root/myuser/photo/mi.bmp"};


//创建线程函数
void handle_pthread()
{
    
    pthread_create(&bmp_tid,NULL,call_show_bmp,NULL);
    pthread_create(&time_tid,NULL,call_show_time,NULL);
    pthread_create(&read_tid,NULL,call_readtxt,NULL);
    pthread_create(&weather_tid,NULL,call_show_weather,NULL);
    pthread_create(&rolltxt_tid,NULL,call_show_rolltxt,NULL);
    pthread_create(&music_tid,NULL,call_music_dis,NULL);
    pthread_join(bmp_tid,NULL);
    pthread_join(time_tid,NULL);
    pthread_join(read_tid,NULL);
    pthread_join(weather_tid,NULL);
    pthread_join(rolltxt_tid,NULL);
    pthread_join(music_tid,NULL);

}


//调用图片显示函数
void *call_show_bmp(void *arg)
{
    int flag = 0;
    while(1)
    {
        show_bmp(pic.x, pic.y, pic.w, pic.h, paths[flag]); 
        flag++;
        sleep(5);
        if(flag == 8)
        {
            // system("killall mplayer");
            // char buf[128];
            // sprintf(buf,"mplayer -slave -quiet -zoom -x %d -y %d -af volume=-30 -playlist ./midea/m.list &",pic.w,pic.h);
            // system(buf);
            // sleep(55);
            flag = 0; 
        }       
          
    }

    //printf("图片线程id:%d\n",bmp_tid);
    pthread_exit(0);
}

//调用时间显示函数
void *call_show_time(void *arg)
{
    while(1)
    {
        show_time();  
        usleep(30000);     
    }
    pthread_exit(0);
}

//调用读取配置文件函数
void *call_readtxt(void *arg)
{
    while(1)
    {
        readtxt();
        sleep(2);
    }
    pthread_exit(0);
}

//调用显示天气函数
void *call_show_weather(void *arg)
{
    while(1)
    {
        show_weather();
        sleep(2);
    }
    pthread_exit(0);
}

//调用显示滚动字幕函数
void *call_show_rolltxt(void *arg)
{
    show_rolltxt();
    pthread_exit(0);
}

//放音乐
void *call_music_dis(void *arg)
{
    music_dis();
    pthread_exit(0);    
}

