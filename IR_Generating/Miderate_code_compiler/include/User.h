/*!
 *@file User.h
 *@brief 用户类接口头文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#ifndef SYSYC_USER_H
#define SYSYC_USER_H

#include "Value.h"
#include <vector>

/*! user类，中间IR的基础*/
class User : public Value {
private:
protected:
  std::vector<Value *> operands_; // operands of this value
  unsigned num_ops_;              // value值的个数

public:
  /*!
   *@brief User的构造函数
   *@param ty 类型
   *@param name User名称
   *@param num_ops value的位置
   *@return 当前对象本身
   */
  User(Type *ty, const std::string &name = "", unsigned num_ops = 0);

  /*!
   *@brief Value的析构函数
   */
  ~User() = default;

  /*!
   *@brief 获得包含value指针的数组
   *@return 返回User维护的Value数组
   */
  std::vector<Value *> &get_operands();

  /*!
   *@brief 获得数组中的第i个value数值指针
   *@return 获得数组中的第i个value数值常量指针
   */
  Value *get_operand(unsigned i) const;

  /*!
   *@brief 设置数组中的第i个value数值指针
   *@note
   *--------
   *设置数组中的第i个value数值常量指针
   */
  void set_operand(unsigned i, Value *v);

  /*!
   *@brief 添加新的value数值指针
   *@param v value数值指针
   *@note
   *--------
   *&emsp; value数组尾插入一个value
   *&emsp; 为value添加一个use关系
   *&emsp; 计数加一
   */
  void add_operand(Value *v);

  /*!
   *@brief 获取User维护的operand数量
   *@return operand常量数值
   *@note
   *-------
   *判断
   */
  unsigned get_num_operand() const;

  /*!
   *@brief 添加新的value数值指针
   *@param v value数值指针
   *@note
   *--------
   *
   */
  void remove_use_of_ops();

  /*!
   *@brief 添加新的value数值指针
   *@param index1 索引1
   *@param index2 索引2
   *@note
   */
  void remove_operands(int index1, int index2);
};

#endif // SYSYC_USER_H
