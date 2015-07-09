/*
** 互斥锁——多线程多任务互斥访问资源
** 信号量——多线程多任务同步操作
*/
#include<iostream>
#include<pthread.h>

using namespace std;
pthread_mutex_t sum_mutex; //互斥锁
pthread_cond_t tasks_cond;//信号量

int sum=10;  //全局变量

//测试线程互斥操作
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
//测试线程同步操作
void* say_hello1(void *args)
{
	//当sum>5，线程1运行
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
			pthread_cond_signal(&tasks_cond);//发送一个信号给另外一个正在处于阻塞等待状态的线程,使其脱离阻塞状态,继续执行

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
			/*调用pthread_cond_wait前要先锁互斥量,
			**pthread_cond_wait在把线程放进阻塞队列后，自动对mutex进行解锁(它内部有自己维护一个队列)，
			**使得其他线程可以获得加锁的权利。这样其它线程才能对临界资源进行访问并在适当的时候唤醒这个阻塞的进程。
			**当pthread_cond_wait返回的时候又会自动给mutex加锁，所以最后需要一个pthread_mutex_unlock()函数来解锁
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
	//线程id
	//pthread_t tid1,tid2;
    //对互斥锁进行初始化
	pthread_mutex_init(&sum_mutex,NULL); 
	//对信号量初始化
	pthread_cond_init(&tasks_cond,NULL);

    //测试线程互斥
	/*int th1=1,th2=2;
	//线程id，线程属性，线程函数，线程函数参数
	int ret1=pthread_create(&tid1,NULL,say_hello,(void *)&th1);
	if(ret1!=0)
		cout<<"pthread_create 1 error="<<ret1<<endl;

	int ret2=pthread_create(&tid2,NULL,say_hello,(void*)&th2);
	if(ret2!=0)
		cout<<"pthread_create 2 error="<<ret2<<endl;*/


	//测试线程同步操作
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
	//主线程会一直等待直到等待的线程结束,自己才结束，使创建的线程有机会执行
	//pthread_join(tid1,&status); 
	//pthread_join(tid2,&status);
	pthread_join(tid3,&status);
	pthread_join(tid4,&status);
	pthread_mutex_destroy(&sum_mutex); //销毁互斥锁
	pthread_cond_destroy(&tasks_cond); //销毁信号量
	getchar();
	return 0;
}
