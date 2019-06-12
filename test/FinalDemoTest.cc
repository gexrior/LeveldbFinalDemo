//
// Created by rui on 19-6-12.
//

#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/env.h"
#include "leveldb/table.h"
#include "db/filename.h"
#include "db/version_set.h"
#include "db/version_edit.h"
#include "db/dbformat.h"

using namespace std;
using namespace leveldb;

string Trim(string& str)
{
  str.erase(0,str.find_first_not_of(" \t\r\n"));
  str.erase(str.find_last_not_of(" \t\r\n") + 1);
  return str;
}


int main() {
  // 1.initialize
  // 实现mycomparator

  // comparator.h参考BytewiseComparatorImpl实现可以直接比较大小的mycomparator
  //  修改option.cc内容,option.h实现，参考“options.create_if_missing = true;”
  //  options.comparator = mycomparator;

  // open a db
  leveldb::DB *db;
  leveldb::Options options;
//options.comparator = myCom;
  options.create_if_missing = true;
  std::string dbname_ = "/home/rui/CLionProjects/test_read_sst/cmake-build-debug/mydb123";
  leveldb::Status status2 = leveldb::DB::Open(options, dbname_, &db);


  // put datas into sstables
  // getline();

//  ifstream fin("../test/TestData.csv");
//  string line;
//  while (getline(fin, line))
//  {
//
//    istringstream sin(line);
//    vector<string> fields;
//    string field;
//    while (getline(sin, field, ','))
//    {
//      fields.push_back(field);
//    }
//    string key = Trim(fields[0]);
//    string value = Trim(fields[1]);
//    //status2 =db2->Put(leveldb::WriteOptions(),key,value);
//    assert(status2.ok());
//  }

  // 找到每个ssts的最大最小值-通过最大最小值来减少ssts
  // find level min-max
  // findMinMax（files_[i]->smallest/largest）;
  // files_[i]->smallest;
  // files_[i]->largeset;

//  leveldb::Version *current = db2->GetCurrentVersion();
//  std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(2);
//  for (int i = 0; i < files_.size(); i++) {
//    std::cout << "number: " << files_[i]->number << " ";
//    std::cout << "id_min: " << files_[i]->smallest.user_key().ToString() << " ";
//    std::cout << "id_max: " << files_[i]->largest.user_key().ToString() << std::endl;
//  }


  // 2.主键过滤优化
  // 判断给定的id＿range想法是否在每个sst的min-max区间，找到剩下的sst
  // 返回满足条件的sst集合id＿ssts
  // return id＿ssts

  //if(min < id＿range &&id＿range < max){
  //  return id＿ssts;
  //}


  // 3.非主键过滤优化-通过filter判断sst里是否存在该mail来减少ssts
  // 给定的mail判断是否在id＿ssts
  // 返回满足条件的sst集合id＿ssts
  // return id＿ssts

  //   r->filter_block->AddKey(key); 改成　r->filter_block->AddKey(mail);
  //   if（filter（mail)){}

  // 读入ssts步骤
  leveldb::Version *current = db->GetCurrentVersion();
  std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(0);

  std::string fname = leveldb::TableFileName(dbname_, files_[0]->number);

  leveldb::RandomAccessFile *file = nullptr;
  leveldb::Table *table = nullptr;
  leveldb::Env *env_ = options.env;
  env_->NewRandomAccessFile(fname, &file);
  leveldb::Table::Open(options, file, files_[0]->file_size, &table);

  leveldb::Iterator *result = table->NewIterator(leveldb::ReadOptions());

  result->SeekToFirst();
  while (result->Valid()) {
    leveldb::ParsedInternalKey ikey;
    leveldb::ParseInternalKey(result->key(), &ikey);
    std::cout << "key:" << ikey.user_key.ToString();
    std::cout << "   value:" << result->value().ToString() << std::endl;
    result->Next();
  }

  //　4.将过滤后的id_sst,传入multimap
  std::multimap<std::string,std::string> mmap;
  //iterator
  leveldb::Iterator *iter=db->NewIterator(leveldb::ReadOptions());
  mmap.insert(std::make_pair(iter->value().ToString(),iter->key().ToString()));
  // find strFind
  std::string strFind = "KOS@ecnu.cn";
  // find（strFind);

  //　5.输出对应的<key1,key2> ＋ mail键值对


  delete iter;
  delete db;
  return 0;

}
