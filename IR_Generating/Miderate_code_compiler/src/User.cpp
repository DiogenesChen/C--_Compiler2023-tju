/*!
 *@file User.h
 *@brief 用户类接口定义文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#include "User.h"
#include <cassert>

/*!
 *@brief User的构造函数
 *@param ty 类型
 *@param name User名称
 *@param num_ops value的位置
 *@return 当前对象本身
 *@note
 *---------
 *初始化operands数组全为nullptr
 */
User::User(Type *ty, const std::string &name, unsigned num_ops)
    : Value(ty, name), num_ops_(num_ops) {
  operands_.resize(num_ops_, nullptr);
}

/*!
 *@brief 获得包含value指针的数组
 *@return 返回User维护的Value数组
 */
std::vector<Value *> &User::get_operands() { return operands_; }

/*!
 *@brief 获得数组中的第i个value数值指针
 *@return 获得数组中的第i个value数值常量指针
 */
Value *User::get_operand(unsigned i) const { return operands_[i]; }

/*!
 *@brief 设置数组中的第i个value数值指针
 *@note
 *--------
 *设置数组中的第i个value数值常量指针
 *设置界限检查，查看索引i是否超限
 *--------
 *&emsp; value数组尾插入一个value
 *&emsp; 为value添加一个use关系
 *&emsp; 计数加一
 */
void User::set_operand(unsigned i, Value *v) {
  assert(i < num_ops_ && "set_operand out of index");
  // assert(operands_[i] == nullptr && "ith operand is not null");
  operands_[i] = v;
  v->add_use(this, i);
}

/*!
 *@brief 添加新的value数值指针
 *@param v value数值指针
 *@note
 *--------
 *&emsp; value数组尾插入一个value
 *&emsp; 为value添加一个use关系
 *&emsp; 计数加一
 */
void User::add_operand(Value *v) {
  operands_.push_back(v);
  v->add_use(this, num_ops_);
  num_ops_++;
}

/*!
 *@brief 获取User维护的operand数量
 *@return operand常量数值
 *@note
 *-------
 *判断
 */
unsigned User::get_num_operand() const { return num_ops_; }

/*!
 *@brief 添加新的value数值指针
 *@param v value数值指针
 *@note
 *--------
 *遍历operands链表，对于每个value量的use_list进行检查
 *在operands表中删除对于本对象的使用
 */
void User::remove_use_of_ops() {
  for (auto op : operands_) {
    op->remove_use(this);
  }
}

/*!
 *@brief 删除指定范围的operands
 *@param index1 索引1
 *@param index2 索引2
 *@note
 *--------
 *遍历operands表索引范围，删除对于本user的使用
 *删除本表的相关operands
 *修改operands_size
 */
void User::remove_operands(int index1, int index2) {
  for (int i = index1; i <= index2; i++) {
    operands_[i]->remove_use(this);
  }
  operands_.erase(operands_.begin() + index1, operands_.begin() + index2 + 1);
  num_ops_ = operands_.size();
}
