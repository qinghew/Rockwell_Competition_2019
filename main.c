#include "headfiles.h"

double start = 0,stop = 0; //clock_tΪclock()�������صı�������
double Current_Time = 0;

int main()
{
    Input_Init();       //�������ݳ�ʼ��
    start = clock();
//    printf("%d",Router_tree[Router_count].level)
    while(1)
    {

        //Sleep(500);   ��ʱ��������λms
        /**ѭ�����������Ƿ����ر������߸��±��������к���Ŀ����������**/
        Make_Decision();

        /**���ʱ�䣬�Ƿ��а����ػ��������**/
        check_time();

        if(Mission_completed_packages == Packages_Sum)      //���а������ء������������ʱ
        {
            break;
        }
        //stop = clock();
        //Current_Time = (stop-start)/CLK_TCK;
        //printf("\n ����ʱ�䣺 %f\n",Current_Time);
        //break;
    }

    printf("\n\n\nDear Client,is this what your want?\n");
    /**8.1wqh��ѭ��ÿ��·������ʾ����������**/
    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        printf("\n");
        for(int i = 0; i < Router_tree[Router_count].output_index ; i++ )   //�����ʾ
        {
            printf("  %d  ",Router_tree[Router_count].output[i]);
        }
        printf("\n");

    }
    printf("\n\nThe task completion time is %d seconds.\n\n",mission_completed_time);

    return 0;
}

