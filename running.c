#include "headfiles.h"

extern double start,Current_Time;
int all_update_Router = 0;
int Mission_completed_packages = 0; //�����������İ�
int mission_completed_time = 0;     //�����������ʱ
void Make_Decision()        /**ѭ�����������Ƿ����ر������߸��±��������к���Ŀ����������**/
{


    //���ȵ�һ�����е�����Ƕ���׼������

    /**
        wqh 7.31��
        ����ѭ�����ɣ���ÿ������boss��֧·�е��ϼ���û���£��¼��ڸ��»���û���������Ը��»�������
                                                               �¼������أ��������أ����ܸ���
                                                        �����ǰ����ʼ���أ���֪֧ͨ·�ϼ�����ʱ���ܸ���
        wqh 8.1�磺
        ����ɹ���֮��������forѭ������3�������ɴ�������ʽ�㷨�ɡ�

        wqh 8.1��
            ���Ϊ5�ı�����·������������Զ����£��������Ҫ�ϼ����ڸ��£����¼��������أ�Ϊ�˰�ȫ��
            5N������ʱ������������·�������£�ֻ�������أ�
            �Ҽ���Խ�͵�5NԽ�翪ʼ����

    **/

    //��score����ѭ��������һ��ѭ��ʱ�¼������δ֪�����¼���Ҳ�϶�û��ʼ���������ڶ���ѭ��ʱ�Ͷ�������֪��...
    for(Packages_Count = 0; Packages_Count < Packages_Sum; Packages_Count++)
    {   //��ʱ��Packages_ULimit�ǴӴ�С���е�

        int refuse_load_now = 0;    //ÿ�������ж�һ�Σ��Ƿ񱾴ξܾ�����
        int Up_level_no_update = 0; //�ϼ�û���ڸ��°��ı�־λ


        Router_count = Packages_ULimit[Packages_Count].owner;
        //�����5N����Ҫ�·�·����ȫ�������꣬����Կ�ʼ�����Լ����������ţ�ûʱ���������ˣ�
        if(Router_tree[Router_count].is_or_not_5N == 1 && Router_tree[Router_count].sum_son_packages != 0)
        {
            continue;
        }




        if(Packages_ULimit[Packages_Count].condition == 1 || Packages_ULimit[Packages_Count].condition == 3
            || Packages_ULimit[Packages_Count].condition == 4)  //�����ǰ��װ���Ѿ��������ˣ�ֱ��continue����һ��
        {
            continue;
        }


        /**ѭ��ÿ�����Լ�������boss���൱��ѭ����һ��֧·���Լ�������**/
        for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
        {
            if(Router_tree[boss_now].condition == 2)        //������·�������ڸ���
            {
                Up_level_no_update = 1;
            }
            boss_now = Router_tree[boss_now].boss;  //��boss_now���ڵ�ǰ·��������һ����boss��


        }

        if(Up_level_no_update == 0)     //��ǰ�����ϼ�û�ڸ���ʱ
        {
                        //ÿ����������ʱ��ѭ���ж�ÿ����֧�Ƿ񳬹���P-1����������ˣ�ѭ���ж������֧��Ҳû�е�ǰ��
            /**ѭ��ÿ����֧���ж���û��ͬʱ���ذ�����>= P�ģ�����У�������÷�֧������ǰ���°�����·�������Ͳ�������**/
            //����ÿ��֧·�������ذ���·����
            for(int i = 0; i<branch_number; i++)
            {
                tree_branch[i].Line_Load_now = 0;   //����ǰ��֧����
                for(int j = 0; j<tree_branch[i].members_number; j++)
                {
                    if(Router_tree[ tree_branch[i].member[j] ].condition == 1)  //�������ذ�
                    {
                        tree_branch[i].Line_Load_now ++;        //������ǰ��֧�������ذ���·������
                    }
                }

                /**����������ͬһ֧·ͬʱ������������·������Ŀ���ܳ�������P**/
                if(tree_branch[i].Line_Load_now >= Download_ULimit_P)   //˵����ǰ��֧�����������ذ�
                {
                    for(int j = 0; j<tree_branch[i].members_number; j++)
                    {   //�����ǰ��������·�����������֧�ģ��Ͳ�������
                        if(tree_branch[i].member[j] == Packages_ULimit[Packages_Count].owner)
                        {
                            refuse_load_now = 1;
                        }
                    }
                }
            }

            //�����ʱ����������������ǰ��Ϊ����״̬
            if(refuse_load_now == 0 && Packages_ULimit[Packages_Count].condition == 0
                && Router_tree[Packages_ULimit[Packages_Count].owner].condition == 0)   //��ǰ·����ҲΪ����״̬
            {
                Packages_ULimit[Packages_Count].permission_to_load = 1; //��������
            }


//��¼ÿ��·�������¼�sum_son_packages�ж��ٰ���ÿ������һ������ѭ��֪ͨ�ϼ�son--


            if(Packages_ULimit[Packages_Count].condition == 2     //���Ѿ����������
                && Router_tree[Packages_ULimit[Packages_Count].owner].condition == 3  //��ǰ·����Ϊ�ȴ�����״̬
                && all_update_Router < Update_ULimit_Q      /**��������ҪС��Q**/
                /**��ʱ�¼�û�������ذ��ģ������ڸ��»��߿��У���·�����ȿ�������Ҳ���Ը���**/
                && Router_tree[Packages_ULimit[Packages_Count].owner].mail == 0
                /**��ǰ·�����ĸ���ʱ����¼����ڸ��µ�·��������������ʱ��Ҫ��**/
                &&
                (Router_tree[Packages_ULimit[Packages_Count].owner].sum_son_packages == 0
                || (Packages_ULimit[Packages_Count].Update_time <= Router_tree[Packages_ULimit[Packages_Count].owner].mail_time_2_update_end
                     && Router_tree[Packages_ULimit[Packages_Count].owner].sum_son_packages != 0)))      //&& �¼����а�û������
            {                                               //�¼������а����������˾Ϳ���ֱ��ִ����
                Packages_ULimit[Packages_Count].permission_to_update = 1;

                //��ֹѭ������һ��������֪���ղ��а���ʼ������
                for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
                {
                    if(Packages_ULimit[Packages_Count].Update_time < Router_tree[boss_now].mail_time_2_update_end)
                    {
                        Router_tree[boss_now].mail_time_2_update_end = Packages_ULimit[Packages_Count].Update_time;
                    }   //�տ�ʼ���������ʱ���Ǹð��ĸ��º�ʱ

                    boss_now = Router_tree[boss_now].boss;  //��boss_now���ڵ�ǰ·��������һ����boss��
                }

                //printf("\n��2��·������mail = %d\n",Router_tree[1].mail);
                //printf("\n��ǰ��װ�������� = %d\n",Packages_ULimit[Packages_Count].owner);
            }

        }       //�ж���Ϳ���



        /**��ʱ����Ҫ�����ˣ�Ҫ�޸İ���·������condition��Ϣ����������Ҫ��������**/
        if(Packages_ULimit[Packages_Count].permission_to_load == 1)
        {   //���ֵ�������if�͵����޲����ٽ�
            //ѭ�����ϼ�����Ϣ
            Packages_ULimit[Packages_Count].start_load_time = (clock() - start) / CLK_TCK * 50;       //��¼��ʼʱ��

            printf("\n\n��վ��ʶ�����տ�ʼʱ�̣�%d ��%d",Packages_ULimit[Packages_Count].owner+1,(int)Packages_ULimit[Packages_Count].start_load_time);


            /**�����������ʼ����ʱ��**/
            Router_count = Packages_ULimit[Packages_Count].owner;
            Router_tree[Router_count].output[Router_tree[Router_count].output_index] = (int)Packages_ULimit[Packages_Count].start_load_time;
            Router_tree[Router_count].output_index ++;
            /**����ʱҪ��һ�ı�־λ**/
            //ѭ��ÿ�����Լ�������boss���൱��ѭ����һ��֧·
            for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
            {
                Router_tree[boss_now].mail ++;     /**�����ϼ��������أ��㲻�ܸ��£���������ϼ�������**/
                if(boss_now != Packages_ULimit[Packages_Count].owner)
                {
                    Router_tree[boss_now].sum_son_packages --;      //�����ϼ������¼��İ�������һ�����¼����������ˣ��������
                }

                //���Ҫ��������Ҫ�����ϼ���ǰ��Ҳ�жϹ��ϼ���û���ڸ��µ��ˣ����û�оͿ������ɸ���
                boss_now = Router_tree[boss_now].boss;  //��boss_now���ڵ�ǰ·��������һ����boss��
            }

            Packages_ULimit[Packages_Count].condition = 1;  //����������������
            Router_tree[Packages_ULimit[Packages_Count].owner].condition = 1;   //�����·������������ร��������������Բۣ�

        }


        /**��ʱ����Ҫ������**/
        if(Packages_ULimit[Packages_Count].permission_to_update == 1)
        {
            Packages_ULimit[Packages_Count].condition = 3;  //����������ڸ����
            Router_tree[Packages_ULimit[Packages_Count].owner].condition = 2;  //�����·�������ڸ����
            /**all_update_Router��¼ȫ�����ڸ��µ�·������Ҫ��֤С����������Q**/
            all_update_Router ++;
            Packages_ULimit[Packages_Count].start_update_time = (clock() - start) / CLK_TCK * 50;       //��¼��ʼʱ��

            printf("\n\n��վ��ʶ�����¿�ʼʱ�̣�%d ��%d",Packages_ULimit[Packages_Count].owner+1,(int)Packages_ULimit[Packages_Count].start_update_time);

            /**�����������ʼ����ʱ��**/
            Router_count = Packages_ULimit[Packages_Count].owner;
            Router_tree[Router_count].output[Router_tree[Router_count].output_index] = (int)Packages_ULimit[Packages_Count].start_update_time;
            Router_tree[Router_count].output_index ++;    //Ҳ�𵽸����Ԫ�صĸ���������Ч��
        }
    }


    //�൱�ڸ�λ������������һȦÿ��·�������¼�·�����������������ʱ��
    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        Router_tree[Router_count].mail_time_2_update_end = 50000;
    }


}




void check_time()       /**���ʱ�䣬�Ƿ��а����ػ��������**/
{

    for(Packages_Count = 0; Packages_Count < Packages_Sum; Packages_Count++)
    {
        int duration_each_package = 0;
        //ֻ�жϴ�ʱ�������ػ������ڸ��µİ�

        if(Packages_ULimit[Packages_Count].condition == 0 || Packages_ULimit[Packages_Count].condition == 2
            || Packages_ULimit[Packages_Count].condition == 4)
        {
            continue;
        }

        Current_Time = (clock()-start) / CLK_TCK * 50;   //������


        //�ó�ÿ���������ػ����ڸ��µİ�װ���Ľ���ʱ�䣨���ͣ���
        duration_each_package = Current_Time - Packages_ULimit[Packages_Count].start_load_time;

        if(Packages_ULimit[Packages_Count].condition == 1)      //�������صĻ����ж�ʱ�䵽û��
        {
             if(duration_each_package >= Packages_ULimit[Packages_Count].Load_time) //���ʱ�乻�ˣ����غ���
             {      /**���㣡����**/

                    for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
                    {
                        Router_tree[boss_now].mail --;     /**��������ϼ�������**/
                        boss_now = Router_tree[boss_now].boss;  //��boss_now���ڵ�ǰ·��������һ����boss��
                    }

                    Packages_ULimit[Packages_Count].condition = 2;  //����������غ���ร��ȴ�����
                    Router_tree[Packages_ULimit[Packages_Count].owner].condition = 3;   //�����·�����ڵȴ�����
                    Packages_ULimit[Packages_Count].permission_to_load = 0;

                    printf("\n\n��վ��ʶ��������ʱ�̣�%d ��%d",Packages_ULimit[Packages_Count].owner+1,(int)Current_Time);

                    if(Router_tree[Packages_ULimit[Packages_Count].owner].is_or_not_5N == 1) //�����5N��·���������غ��Զ���ʼ����
                    {
                        Packages_ULimit[Packages_Count].permission_to_update = 1;
                    }


             }
        }


        //�ó�ÿ���������ػ����ڸ��µİ�װ���Ľ���ʱ�䣨���ͣ���
        duration_each_package = Current_Time - Packages_ULimit[Packages_Count].start_update_time;

        if(Packages_ULimit[Packages_Count].condition == 3)      //���ڸ��µĻ����ж�ʱ�䵽û��
        {
             if(duration_each_package >= Packages_ULimit[Packages_Count].Update_time) //���ʱ�乻�ˣ����غ���
             {      /**���㣡����**/
                    Packages_ULimit[Packages_Count].condition = 4;  //��������������ˣ�
                    Router_tree[Packages_ULimit[Packages_Count].owner].condition = 0;  //�����·�������ڿ��У�Ҳ������ʵ��4������ûӰ�죩
                    all_update_Router --;    //�����������
                    Packages_ULimit[Packages_Count].permission_to_update = 0;
                    Mission_completed_packages ++;

                    printf("\n\n��վ��ʶ��������ʱ�̣�%d ��%d",Packages_ULimit[Packages_Count].owner+1,(int)Current_Time);
                    mission_completed_time = Current_Time;


             }
             else
             {
                    for(int boss_now = Packages_ULimit[Packages_Count].owner; boss_now >= 0; )
                    {
                        /**�Ҹ��°���Ҫ�µ�������İ��������˲������أ�����С���¼���С��ʱ��Ϳ��Ը���**/
                        //�����ǰ���ľ�����½���ʱ�� < ��ǰ·�����洢����������ʱ�䣬�͸��ǵ�
                        if(Packages_ULimit[Packages_Count].Update_time - duration_each_package < Router_tree[boss_now].mail_time_2_update_end)
                        {
                            Router_tree[boss_now].mail_time_2_update_end = Packages_ULimit[Packages_Count].Update_time - duration_each_package;
                        }

                        //���Ҫ��������Ҫ�����ϼ���ǰ��Ҳ�жϹ��ϼ���û���ڸ��µ��ˣ����û�оͿ������ɸ���
                        boss_now = Router_tree[boss_now].boss;  //��boss_now���ڵ�ǰ·��������һ����boss��
                    }

             }
        }
    }
}

            //stop = clock();
            //duration = (stop-start)/CLK_TCK;
