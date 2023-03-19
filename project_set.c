#include "head.h"

int pro_init()
{
    //打开屏幕
    lcdfd = open("/dev/fb0",O_RDWR);
    if(lcdfd == -1)
    {
        perror("打开屏幕失败\n");
        return -1;
    }

    //屏幕内存映射
    lcdp = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE ,MAP_SHARED,lcdfd,0);
    if(lcdp == MAP_FAILED)
    {
        perror("内存映射失败\n");
        return -1;
    }

    // dis_font(" ",600,0,10,400,0x4682B400,0);
    // dis_font(" ",610,160,190,10,0x4682B400,0);
    dis_font(" ",0,400,800,9,0x4682B400,0);


    return 0;
}

//显示图片
int show_bmp(int x,int y,int w,int h,char *path)
{
    int bmpfd = open(path,O_RDWR);
    if(bmpfd == -1)
    {
        perror("打开图片失败\n");
        return -1;
    }
    //读取图片信息
    int bw,bh;
    lseek(bmpfd,18,SEEK_SET);
    read(bmpfd,&bw,4);
    read(bmpfd,&bh,4);
    //printf("w=%d,h=%d\n",bw,bh);

    char bbuf[bw*bh*3];
    lseek(bmpfd,54,SEEK_SET);
    //read(bmpfd,bbuf,bw*bh*3);
    int rubbish = (4 - (bw * 3) % 4) % 4;
    //跳过垃圾数
    for(int i=0; i<bh; i++)
    {
        read(bmpfd,&bbuf[bw*3*i],bw*3);
        lseek(bmpfd,rubbish,SEEK_CUR);
    }

    if(w == -1 && h == -1)
    {
        w = bw;
        h = bh;
    }
    //放大或缩小的比值
    int reth = bh*100 / h;    //800:600 = 4:3
    int retw = bw*100 / w;    //480:400 = 6:5
    //将原图片放大或缩小后的像素存放到新的数组，过程中反转y轴
    char temp[w*h*3];
    for(int j=0; j<h; j++)
    {
        for(int k=0; k<w; k++)
        {
            temp[((h-1-j)*w+k)*3+0] = bbuf[(j*reth/100)*bw*3 + (k*retw/100)*3+0];               
            temp[((h-1-j)*w+k)*3+1] = bbuf[(j*reth/100)*bw*3 + (k*retw/100)*3+1];               
            temp[((h-1-j)*w+k)*3+2] = bbuf[(j*reth/100)*bw*3 + (k*retw/100)*3+2];               
        }
    }
    //内存映射写入到lcd文件
    int num = 0;
    int *newp = lcdp + 800*y+x;
    for(int y=0,n=0; y<h; y++)
    {
        for(int x=0; x<w; x++,n+=3)
        {           
            newp[800*y+x] = temp[n] | temp[n+1] << 8 | temp[n+2] << 16;
        }
    }

    close(bmpfd);
    return 0;
}

//显示时间
int show_time()
{
    time_t rawtime;
    struct tm *time_info;  //时间信息结构体
    //时间
    char today   [32] = {0};
    char now_time[32] = {0};
    char week    [32] = {0};

    time( &rawtime );

    time_info = localtime( &rawtime );
    //获取时间
    strftime(today,    32,"%F", time_info);
    strftime(now_time, 32,"%T", time_info);
    strftime(week,     32,"%a", time_info);

    //把英文缩写转换成中文
    if(strcmp(week,"Mon" ) == 0)   {  bzero(week,32);  strcpy(week, "星期一" );  } 
    if(strcmp(week,"Tue" ) == 0)   {  bzero(week,32);  strcpy(week, "星期二" );  } 
    if(strcmp(week,"Wed" ) == 0)   {  bzero(week,32);  strcpy(week, "星期三" );  } 
    if(strcmp(week,"Thu" ) == 0)   {  bzero(week,32);  strcpy(week, "星期四" );  } 
    if(strcmp(week,"Fri" ) == 0)   {  bzero(week,32);  strcpy(week, "星期五" );  } 
    if(strcmp(week,"Sat" ) == 0)   {  bzero(week,32);  strcpy(week, "星期六" );  } 
    if(strcmp(week,"Sun" ) == 0)   {  bzero(week,32);  strcpy(week, "星期天" );  }  
    
    //printf("%s  %s  %s\n", today,now_time,week );
    char now_time_all[128];
    //拼接时间字符串打印到屏幕
    sprintf(now_time_all,"%s\n%s\n%s",today,week,now_time);
   
    dis_font(now_time_all, tim.x, tim.y, tim.w, tim.h, tim.bgc, tim.foc);  
    
  
    // dis_font(today, tim.x, tim.y, tim.w, tim.h, tim.bgc, tim.foc);
    // dis_font(week, tim.x, tim.y+30, tim.w, tim.h, tim.bgc, tim.foc);
    // dis_font(now_time, tim.x, tim.y+60, tim.w, tim.h, tim.bgc, tim.foc);

    return 0;
}


//显示天气
int show_weather()
{
    char city[320] = {"广州\n16度 ~ 29度\n晴朗\n湿度39%\n空气质量指数:79"};


    dis_font(city, weather.x, weather.y, weather.w, weather.h, weather.bgc, weather.foc);


}


//显示滚动字幕
int show_rolltxt()
{
    char txtbuf[2][128] = {"计划是时间的最好保障，时间是效率的坚实基础，效率是行动的优化大师。         ",
                           "近水楼台此一家，悠然自得别无处。                "};
    int x = rolltext.x;
    int i = 0;
    while(1)
    {
        dis_font(txtbuf[i], x, rolltext.y, rolltext.w, rolltext.h, rolltext.bgc, rolltext.foc);
        if(x <= 0) x = 799;        
        x--;
    }
    
}


//播放音乐
int music_dis()
{
    system("mplayer -slave -quiet -af volume=-10 Experience.mp3 &");
    return 0;
}


//项目释放
int pro_free()
{
    
    close(lcdfd);
    munmap(lcdp,800*480*4);
    return 0;
}