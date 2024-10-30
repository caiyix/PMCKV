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
  ifstream infile,infile1,infile2;
  double dur,dur1;
  double second,second1;
  clock_t start,end, start1,end1;
   
  infile.open("/home/zjn/桌面/app/DBtest/load10M/value.txt");
  string key,value;
  getline(infile,value);
  //cout<<value<<endl;
  
  //load操作
  infile1.open("/home/zjn/桌面/app/DBtest/load10M/loadkey.txt");
  //int txtRows = 10485760;
  //int txtRows = 20971520;
  int txtRows = 31457280;
  //int txtRows = 41943040;
  start = clock();
  start1 = clock();
  outfile.open("/home/zjn/桌面/app/my_leveldb_test/loadres.txt",ios::app);
  //outfile1.open("/home/zjn/桌面/app/my_leveldb_test/loadres_p.txt",ios::app);
  for(int i=0;i<txtRows;i++){//txtRows
        infile1>>key;
        //cout<<key<<endl;
        double dur_p,second_p;
  	clock_t start_p,end_p;
  	start_p = clock();
        s = db->Put(leveldb::WriteOptions(), key, value);
        end_p = clock();
        dur_p = (double)(end_p-start_p);
  	//second_p = dur_p/CLOCKS_PER_SEC;
        outfile1<<dur_p;
	outfile1<<"\n";
        assert(s.ok());
        if(i != 0 && (i+1)%1048576 == 0){//if(i != 0 && i%1048576 == 0){
        	end1 = clock();
  		dur1 = (double)(end1-start1);
  		second1 = dur1/CLOCKS_PER_SEC;
  		cout<<(i+1)/1048576<<":start1--"<<start1<<",end1--"<<end1<<",dur1--"<<dur1<<",second1--"<<second1<<endl;
		outfile<<start1;
		outfile<<"  ";
		outfile<<end1;
		outfile<<"  ";
		outfile<<dur1;
		outfile<<"  ";
		outfile<<second1;
		outfile<<"\n";
		start1 = clock();
		//outfile.close();
        }
  }
  end = clock();
  dur = (double)(end-start);
  second = dur/CLOCKS_PER_SEC;
  outfile<<"\n----------------put op end-1-----------------\n";
  outfile<<start;
  outfile<<"  ";
  outfile<<end;
  outfile<<"  ";
  outfile<<dur;
  outfile<<"  ";
  outfile<<second;
  outfile<<"  ";
  outfile<<CLOCKS_PER_SEC;
  outfile<<"\n----------------put op end-2-----------------\n";
  outfile.close();

  return 0;
}
