#include "headfiles.h"

extern double start,Current_Time;
int all_update_Router = 0;
int Mission_completed_packages = 0; //所有完成任务的包
int mission_completed_time = 0;     //任务完成总用时
void Make_Decision()        /**循环包，决策是否下载本包或者更新本包，其中含题目的限制条件**/
{


    //首先第一次运行到这儿是都在准备下载

    /**
        wqh 7.31晚：
        正着循环包吧，查每个包的boss（支路中的上级）没更新：下级在更新或者没工作，可以更新或者下载
                                                               下级在下载，可以下载，不能更新
                                                        如果当前包开始下载，则通知支路上级，此时不能更新
        wqh 8.1早：
        先完成功能之后，用三个for循环遍历3个参数吧代替启发式算法吧。

        wqh 8.1晚
            编号为5的倍数的路由器下载完会自动更新，想更新需要上级不在更新，且下级不在下载，为了安全，
            5N在下载时，不允许其他路由器更新，只允许下载，
            且级别越低的5N越早开始下载

    **/

    //按score分数循环包：第一次循环时下级包情况未知（但下级包也肯定没开始工作），第二次循环时就都连续已知了...
    for(Packages_Count = 0; Packages_Count < Packages_Sum; Packages_Count++)
    {   //此时的Packages_ULimit是从大到小排列的

        int refuse_load_now = 0;    //每个包都判断一次，是否本次拒绝下载
        int Up_level_no_update = 0; //上级没有在更新包的标志位


        Router_count = Packages_ULimit[Packages_Count].owner;
        //如果是5N，需要下方路由器全部下载完，则可以开始下载自己（不是最优，没时间做最优了）
        if(Router_tree[Router_count].is_or_not_5N == 1 && Router_tree[Router_count].sum_son_packages != 0)
        {
            continue;
        }




        if(Packages_ULimit[Packages_Count].condition == 1 || Packages_ULimit[Packages_Count].condition == 3
            || Packages_ULimit[Packages_Count].condition == 4)  //如果当前安装包已经更新完了，直接continue到下一个
        {
            continue;
        }


        /**循环每个包自己和它的boss，相当于循环了一条支路中自己的上面**/
        for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
        {
            if(Router_tree[boss_now].condition == 2)        //有上游路由器正在更新
            {
                Up_level_no_update = 1;
            }
            boss_now = Router_tree[boss_now].boss;  //另boss_now等于当前路由器的上一级（boss）


        }

        if(Up_level_no_update == 0)     //当前包的上级没在更新时
        {
                        //每个包想下载时都循环判断每个分支是否超过了P-1，如果超过了，循环判断这个分支里也没有当前包
            /**循环每个分支，判断有没有同时下载包数量>= P的，如果有，且如果该分支包含当前更新包所属路由器，就不能下载**/
            //计数每个支路正在下载包的路由器
            for(int i = 0; i<branch_number; i++)
            {
                tree_branch[i].Line_Load_now = 0;   //给当前分支清零
                for(int j = 0; j<tree_branch[i].members_number; j++)
                {
                    if(Router_tree[ tree_branch[i].member[j] ].condition == 1)  //正在下载包
                    {
                        tree_branch[i].Line_Load_now ++;        //计数当前分支正在下载包的路由器数
                    }
                }

                /**限制条件：同一支路同时接收升级包的路由器数目不能超过上限P**/
                if(tree_branch[i].Line_Load_now >= Download_ULimit_P)   //说明当前分支不可以再下载包
                {
                    for(int j = 0; j<tree_branch[i].members_number; j++)
                    {   //如果当前包归属的路由器是这个分支的，就不能下载
                        if(tree_branch[i].member[j] == Packages_ULimit[Packages_Count].owner)
                        {
                            refuse_load_now = 1;
                        }
                    }
                }
            }

            //如果此时满足限制条件，当前包为空闲状态
            if(refuse_load_now == 0 && Packages_ULimit[Packages_Count].condition == 0
                && Router_tree[Packages_ULimit[Packages_Count].owner].condition == 0)   //当前路由器也为空闲状态
            {
                Packages_ULimit[Packages_Count].permission_to_load = 1; //可以下载
            }


//记录每个路由器的下级sum_son_packages有多少包，每下载完一个包，循环通知上级son--


            if(Packages_ULimit[Packages_Count].condition == 2     //包已经下载完成了
                && Router_tree[Packages_ULimit[Packages_Count].owner].condition == 3  //当前路由器为等待更新状态
                && all_update_Router < Update_ULimit_Q      /**限制条件要小于Q**/
                /**此时下级没有在下载包的，可能在更新或者空闲，本路由器既可以下载也可以更新**/
                && Router_tree[Packages_ULimit[Packages_Count].owner].mail == 0
                /**当前路由器的更新时间比下级正在更新的路由器距离更新完成时间要短**/
                &&
                (Router_tree[Packages_ULimit[Packages_Count].owner].sum_son_packages == 0
                || (Packages_ULimit[Packages_Count].Update_time <= Router_tree[Packages_ULimit[Packages_Count].owner].mail_time_2_update_end
                     && Router_tree[Packages_ULimit[Packages_Count].owner].sum_son_packages != 0)))      //&& 下级还有包没下载完
            {                                               //下级的所有包都下载完了就可以直接执行了
                Packages_ULimit[Packages_Count].permission_to_update = 1;

                //防止循环到下一个包，不知道刚才有包开始更新了
                for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
                {
                    if(Packages_ULimit[Packages_Count].Update_time < Router_tree[boss_now].mail_time_2_update_end)
                    {
                        Router_tree[boss_now].mail_time_2_update_end = Packages_ULimit[Packages_Count].Update_time;
                    }   //刚开始，距离结束时间是该包的更新耗时

                    boss_now = Router_tree[boss_now].boss;  //另boss_now等于当前路由器的上一级（boss）
                }

                //printf("\n第2个路由器的mail = %d\n",Router_tree[1].mail);
                //printf("\n当前安装包的主人 = %d\n",Packages_ULimit[Packages_Count].owner);
            }

        }       //判断完就开工



        /**此时决定要下载了，要修改包和路由器的condition信息，下载完了要各种清零**/
        if(Packages_ULimit[Packages_Count].permission_to_load == 1)
        {   //最大值进了这个if就到上限不会再进
            //循环给上级发信息
            Packages_ULimit[Packages_Count].start_load_time = (clock() - start) / CLK_TCK * 50;       //记录开始时间

            printf("\n\n电站标识、接收开始时刻：%d ，%d",Packages_ULimit[Packages_Count].owner+1,(int)Packages_ULimit[Packages_Count].start_load_time);


            /**储存输出：开始下载时间**/
            Router_count = Packages_ULimit[Packages_Count].owner;
            Router_tree[Router_count].output[Router_tree[Router_count].output_index] = (int)Packages_ULimit[Packages_Count].start_load_time;
            Router_tree[Router_count].output_index ++;
            /**下载时要置一的标志位**/
            //循环每个包自己和它的boss，相当于循环了一条支路
            for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
            {
                Router_tree[boss_now].mail ++;     /**告诉上级我在下载，你不能更新，下载完给上级减回来**/
                if(boss_now != Packages_ULimit[Packages_Count].owner)
                {
                    Router_tree[boss_now].sum_son_packages --;      //告诉上级，你下级的包下载完一个，下级都下载完了，随意更新
                }

                //如果要更新则不需要告诉上级，前面也判断过上级有没有在更新的了，如果没有就可以自由更新
                boss_now = Router_tree[boss_now].boss;  //另boss_now等于当前路由器的上一级（boss）
            }

            Packages_ULimit[Packages_Count].condition = 1;  //我这个包正在下载喔
            Router_tree[Packages_ULimit[Packages_Count].owner].condition = 1;   //我这个路由器正在下载喔（终于下载上了卧槽）

        }


        /**此时决定要更新了**/
        if(Packages_ULimit[Packages_Count].permission_to_update == 1)
        {
            Packages_ULimit[Packages_Count].condition = 3;  //我这个包正在更新喔
            Router_tree[Packages_ULimit[Packages_Count].owner].condition = 2;  //我这个路由器正在更新喔
            /**all_update_Router记录全部正在更新的路由器，要保证小于限制条件Q**/
            all_update_Router ++;
            Packages_ULimit[Packages_Count].start_update_time = (clock() - start) / CLK_TCK * 50;       //记录开始时间

            printf("\n\n电站标识、更新开始时刻：%d ，%d",Packages_ULimit[Packages_Count].owner+1,(int)Packages_ULimit[Packages_Count].start_update_time);

            /**储存输出：开始更新时间**/
            Router_count = Packages_ULimit[Packages_Count].owner;
            Router_tree[Router_count].output[Router_tree[Router_count].output_index] = (int)Packages_ULimit[Packages_Count].start_update_time;
            Router_tree[Router_count].output_index ++;    //也起到给输出元素的个数计数的效果
        }
    }


    //相当于复位，下面重新找一圈每个路由器的下级路由器距离更新完的最短时间
    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        Router_tree[Router_count].mail_time_2_update_end = 50000;
    }


}




void check_time()       /**检查时间，是否有包下载或更新完了**/
{

    for(Packages_Count = 0; Packages_Count < Packages_Sum; Packages_Count++)
    {
        int duration_each_package = 0;
        //只判断此时正在下载或者正在更新的包

        if(Packages_ULimit[Packages_Count].condition == 0 || Packages_ULimit[Packages_Count].condition == 2
            || Packages_ULimit[Packages_Count].condition == 4)
        {
            continue;
        }

        Current_Time = (clock()-start) / CLK_TCK * 50;   //浮点型


        //得出每个正在下载或正在更新的安装包的进行时间（整型）！
        duration_each_package = Current_Time - Packages_ULimit[Packages_Count].start_load_time;

        if(Packages_ULimit[Packages_Count].condition == 1)      //正在下载的话，判断时间到没到
        {
             if(duration_each_package >= Packages_ULimit[Packages_Count].Load_time) //如果时间够了，下载好了
             {      /**清零！！！**/

                    for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
                    {
                        Router_tree[boss_now].mail --;     /**下载完给上级减回来**/
                        boss_now = Router_tree[boss_now].boss;  //另boss_now等于当前路由器的上一级（boss）
                    }

                    Packages_ULimit[Packages_Count].condition = 2;  //我这个包下载好了喔，等待更新
                    Router_tree[Packages_ULimit[Packages_Count].owner].condition = 3;   //我这个路由器在等待更新
                    Packages_ULimit[Packages_Count].permission_to_load = 0;

                    printf("\n\n电站标识、下载完时刻：%d ，%d",Packages_ULimit[Packages_Count].owner+1,(int)Current_Time);

                    if(Router_tree[Packages_ULimit[Packages_Count].owner].is_or_not_5N == 1) //如果是5N的路由器，下载好自动开始更新
                    {
                        Packages_ULimit[Packages_Count].permission_to_update = 1;
                    }


             }
        }


        //得出每个正在下载或正在更新的安装包的进行时间（整型）！
        duration_each_package = Current_Time - Packages_ULimit[Packages_Count].start_update_time;

        if(Packages_ULimit[Packages_Count].condition == 3)      //正在更新的话，判断时间到没到
        {
             if(duration_each_package >= Packages_ULimit[Packages_Count].Update_time) //如果时间够了，下载好了
             {      /**清零！！！**/
                    Packages_ULimit[Packages_Count].condition = 4;  //我这个包更新完了！
                    Router_tree[Packages_ULimit[Packages_Count].owner].condition = 0;  //我这个路由器处于空闲（也可能其实是4，不过没影响）
                    all_update_Router --;    //更新完减回来
                    Packages_ULimit[Packages_Count].permission_to_update = 0;
                    Mission_completed_packages ++;

                    printf("\n\n电站标识、更新完时刻：%d ，%d",Packages_ULimit[Packages_Count].owner+1,(int)Current_Time);
                    mission_completed_time = Current_Time;


             }
             else
             {
                    for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
                    {
                        /**我更新包主要怕耽误下面的包更新完了不能下载！所以小于下级最小的时间就可以更新**/
                        //如果当前包的距离更新结束时间 < 当前路由器存储的其他包的时间，就覆盖掉
                        if(Packages_ULimit[Packages_Count].Update_time - duration_each_package < Router_tree[boss_now].mail_time_2_update_end)
                        {
                            Router_tree[boss_now].mail_time_2_update_end = Packages_ULimit[Packages_Count].Update_time - duration_each_package;
                        }

                        //如果要更新则不需要告诉上级，前面也判断过上级有没有在更新的了，如果没有就可以自由更新
                        boss_now = Router_tree[boss_now].boss;  //另boss_now等于当前路由器的上一级（boss）
                    }

             }
        }
    }
}

            //stop = clock();
            //duration = (stop-start)/CLK_TCK;
