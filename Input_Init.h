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

/*��ʼ��ʱҪô�г�ֵ���ǣ�Ҫô���ڳ�ʼ��ʱ������*/
struct Router    //ÿ��·���������Լ��Ĳ㼶����װ����������ǰ״̬
{
    int level;

    /*
        condition =
        0 �������״̬�����а���û�����ػ�װ�������е��Ѿ����غ��ˣ���
        1 �����а��������أ�
        2 �����а����ڸ��£�3�����а����غ����ڵȴ�����
    */
    int condition;

    //���ڵ���·������������������������10������ˣ�����һ��2+10*2 = 22��С��������·������Ϣ
    int information[22];

    /*
        boss����:
        �ϼ���˭����һ��·������������ָ��0�������ϼ����Լ���
        �ڶ���·������������ָ��1�������ϼ��ǵ�0��·����(���)
    */
    int boss;


    /*
        �ʼ���mail = 1�������ʱ�¼����������ذ�������·�������ܸ���
        mail = 0�������ʱ�¼�û�������ذ��ģ�����·�������Ը���
    */
    int mail;

    int mail_time_2_update_end; //���ڸ��µ��¼������ģ�������½�����ʱ��

    int branch;     //��Ϊ�߲㶼ռ�ܶ��֧���������ֻ��Ϊ�˴���һ�£��������֧����tree_branch����

    int output[21];     //�������
    int output_index;   //���������±�
    int sum_son_packages;   //�·�·�����İ�װ������

    int is_or_not_5N;
};


struct branch       //��֧˳����ң�0������
{
    int member[500];
    int members_number; //��Ա��
    int Line_Load_now;
};



/*��ʼ��ʱҪô�г�ֵ���ǣ�Ҫô���ڳ�ʼ��ʱ������*/
struct Package
{
    int owner;                  //���°������ĸ�·����(���)
    int Load_time;              //���غ�ʱ
    int Update_time;            //���º�ʱ
    float score;                //���°��ĵ÷�


    int condition;              //0����û����Ҳû���£�1�����������أ�2����������ɵȸ��£�3�������ڸ��£�4����������
    int permission_to_load;     //�������ر�־λ
    int permission_to_update;   //������±�־λ
    double start_load_time;     //��ʼ����ʱ��
    double start_update_time;   //��ʼ����ʱ��


};


extern struct Router Router_tree[500];
extern struct Package Packages_ULimit[5000];    //�����500*10����
extern struct branch tree_branch[500];
#endif
