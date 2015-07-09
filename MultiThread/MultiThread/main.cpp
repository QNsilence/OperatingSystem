/*
** �������������̶߳����񻥳������Դ
** �ź����������̶߳�����ͬ������
*/
#include<iostream>
#include<pthread.h>

using namespace std;
pthread_mutex_t sum_mutex; //������
pthread_cond_t tasks_cond;//�ź���

int sum=10;  //ȫ�ֱ���

//�����̻߳������
void* say_hello(void *args)
{
	int i=(*(int*)args);
	pthread_mutex_lock(&sum_mutex);
	cout<<"before thread,sum="<<sum<<endl;
	sum=sum+i;
	cout<<"after thread,sum="<<sum<<endl;
	pthread_mutex_unlock(&sum_mutex);
	pthread_exit(0);
	return 0;
}
//�����߳�ͬ������
void* say_hello1(void *args)
{
	//��sum>5���߳�1����
	pthread_t pid1=pthread_self();
	while(true)
	{
		if(sum>5)
		{
			cout<<pid1.p<<" thread id:"<<sum<<endl;
			sum--;
		}
		else
		{
			pthread_cond_signal(&tasks_cond);//����һ���źŸ�����һ�����ڴ��������ȴ�״̬���߳�,ʹ����������״̬,����ִ��

		}
		if(0==sum)
			break;
	}
	
	return 0;
}
void* say_hello2(void *args)
{
	pthread_t pid2=pthread_self();
	while(true)
	{
		pthread_mutex_lock(&sum_mutex);
		if(sum>5)
		{
			pthread_cond_wait(&tasks_cond,&sum_mutex);
			/*����pthread_cond_waitǰҪ����������,
			**pthread_cond_wait�ڰ��̷߳Ž��������к��Զ���mutex���н���(���ڲ����Լ�ά��һ������)��
			**ʹ�������߳̿��Ի�ü�����Ȩ�������������̲߳��ܶ��ٽ���Դ���з��ʲ����ʵ���ʱ������������Ľ��̡�
			**��pthread_cond_wait���ص�ʱ���ֻ��Զ���mutex���������������Ҫһ��pthread_mutex_unlock()����������
			*/
		}
		else
		{
			cout<<pid2.p<<" thread id:"<<sum<<endl;
			sum--;
		}
		pthread_mutex_unlock(&sum_mutex);
		if(0==sum)
			break;
	}
	return 0;
}
int main()
{
	//�߳�id
	//pthread_t tid1,tid2;
    //�Ի��������г�ʼ��
	pthread_mutex_init(&sum_mutex,NULL); 
	//���ź�����ʼ��
	pthread_cond_init(&tasks_cond,NULL);

    //�����̻߳���
	/*int th1=1,th2=2;
	//�߳�id���߳����ԣ��̺߳������̺߳�������
	int ret1=pthread_create(&tid1,NULL,say_hello,(void *)&th1);
	if(ret1!=0)
		cout<<"pthread_create 1 error="<<ret1<<endl;

	int ret2=pthread_create(&tid2,NULL,say_hello,(void*)&th2);
	if(ret2!=0)
		cout<<"pthread_create 2 error="<<ret2<<endl;*/


	//�����߳�ͬ������
	pthread_t tid3,tid4;
	int ret3=pthread_create(&tid3,NULL,say_hello1,NULL);
	cout<<"ret3="<<ret3<<endl;
	if(ret3!=0)
		cout<<"thread_create 3 error="<<ret3<<endl;

	int ret4=pthread_create(&tid4,NULL,say_hello2,NULL);
	cout<<"ret4="<<ret4<<endl;
	if(ret4!=0)
		cout<<"thread_create 4 error"<<ret4<<endl;


	void* status;
	//���̻߳�һֱ�ȴ�ֱ���ȴ����߳̽���,�Լ��Ž�����ʹ�������߳��л���ִ��
	//pthread_join(tid1,&status); 
	//pthread_join(tid2,&status);
	pthread_join(tid3,&status);
	pthread_join(tid4,&status);
	pthread_mutex_destroy(&sum_mutex); //���ٻ�����
	pthread_cond_destroy(&tasks_cond); //�����ź���
	getchar();
	return 0;
}
