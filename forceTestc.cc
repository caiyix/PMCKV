// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <time.h>
#include <vector>

#include "leveldb/db.h"
#include "leveldb/write_batch.h"
using namespace std;

int main() {
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  //options.block_size = 32 * 1024;
  cout<<"DBInfo:block_size=="<<options.block_size<<endl;
  leveldb::Status s = leveldb::DB::Open(options, "/home/zjn/桌面/app/my_leveldb_test", &db);
  if (!s.ok()) cerr << s.ToString() << endl;
  assert(s.ok());
  
  ofstream outfile,outfile1;
  ifstream infile,infile1,infile2,infile3;
  double dur,dur1;
  double second,second1;
  clock_t start,end, start1,end1;
   
  infile.open("/home/zjn/桌面/app/DBtest/load10M/value.txt");
  string key,value,op;
  getline(infile,value);
  //cout<<value<<endl;
  int txtRows = 10485760;
  //run操作
  infile2.open("/home/zjn/桌面/app/DBtest/load10M/write/100read-runkey.txt");
  infile3.open("/home/zjn/桌面/app/DBtest/load10M/write/100read-runop.txt");
  outfile1.open("/home/zjn/桌面/app/leveldb_test/100read-run.txt",ios::app);
  start = clock();
  start1 = clock();
  for(int i=0;i<txtRows;i++){//txtRows	
        infile2>>key;
        infile3>>op;
        s = db->Get(leveldb::ReadOptions(), key, &value);
        if(op == "READ"){
        	//cout<<key<<"---READ---"<<op<<endl;
        	s = db->Get(leveldb::ReadOptions(), key, &value);
        }else if(op == "UPDATE"){
        	//cout<<key<<"---INSERT||UPDATE---"<<op<<endl;
        	s = db->Put(leveldb::WriteOptions(), key, value);
        }else{
        	cout<<"-------op error-------"<<endl;
        }
        if(i != 0 && (i+1)%1048576 == 0){//if(i != 0 && i%100000 == 0){
        	end1 = clock();
  		dur1 = (double)(end1-start1);
  		second1 = dur1/CLOCKS_PER_SEC;
  		cout<<(i+1)/1048576<<":start1--"<<start1<<",end1--"<<end1<<",dur1--"<<dur1<<",second1--"<<second1<<endl;
		outfile1<<start1;
		outfile1<<"  ";
		outfile1<<end1;
		outfile1<<"  ";
		outfile1<<dur1;
		outfile1<<"  ";
		outfile1<<second1;
		outfile1<<"\n";
		start1 = clock();
        }
  }
  end = clock();
  dur = (double)(end-start);
  second = dur/CLOCKS_PER_SEC;
  outfile1<<"\n----------------run op end-1-----------------\n";
  outfile1<<start;
  outfile1<<"  ";
  outfile1<<end;
  outfile1<<"  ";
  outfile1<<dur;
  outfile1<<"  ";
  outfile1<<second;
  outfile1<<"  ";
  outfile1<<CLOCKS_PER_SEC;
  outfile1<<"\n----------------run op end-2-----------------\n";
  outfile1.close();/**/
  return 0;
}
