#ifndef STORAGE_LEVELDB_DB_THREADTASK_H_
#define STORAGE_LEVELDB_DB_THREADTASK_H_
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>
#include "db/version_edit.h"
#include "message.h"

using namespace std;
namespace leveldb {
	#define process_num 4
	#define task_num 1

	struct fileInfo{
		FileMetaData* fileMetaData;
		int levelNum;             //�ļ������Ĳ㼶
		int fileNum;              //�ļ�������Ϣ����leveldb��FileMetaData�ṹ
		bool modifyFlag = false;  //�ļ��޸ı�־������ָ���������� P3 �Ƿ���Ҫ�ȴ���һ������� P3���
		bool writeBackFlag = true;//�ļ�д�ر�־������ָ���������� P3 ���ɵ��ļ��Ƿ����ֱ���� P4д��
	};
	
	class ThreadTask{
		public:
		void process1(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
			sem_wait(&mes->sem_ts[0]);
			//���ǵ�һ��������Ҫ��������ڻ�ȡ���ļ���Ϣ�Ƿ�ᱻ��һ�������޸�
			if(taskId != 1){
//				ThreadTask* threadTask = taskVector->at(taskVector->size()-1); 
				//if(taskId == 3){
					//cout<<"*****************************************"<<endl;
				//}
				ThreadTask* threadTask = new ThreadTask();
				for(int i=0;i<taskVector->size();i++){
					if(taskVector->at(i)->taskId == (taskId-1)){
						threadTask = taskVector->at(i);
						break;
					}
				}
				if(threadTask->taskId == (taskId-1)){
					int size = threadTask->inputs_[1].size();
					//��һ������--����Ҫ������һ���ļ���������--����Ҫ����һ���ļ�
					int lastFileNum = threadTask->inputs_[1].at(size-1).fileNum;
					int firstFileNum = inputs_[1].at(0).fileNum;
					if(lastFileNum == firstFileNum){//��������֮����ڷ�Χ�ص�
						threadTask->inputs_[1].at(size-1).writeBackFlag = false;
						inputs_[1].at(0).modifyFlag = true; 
					}
				}else{
					cout<<"p1-task"<<taskId<<"don't have pre"<<endl;
				}
				//delete threadTask;
				/**/
			}
			cout<<taskId<<"--p1: get compactin file info ,files are:"<<endl;
			cout<<"level i:";
			for(int i=0;i<inputs_[0].size();i++){
				cout<<inputs_[0].at(i).fileMetaData->number<<",";
			}
			cout<<endl;
			cout<<"level i+1:";
			for(int i=0;i<inputs_[1].size();i++){
				cout<<inputs_[1].at(i).fileMetaData->number<<",";
			} 
			cout<<endl;
			mes->p_finish[mes->messageID] = true;
		}
		void process2(){
			sem_wait(&mes->sem_ts[1]);//���߳��Ƿ�������ִ�б����� 
			cout<<taskId<<"--p2:read files"<<endl;
			cout<<"level i:";
			for(int i=0;i<inputs_[0].size();i++){
				cout<<inputs_[0].at(i).fileMetaData->number<<",";
			}
			cout<<endl;
			cout<<"level i+1:";
			for(int i=0;i<inputs_[1].size();i++){
				cout<<inputs_[1].at(i).fileMetaData->number<<",";
			}
			cout<<endl;
			mes->p_finish[mes->messageID] = true;
		}
		void process3(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
			sem_wait(&mes->sem_ts[2]);
			bool modifyFlag = inputs_[1].at(0).modifyFlag;//�������һ���ļ���modifyFlag
			if(taskId != 1 && modifyFlag){
				ThreadTask* threadTask = taskVector->at(taskVector->size()-1); 
				//���������Ѿ���ӣ���ô��Ҫ��ȡ��һ������ 
				for(int i=0;i<taskVector->size();i++){
					if(taskVector->at(i)->taskId == (taskId-1)){
						threadTask = taskVector->at(i);
						break;
					}
				}	
				if(threadTask->taskId == (taskId-1)){
					sem_wait(&threadTask->mes->sem_p3);//��һ�������P3�Ƿ����
				}else{
					cout<<"p3-1-task"<<taskId<<"don't have pre"<<endl;
				}
			}
			/**/
			vector<fileInfo> newfile0,newfile1,newfile2;
			fileInfo fileInfo9,fileInfo10,fileInfo11,fileInfo12,fileInfo13,fileInfo14;
			fileInfo9.levelNum = 4;
			fileInfo10.levelNum = 4;
			fileInfo11.levelNum = 4;
			fileInfo12.levelNum = 4;
			fileInfo13.levelNum = 4;
			fileInfo14.levelNum = 4;
			fileInfo9.fileNum = 9;
			fileInfo10.fileNum = 10;
			fileInfo11.fileNum = 11;
			fileInfo12.fileNum = 12;
			fileInfo13.fileNum = 13;
			fileInfo14.fileNum = 14;
			newfile0.push_back(fileInfo9);
			newfile0.push_back(fileInfo10);
			newfile1.push_back(fileInfo11);
			newfile1.push_back(fileInfo12);
			newfile2.push_back(fileInfo13);
			newfile2.push_back(fileInfo14);
			if(taskId == 1){
				newfile =  newfile0;
			}else if(taskId == 2){
				newfile =  newfile1;
			}else{
				newfile =  newfile2;
			}
			//���ǵ�һ��������Ҫ��������ڻ�ȡ���ļ���Ϣ�Ƿ���һ�������޸�
			//��������֮����ڷ�Χ�ص�������Ҫ��ȡ��һ����������ļ�---�����������еĵ����ڶ�������
			
			if(taskId != 1 && modifyFlag){
				ThreadTask* threadTask = taskVector->at(taskVector->size()-1); 
				for(int i=0;i<taskVector->size();i++){
					if(taskVector->at(i)->taskId == (taskId-1)){
						threadTask = taskVector->at(i); 
						break;
					}
				}
				if(threadTask->taskId == (taskId-1)){
					//������Ҫ��һ������Ĳ������ļ�����ʱ������һ�������������Ҫ���ļ�д��
					cout<<"****************************"<<threadTask->taskId<<"******************************"<<endl;
					cout<<"threadTask->newfile:"<<threadTask->newfile[0].fileNum<<",don't need"<<endl;
					cout<<"threadTask->newfile:"<<threadTask->newfile[1].fileNum<<",need"<<endl;
					threadTask->newfile[0].writeBackFlag = true; 
					threadTask->newfile[1].writeBackFlag = false; 
					sem_post(&(threadTask->mes->sem_write));
				}else{
					cout<<"p3-2-task"<<taskId<<"don't have pre"<<endl;
				}
					
				
			}
			cout<<taskId<<"--p3:files content marge and sort,new files are"<<endl;
			cout<<"level i+1:";
			for(int i=0;i<newfile.size();i++){
				cout<<newfile[i].fileNum<<",";
			}
			cout<<endl;
			//�ҵ���������ˣ�������һ������ʹ���Ҳ��������ݣ����� sem_p3
			sem_post(&mes->sem_p3);
			mes->p_finish[mes->messageID] = true;
		}
		void process4(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
			sem_wait(&mes->sem_ts[3]);
			bool writeFlag = false;
			ThreadTask* threadTask = taskVector->at(taskVector->size()-1); 
			if(threadTask->taskId == taskId && taskId == task_num){//���������µ����񣬿���ֱ��д�� 
				cout<<"task"<<taskId<<"is the last task"<<endl;
				writeFlag = true; 
			}else{//��Ҫ�����һ������Ա���������� 
			    vector<int>::iterator iter = std::find(threadFinished->begin(), threadFinished->end(), taskId+1);
			    if (iter != threadFinished->end())
			    {
			        int size = inputs_[1].size();
				writeFlag = inputs_[1].at(size-1).writeBackFlag;
			    }else{
			       cout<<"****************************"<<taskId<<"******************************"<<endl;
				while(threadTask->taskId == taskId){//��Ҫ�ȴ���һ���������
					threadTask = taskVector->at(taskVector->size()-1); 
				}
				cout<<"****************************"<<taskId<<"******************************"<<endl;
				while(!threadTask->mes->p_finish[0]){//��Ҫ�ȴ���һ���������P1
				}
				cout<<"****************************"<<taskId<<"******************************"<<endl;
				int size = inputs_[1].size();
				writeFlag = inputs_[1].at(size-1).writeBackFlag;
			    }
			}
			cout<<"task"<<taskId<<"--p4: writeFlag is :"<<writeFlag<<endl;
			while(!writeFlag){
				sem_wait(&mes->sem_write);	
				writeFlag = true;
			} /**/
			cout<<taskId<<"--p4:new files write to disk,write files are:"<<endl;
			for(int i=0;i<newfile.size();i++){
				if(newfile[i].writeBackFlag){
					cout<<newfile[i].fileNum<<",";
				}
			} 
			mes->p_finish[mes->messageID] = true;
		}
		void process(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
		 	process1(taskVector,threadFinished);
		 	process2();
		 	process3(taskVector,threadFinished);
		 	process4(taskVector,threadFinished);
		}
    	
    	bool nextProcess()
		{	
			mes->messageID++;
			cout <<"task"<<taskId<<"--start run P"<< mes->messageID+1<< endl;
			sem_post(&(mes->sem_ts[mes->messageID])); 
			
		}
    	
		thread thread_creat(vector<ThreadTask*>* taskVector,vector<int>* threadFinished)
		{
			thread t;
			t = thread(std::mem_fn(&ThreadTask::process), this,taskVector,threadFinished);
			cout<<"create thread,id----:" << t.get_id() << endl;
			return t;
		}
		
		void thread_process()
		{
			task_thread.join();
			cout<<"process thread,id----:" << task_thread.get_id() << endl;
		}
		
		void getFileMetaData(){
			for(int i=0;i<leveldb_inputs_[0].size();i++){
				cout<<"threadTask--leveldb_inputs_[0]:"<<leveldb_inputs_[0][i]->number<<endl;
				fileInfo info;
				info.fileMetaData = leveldb_inputs_[0][i];
				inputs_[0].push_back(info);
			}
			for(int i=0;i<leveldb_inputs_[1].size();i++){
				cout<<"threadTask--leveldb_inputs_[1]:"<<leveldb_inputs_[1][i]->number<<endl;
				fileInfo info;
				info.fileMetaData = leveldb_inputs_[1][i];
				inputs_[1].push_back(info);
			}
		}
		
		int taskId;                 //����id 
		thread task_thread;         //�������󶨵����߳� 
		Message* mes;               //������Ϣ���������̶߳�����ִ�еĿ��� 
		vector<fileInfo> inputs_[2];//���ļ���i���i+1��
		vector<fileInfo> newfile;   //���ļ�:i+1��
		vector<FileMetaData*> leveldb_inputs_[2]; //���ļ���i���i+1��,���ݼ̳���compaction
	};
}  // namespace leveldb
#endif  // STORAGE_LEVELDB_DB_THREADTASK_H_


