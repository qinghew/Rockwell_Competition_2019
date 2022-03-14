#include "headfiles.h"

/**·��������**/
int Router_Number_N = 0;    //·��������
int Single_Wire_M = 0;      //�����������
int Download_ULimit_P = 0;  //ͬһ֧·ͬʱ����������·��������P
int Update_ULimit_Q = 0;    //ͬʱ���µ�վ����Q
int level_now = 2,level_total = 0;
int Router_count = 0;   //��ǰ�ǵڼ���·����
struct Router Router_tree[500]; //����·�������飬һ��Router_Number_N��·����


/**֧·����**/
int branch_number = 0;   //֧·����
struct branch tree_branch[500];


/**��װ������**/
int Packages_Sum = 0,Packages_Count = 0;   //ʵ�ʰ�������
struct Package Packages_ULimit[5000] = {0};    //�����500*10����
int min_load_time = 500,max_load_time = 0;
int min_update_time = 500,max_update_time = 0;

//    Router_all = Router_tree;


void Input_Init()
{
    printf("Welcome to Rockwell!!!\n\n");
    printf("����������ʱ���� Ӣ�Ķ��� ����������һ������밴�س���\n\n");

    printf("·������N�����������M��ͬһ֧·ͬʱ����������·��������P��ͬʱ���µ�վ����Q:");
    scanf("%d,%d,%d,%d",&Router_Number_N,&Single_Wire_M,&Download_ULimit_P,&Update_ULimit_Q);
    printf("\n��������·������ %d ����������� %d ��ͬһ֧·ͬʱ����������·�������� %d ��ͬʱ���µ�վ���� %d :\n",Router_Number_N,Single_Wire_M,Download_ULimit_P,Update_ULimit_Q);

    //struct Router Router_tree[Router_Number_N]; //����·�������飬һ��Router_Number_N��·����
    //Router_treeֻ������Ӻ����������

    //��ʼ������·������һЩ����
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


    Router_tree[0].level = 1;   //��һ���ļ���һ������߼� 1
    Router_tree[0].boss = -1;       //��1����boss��-1
    Router_count = 1;       //��һ��¼�ˣ�ֱ�Ӵӵڶ�����ʼ
    int head = 1,tail = 0;  //�����������ʼ����ֹ
    int last_head = 1;//last_tail = 0;    //�ϴ��������ʼ����ֹ

    for(int line_count = 1; line_count<=Single_Wire_M; line_count ++,Router_count ++)
    {  /*
        Router_count����ڼ���·�������ӵڶ�(1)��·������ʼ����
        level_now����ǰ�㼶���ӵڶ�����ʼ����
        line_count����ڼ�������
        */
        printf("\n·������·�����ĵ����:");

        scanf("%d,%d",&head,&tail);        //���뱾�ε�ͷβ

        if(Router_tree[head-1].level == Router_tree[last_head-1].level)
        {  /*
            ע�⣺
            1.�����ʼ��·�������ϴ���ʼ��·����������ȣ�˵����ǰ·����û����һ��
            2.������л�����һ�����ʱ��������ͬһ�������л�
            */
            Router_tree[Router_count].level = level_now;
        }
        else
        {
            level_now ++;
            Router_tree[Router_count].level = level_now;
        }

        //��¼��·�������ϼ���˭�����Ӱ�쵱ǰ·�����ܲ����� ��������һ��֧·�ϵ�·���� ͬʱ����
        Router_tree[Router_count].boss = head - 1;

        printf("�� %d ��·�������ڵ� %d ��:\n\n",Router_count+1,Router_tree[Router_count].level);
        last_head = head;   //�����ϴε�ֵ
        //last_tail = tail;
    }

    level_total = level_now;    //��¼һ������

    for(level_now = 1; level_now<=level_total; level_now++)    //��ʾ���м�������·�������
    {

        printf("\n\n�� %d ��:",level_now);
        for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
        {
            if(Router_tree[Router_count].level == level_now)
            {
                printf(" %d ",Router_count + 1);
            }
        }
    }






    printf("\n\n������������\n·������Ϣ:��ţ�����������������������������ʱ�����º�ʱ......\n");

    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        for(int i = 0; i<22; i++)       //�ȸ��ṹ�������Ϣ�����ʼ�����㣬�ٽ�������
        {
            Router_tree[Router_count].information[i] = 0;
        }

        printf("\n�� %d ��·������Ϣ:",Router_count+1);
        /*�����м�����Ҫ���룬ֻҪ���갴�»س�����������һ��Ҫ�����·������Ϣ����*/
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

        if(Router_tree[Router_count].information[0] % 5 == 0)   //��Ŵ�1��ʼ
        {
            Router_tree[Router_count].is_or_not_5N = 1;
        }
        else
        {
            Router_tree[Router_count].is_or_not_5N = 0;
        }

        //ÿ������ĵ�һ��������������·������Ϣ�ĵ�һ����
        Router_tree[Router_count].output[0] = Router_tree[Router_count].information[0];
        Router_tree[Router_count].output_index ++;  //�±�����Ųһλ
    }


    //��ʾ�������Ϣ
    for(Router_count = 0; Router_count<Router_Number_N; Router_count++)
    {
        printf("\n\n�� %d ��·������Ϣ:",Router_count+1);

        for(int i = 0; i<Router_tree[Router_count].information[1]*2 + 2; i++)
        {               //Router_tree[Router_count].information[1]�����м�����װ��
            printf(" %d ",Router_tree[Router_count].information[i]);
        }

        /*
            �����а�װ�������������ʱ�䡢��������ʱ�䣬
            �����ݹ�һ������
        */
        for(int i = 0; i<Router_tree[Router_count].information[1]; i++)
        {
            if(Router_tree[Router_count].information[2 + i*2] < min_load_time)
            {   //�������ʱ��
                min_load_time = Router_tree[Router_count].information[2 + i*2];
            }

            if(Router_tree[Router_count].information[2 + i*2 + 1] < min_update_time)
            {   //��̸���ʱ��
                min_update_time = Router_tree[Router_count].information[2 + i*2 + 1];
            }

            if(Router_tree[Router_count].information[2 + i*2] > max_load_time)
            {   //�����ʱ��
                max_load_time = Router_tree[Router_count].information[2 + i*2];
            }

            if(Router_tree[Router_count].information[2 + i*2 + 1] > max_update_time)
            {   //�����ʱ��
                max_update_time = Router_tree[Router_count].information[2 + i*2 + 1];
            }

        }
        Packages_Sum += Router_tree[Router_count].information[1];       //������°�������

    }

    printf("\n\nmin_load_time = %d\n",min_load_time);
    printf("\nmin_update_time = %d\n",min_update_time);
    printf("\nmax_load_time = %d\n",max_load_time);
    printf("\nmax_update_time = %d\n",max_update_time);


    /**¼���֧��Ϣ**/
    for(int i = 0;i<500;i++)
    {
        tree_branch[i].members_number = 0;     //�洢��֧·���ڽ�����������·������Ŀ
        tree_branch[i].Line_Load_now = 0;
    }

    Router_count = Router_Number_N-1;
    for(int i=0; Router_count>=0; Router_count--)
    {   //�����ǰ·����û��¼�����֧����ֻ¼��������һ�е�·����
        if(Router_tree[Router_count].branch == 100)
        {
            for(int boss_now = Router_count,j = 0; boss_now >= 0; j++)       //ѭ��ÿ�����Լ�������boss���൱��ѭ����һ��֧·
            {                               //j�����i����֧�еĵ�j��·����
                Router_tree[boss_now].branch = i;
                tree_branch[i].member[j] = boss_now;       //i��������֧��j�����֧�е�Ԫ�أ���ŵ���·���������
                tree_branch[i].members_number ++;

                //printf("\n\n��%d����֧�ĵ�%d����Ա%d",i,j,tree_branch[i].member[j]);

                boss_now = Router_tree[boss_now].boss;
            }

            i++;        //i�����֧�ţ���֧˳����ң�0������

        }
        branch_number = i;  //���һ�δ��i���Ƿ�֧����������
    }

    //ѭ��ÿ��·������������ֱϵ�ϼ��㱨�Լ��İ�װ������
    for(Router_count = Router_Number_N-1; Router_count>=0; Router_count--)
    {   //�����ǰ·����û��¼�����֧����ֻ¼��������һ�е�·����

            for(int boss_now = Router_count; boss_now >= 0;)       //ѭ��ÿ�����Լ�������boss���൱��ѭ����һ��֧·
            {                               //j�����i����֧�еĵ�j��·����
                //printf("\n\n��%d����֧�ĵ�%d����Ա%d",i,j,tree_branch[i].member[j]);
                //���ϴ���Լ��ж��ٰ��㱨��ȥ
                if(boss_now != Router_count)    //����Լ�
                {
                    Router_tree[boss_now].sum_son_packages += Router_tree[Router_count].information[1];
                }

                boss_now = Router_tree[boss_now].boss;
            }

    }







    /*
        *�����ڲ�η����������ַ����϶���������ʽ�㷨Ч���ã�����ʽ�㷨��������п�������ѡ���Եر���һ�飬
        *������̫�࣬�������۵����ۼ�һЩ......
        *����Ȩ�ظ�����·��������������۷���
        *Ȩ��ԭ��
            1.level����levelԽ�ߣ�Խ�ٸ��£���Ϊ���ߵĿ�ʼ���»ᵢ�󼶵͵����ء�����  ռ0.5
            2.����ʱ�䣺����ʱ��Խ��Խ�����ϼ�����                                    ռ0.2
            3.����ʱ�䣺����ʱ���ο��Ը��£�������ֻ�ܸɵȴ��������׵����¶�          ռ0.3
            ���θ��µ�ʱ�򣬽�������һ����±ȽϺ�

        *���裺
            1. 3�����ݷֱ��һ��
            2. �����·�����ĸ������ķ���
    */

    /*��һ���������°�����*/
//Packages_Sum
    //Packages_ULimit[]

    for(Packages_Count = 0,Router_count = 0; Router_count<Router_Number_N; Router_count++)     //ÿ��·������ѭ��
    {
        for(int i = 0; i<Router_tree[Router_count].information[1]; i++,Packages_Count++)     //ÿ��·������ÿ������ѭ��һ��
        {
            Packages_ULimit[Packages_Count].owner = Router_count;   //��¼���°������ĸ�·����

            Packages_ULimit[Packages_Count].Load_time = Router_tree[Router_count].information[2 + i*2];     //��¼���غ�ʱ
            Packages_ULimit[Packages_Count].Update_time = Router_tree[Router_count].information[2 + i*2 + 1];   //��¼���º�ʱ
            //��һ��������������Ȩ�� =  �ó�����


            Packages_ULimit[Packages_Count].score = 0.8 * (Router_tree[Router_count].level - 1)*1.0 / (level_total - 1)   //Խ��Խ�ã�����أ��������Ǹ���أ������� ��1-��
                                                    + 0.1 * (1 - (Router_tree[Router_count].information[2 + i*2] - min_load_time)*1.0 / (max_load_time - min_load_time))
                                                     + 0.1 * (1 - (Router_tree[Router_count].information[2 + i*2 + 1] - min_update_time)*1.0 / (max_update_time - min_update_time));


            printf("\n��%d������ score = %f\n",Packages_Count,Packages_ULimit[Packages_Count].score);
        }

    }


    /*ð�ݷ����򣬽� Packages_ULimit �����еķ����Ӵ�С���г���*/
    float temp = 0;
    for (int i = 0; i < Packages_Sum - 1; i++)
    {
            for (int j = i + 1; j < Packages_Sum; j++)
            {
                if (Packages_ULimit[i].score < Packages_ULimit[j].score)
                { // ����������λ��        //���ݷ������°���������Ҳ������������
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

        printf("\n��%d��·�����İ��� ���� = %f\n",Packages_ULimit[i].owner,Packages_ULimit[i].score);
    }

    /*��ʼ�����*/



}
