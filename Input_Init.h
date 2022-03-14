#ifndef _INPUT_INIT_h
#define _INPUT_INIT_h


extern void Input_Init();

extern int Router_Number_N;
extern int Single_Wire_M;
extern int Download_ULimit_P;
extern int Update_ULimit_Q;
extern int Router_count;
extern int level_now,level_total;
extern int Packages_Count,Packages_Sum;
extern int branch_count,branch_number;

/*初始化时要么有初值覆盖，要么就在初始化时清零了*/
struct Router    //每个路由器都有自己的层级、安装包数量、当前状态
{
    int level;

    /*
        condition =
        0 代表空闲状态（所有包都没在下载或安装，可能有的已经下载好了），
        1 代表有包正在下载，
        2 代表有包正在更新，3代表有包下载好了在等待更新
    */
    int condition;

    //由于单个路由器升级补丁包个数不超过10个，因此，定义一个2+10*2 = 22大小的数组存放路由器信息
    int information[22];

    /*
        boss含义:
        上级是谁，第一个路由器（数组里指第0个）的上级是自己，
        第二个路由器（数组里指第1个）的上级是第0个路由器(序号)
    */
    int boss;


    /*
        邮件：mail = 1，代表此时下级有人在下载包，本级路由器不能更新
        mail = 0，代表此时下级没有在下载包的，本级路由器可以更新
    */
    int mail;

    int mail_time_2_update_end; //正在更新的下级发来的，距离更新结束的时间

    int branch;     //因为高层都占很多分支，所以这个只是为了存用一下，真正存分支的是tree_branch数组

    int output[21];     //输出数组
    int output_index;   //输出数组的下标
    int sum_son_packages;   //下方路由器的安装包总数

    int is_or_not_5N;
};


struct branch       //分支顺序从右（0）往左
{
    int member[500];
    int members_number; //成员数
    int Line_Load_now;
};



/*初始化时要么有初值覆盖，要么就在初始化时清零了*/
struct Package
{
    int owner;                  //更新包属于哪个路由器(序号)
    int Load_time;              //下载耗时
    int Update_time;            //更新耗时
    float score;                //更新包的得分


    int condition;              //0代表没下载也没更新，1代表正在下载，2代表下载完成等更新，3代表正在更新，4代表更新完成
    int permission_to_load;     //允许下载标志位
    int permission_to_update;   //允许更新标志位
    double start_load_time;     //开始下载时间
    double start_update_time;   //开始更新时间


};


extern struct Router Router_tree[500];
extern struct Package Packages_ULimit[5000];    //最多有500*10个包
extern struct branch tree_branch[500];
#endif
