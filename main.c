#include "headfiles.h"

double start = 0,stop = 0; //clock_t为clock()函数返回的变量类型
double Current_Time = 0;

int main()
{
    Input_Init();       //输入数据初始化
    start = clock();
//    printf("%d",Router_tree[Router_count].level)
    while(1)
    {

        //Sleep(500);   延时函数，单位ms
        /**循环包，决策是否下载本包或者更新本包，其中含题目的限制条件**/
        Make_Decision();

        /**检查时间，是否有包下载或更新完了**/
        check_time();

        if(Mission_completed_packages == Packages_Sum)      //所有包的下载、更新任务都完成时
        {
            break;
        }
        //stop = clock();
        //Current_Time = (stop-start)/CLK_TCK;
        //printf("\n 运行时间： %f\n",Current_Time);
        //break;
    }

    printf("\n\n\nDear Client,is this what your want?\n");
    /**8.1wqh：循环每个路由器显示输出结果！！**/
    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        printf("\n");
        for(int i = 0; i < Router_tree[Router_count].output_index ; i++ )   //输出显示
        {
            printf("  %d  ",Router_tree[Router_count].output[i]);
        }
        printf("\n");

    }
    printf("\n\nThe task completion time is %d seconds.\n\n",mission_completed_time);

    return 0;
}

