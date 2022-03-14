#include "headfiles.h"

/**路由器变量**/
int Router_Number_N = 0;    //路由器数量
int Single_Wire_M = 0;      //单向电线数量
int Download_ULimit_P = 0;  //同一支路同时接收升级包路由器上限P
int Update_ULimit_Q = 0;    //同时更新电站上限Q
int level_now = 2,level_total = 0;
int Router_count = 0;   //当前是第几个路由器
struct Router Router_tree[500]; //定义路由器数组，一共Router_Number_N个路由器


/**支路变量**/
int branch_number = 0;   //支路数量
struct branch tree_branch[500];


/**安装包变量**/
int Packages_Sum = 0,Packages_Count = 0;   //实际包的数量
struct Package Packages_ULimit[5000] = {0};    //最多有500*10个包
int min_load_time = 500,max_load_time = 0;
int min_update_time = 500,max_update_time = 0;

//    Router_all = Router_tree;


void Input_Init()
{
    printf("Welcome to Rockwell!!!\n\n");
    printf("！！！输入时请用 英文逗号 隔开，输完一组参数请按回车键\n\n");

    printf("路由器数N，单向电线数M，同一支路同时接收升级包路由器上限P，同时更新电站上限Q:");
    scanf("%d,%d,%d,%d",&Router_Number_N,&Single_Wire_M,&Download_ULimit_P,&Update_ULimit_Q);
    printf("\n本次输入路由器数 %d ，单向电线数 %d ，同一支路同时接收升级包路由器上限 %d ，同时更新电站上限 %d :\n",Router_Number_N,Single_Wire_M,Download_ULimit_P,Update_ULimit_Q);

    //struct Router Router_tree[Router_Number_N]; //定义路由器数组，一共Router_Number_N个路由器
    //Router_tree只是这个子函数里可以用

    //初始化清零路由器的一些参数
    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        Router_tree[Router_count].condition = 0;
        Router_tree[Router_count].boss = 0;
        Router_tree[Router_count].level = 0;
        Router_tree[Router_count].mail = 0;
        Router_tree[Router_count].branch = 100;
        Router_tree[Router_count].output_index = 0;
        Router_tree[Router_count].mail_time_2_update_end = 50000;
        Router_tree[Router_count].sum_son_packages = 0;

        Router_tree[Router_count].is_or_not_5N = 0;
    }


    Router_tree[0].level = 1;   //第一个的级别一定是最高级 1
    Router_tree[0].boss = -1;       //第1个的boss的-1
    Router_count = 1;       //第一个录了，直接从第二个开始
    int head = 1,tail = 0;  //本次输入的起始、终止
    int last_head = 1;//last_tail = 0;    //上次输入的起始、终止

    for(int line_count = 1; line_count<=Single_Wire_M; line_count ++,Router_count ++)
    {  /*
        Router_count代表第几个路由器（从第二(1)个路由器开始），
        level_now代表当前层级（从第二级开始），
        line_count代表第几根连线
        */
        printf("\n路由器到路由器的单向边:");

        scanf("%d,%d",&head,&tail);        //输入本次的头尾

        if(Router_tree[head-1].level == Router_tree[last_head-1].level)
        {  /*
            注意：
            1.如果起始的路由器和上次起始的路由器级别相等，说明当前路由器没到下一级
            2.在起点切换到下一个起点时，可能是同一级起点的切换
            */
            Router_tree[Router_count].level = level_now;
        }
        else
        {
            level_now ++;
            Router_tree[Router_count].level = level_now;
        }

        //记录该路由器的上级是谁！这个影响当前路由器能不能与 不和它在一条支路上的路由器 同时更新
        Router_tree[Router_count].boss = head - 1;

        printf("第 %d 个路由器属于第 %d 级:\n\n",Router_count+1,Router_tree[Router_count].level);
        last_head = head;   //储存上次的值
        //last_tail = tail;
    }

    level_total = level_now;    //记录一共几级

    for(level_now = 1; level_now<=level_total; level_now++)    //显示所有级包含的路由器序号
    {

        printf("\n\n第 %d 级:",level_now);
        for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
        {
            if(Router_tree[Router_count].level == level_now)
            {
                printf(" %d ",Router_count + 1);
            }
        }
    }






    printf("\n\n接下来请输入\n路由器信息:编号，升级补丁包个数，接收升级包耗时，更新耗时......\n");

    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        for(int i = 0; i<22; i++)       //先给结构体里的信息数组初始化清零，再进行输入
        {
            Router_tree[Router_count].information[i] = 0;
        }

        printf("\n第 %d 个路由器信息:",Router_count+1);
        /*不管有几个数要输入，只要输完按下回车，就跳到下一个要输入的路由器信息里了*/
        scanf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
        &Router_tree[Router_count].information[0],&Router_tree[Router_count].information[1],
        &Router_tree[Router_count].information[2],&Router_tree[Router_count].information[3],
        &Router_tree[Router_count].information[4],&Router_tree[Router_count].information[5],
        &Router_tree[Router_count].information[6],&Router_tree[Router_count].information[7],
        &Router_tree[Router_count].information[8],&Router_tree[Router_count].information[9],
        &Router_tree[Router_count].information[10],&Router_tree[Router_count].information[11],
        &Router_tree[Router_count].information[12],&Router_tree[Router_count].information[13],
        &Router_tree[Router_count].information[14],&Router_tree[Router_count].information[15],
        &Router_tree[Router_count].information[16],&Router_tree[Router_count].information[17],
        &Router_tree[Router_count].information[18],&Router_tree[Router_count].information[19],
        &Router_tree[Router_count].information[20],&Router_tree[Router_count].information[21]
        );

        if(Router_tree[Router_count].information[0] % 5 == 0)   //标号从1开始
        {
            Router_tree[Router_count].is_or_not_5N = 1;
        }
        else
        {
            Router_tree[Router_count].is_or_not_5N = 0;
        }

        //每行输出的第一个数都等于输入路由器信息的第一个数
        Router_tree[Router_count].output[0] = Router_tree[Router_count].information[0];
        Router_tree[Router_count].output_index ++;  //下标往后挪一位
    }


    //显示输入的信息
    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        printf("\n\n第 %d 个路由器信息:",Router_count+1);

        for(int i = 0; i<Router_tree[Router_count].information[1]*2 + 2; i++)
        {               //Router_tree[Router_count].information[1]代表有几个安装包
            printf(" %d ",Router_tree[Router_count].information[i]);
        }

        /*
            找所有安装包里的下载最长最短时间、更新最长最短时间，
            在数据归一化里用
        */
        for(int i = 0; i<Router_tree[Router_count].information[1]; i++)
        {
            if(Router_tree[Router_count].information[2 + i*2] < min_load_time)
            {   //最短下载时间
                min_load_time = Router_tree[Router_count].information[2 + i*2];
            }

            if(Router_tree[Router_count].information[2 + i*2 + 1] < min_update_time)
            {   //最短更新时间
                min_update_time = Router_tree[Router_count].information[2 + i*2 + 1];
            }

            if(Router_tree[Router_count].information[2 + i*2] > max_load_time)
            {   //最长下载时间
                max_load_time = Router_tree[Router_count].information[2 + i*2];
            }

            if(Router_tree[Router_count].information[2 + i*2 + 1] > max_update_time)
            {   //最长更新时间
                max_update_time = Router_tree[Router_count].information[2 + i*2 + 1];
            }

        }
        Packages_Sum += Router_tree[Router_count].information[1];       //计算更新包的总数

    }

    printf("\n\nmin_load_time = %d\n",min_load_time);
    printf("\nmin_update_time = %d\n",min_update_time);
    printf("\nmax_load_time = %d\n",max_load_time);
    printf("\nmax_update_time = %d\n",max_update_time);


    /**录入分支信息**/
    for(int i = 0;i<500;i++)
    {
        tree_branch[i].members_number = 0;     //存储各支路正在接收升级包的路由器数目
        tree_branch[i].Line_Load_now = 0;
    }

    Router_count = Router_Number_N-1;
    for(int i=0; Router_count>=0; Router_count--)
    {   //如果当前路由器没被录入过分支，即只录入最下面一行的路由器
        if(Router_tree[Router_count].branch == 100)
        {
            for(int boss_now = Router_count,j = 0; boss_now >= 0; j++)       //循环每个包自己和它的boss，相当于循环了一条支路
            {                               //j代表第i个分支中的第j个路由器
                Router_tree[boss_now].branch = i;
                tree_branch[i].member[j] = boss_now;       //i代表几个分支，j代表分支中的元素，存放的是路由器的序号
                tree_branch[i].members_number ++;

                //printf("\n\n第%d个分支的第%d个成员%d",i,j,tree_branch[i].member[j]);

                boss_now = Router_tree[boss_now].boss;
            }

            i++;        //i代表分支号，分支顺序从右（0）往左

        }
        branch_number = i;  //最后一次存的i就是分支的最终数量
    }

    //循环每个路由器，向所有直系上级汇报自己的安装包个数
    for(Router_count = Router_Number_N-1; Router_count>=0; Router_count--)
    {   //如果当前路由器没被录入过分支，即只录入最下面一行的路由器

            for(int boss_now = Router_count; boss_now >= 0;)       //循环每个包自己和它的boss，相当于循环了一条支路
            {                               //j代表第i个分支中的第j个路由器
                //printf("\n\n第%d个分支的第%d个成员%d",i,j,tree_branch[i].member[j]);
                //给老大把自己有多少包汇报上去
                if(boss_now != Router_count)    //别记自己
                {
                    Router_tree[boss_now].sum_son_packages += Router_tree[Router_count].information[1];
                }

                boss_now = Router_tree[boss_now].boss;
            }

    }







    /*
        *类似于层次分析法，这种方法肯定不如启发式算法效果好，启发式算法就像把所有可能性有选择性地遍历一遍，
        *但条件太多，这样主观的评价简单一些......
        *主观权重给各个路由器的软件包评价分数
        *权重原因：
            1.level级：level越高，越迟更新，因为级高的开始更新会耽误级低的下载、更新  占0.5
            2.下载时间：下载时间越长越耽误上级更新                                    占0.2
            3.更新时间：更新时上游可以更新，但下游只能干等待，更容易耽误事儿          占0.3
            下游更新的时候，叫上上游一起更新比较好

        *步骤：
            1. 3类数据分别归一化
            2. 计算各路由器的各个包的分数
    */

    /*第一步，将更新包分类*/
//Packages_Sum
    //Packages_ULimit[]

    for(Packages_Count = 0,Router_count = 0; Router_count<Router_Number_N; Router_count++)     //每个路由器的循环
    {
        for(int i = 0; i<Router_tree[Router_count].information[1]; i++,Packages_Count++)     //每个路由器的每个包都循环一次
        {
            Packages_ULimit[Packages_Count].owner = Router_count;   //记录更新包属于哪个路由器

            Packages_ULimit[Packages_Count].Load_time = Router_tree[Router_count].information[2 + i*2];     //记录下载耗时
            Packages_ULimit[Packages_Count].Update_time = Router_tree[Router_count].information[2 + i*2 + 1];   //记录更新耗时
            //归一化—————乘权重 =  得出分数


            Packages_ULimit[Packages_Count].score = 0.8 * (Router_tree[Router_count].level - 1)*1.0 / (level_total - 1)   //越大越好，正相关，后两行是负相关，所以有 （1-）
                                                    + 0.1 * (1 - (Router_tree[Router_count].information[2 + i*2] - min_load_time)*1.0 / (max_load_time - min_load_time))
                                                     + 0.1 * (1 - (Router_tree[Router_count].information[2 + i*2 + 1] - min_update_time)*1.0 / (max_update_time - min_update_time));


            printf("\n第%d个包的 score = %f\n",Packages_Count,Packages_ULimit[Packages_Count].score);
        }

    }


    /*冒泡法排序，将 Packages_ULimit 中所有的分数从大到小排列出来*/
    float temp = 0;
    for (int i = 0; i < Packages_Sum - 1; i++)
    {
            for (int j = i + 1; j < Packages_Sum; j++)
            {
                if (Packages_ULimit[i].score < Packages_ULimit[j].score)
                { // 交换两数的位置        //根据分数重新把其他参数也跟着重新排序
                    temp = Packages_ULimit[i].score;
                    Packages_ULimit[i].score = Packages_ULimit[j].score;
                    Packages_ULimit[j].score = temp;

                    temp = Packages_ULimit[i].owner;
                    Packages_ULimit[i].owner = Packages_ULimit[j].owner;
                    Packages_ULimit[j].owner = temp;

                    temp = Packages_ULimit[i].Load_time;
                    Packages_ULimit[i].Load_time = Packages_ULimit[j].Load_time;
                    Packages_ULimit[j].Load_time = temp;

                    temp = Packages_ULimit[i].Update_time;
                    Packages_ULimit[i].Update_time = Packages_ULimit[j].Update_time;
                    Packages_ULimit[j].Update_time = temp;

                }
            }
    }
    for (int i = 0; i < Packages_Sum; i++)
    {
        Packages_ULimit[i].permission_to_load = 0;
        Packages_ULimit[i].permission_to_update = 0;
        Packages_ULimit[i].condition = 0;
        Packages_ULimit[i].start_load_time = 0;
        Packages_ULimit[i].start_update_time = 0;

        printf("\n第%d个路由器的包的 分数 = %f\n",Packages_ULimit[i].owner,Packages_ULimit[i].score);
    }

    /*初始化完毕*/



}
