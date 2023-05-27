/*!
 *@file Value.h
 *@brief Value类接口头文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#ifndef SYSYC_VALUE_H
#define SYSYC_VALUE_H

#include <iostream>
#include <list>
#include <string>

class Type;
class Value;

/*! use结构体，作为中间IR的基础*/
struct Use {
  Value *val_;      // 使用value的value
  unsigned arg_no_; // the no. of operand, e.g., func(a, b), a is 0, b is 1
  Use(Value *val, unsigned no) : val_(val), arg_no_(no) {} // 构造函数

  /*!
   *@brief 判定两个use是否相等
   *@param 待比较的use-1
   *@param 待比较的use-2
   *@return 比对结果
   */
  friend bool operator==(const Use &lhs, const Use &rhs) { //
    return lhs.val_ == rhs.val_ && lhs.arg_no_ == rhs.arg_no_;
  }
};

/*! value类，作为中间IR的基础*/
class Value {
private:
protected:
  Type *type_;
  std::list<Use> use_list_; // 使用value的value list
  std::string name_;        // value名称

public:
  /*!
   *@brief Value的构造函数
   *@param ty 类型
   *@param name value名称
   *@return 当前对象本身
   */
  explicit Value(Type *ty, const std::string &name = "");
  /*!
   *@brief Value的析构函数
   */
  ~Value() = default;

  /*!
   *@brief 获取value的类型
   *@return value 类型常量指针
   */
  Type *get_type() const { return type_; }

  /*!
   *@brief 获取使用该value的use list
   *@return 返回use-list的引用
   */
  std::list<Use> &get_use_list() { return use_list_; }

  /*!
   *@brief 添加use
   *@param val 使用该value的value
   *@param arg_no 在指令中的顺序
   */
  void add_use(Value *val, unsigned arg_no = 0);

  /*!
   *@brief 对于value设置名称
   *@param name value名称
   *@return 名称设置的布尔结果
   *@note
   *---------
   *名字为空即设置新名字，设置后不再进行修改
   */
  bool set_name(std::string name) {
    if (name_ == "") {
      name_ = name;
      return true;
    }
    return false;
  }

  /*!
   *@brief 获取value的名称
   *@return value字符串常量
   */
  std::string get_name() const;

  /*!
   *@brief 替换所有对于旧value的引用，改为新的
   *@param new_val value型指针
   *@note
   *--------
   *支持对于所有的value的修改，包括基本块
   */
  void replace_all_use_with(Value *new_val);

  /*!
   *@brief 替换所有对于旧value的引用，改为新的
   *@param val value型指针
   *@note
   *----------
   *支持对于指令级别value的替换
   */
  void remove_use(Value *val);

  /*!
   *@brief value的打印
   *@return 默认为空
   *@note
   *--------
   *后期根据不同类型的value进行修改，作为一个虚函数出现
   */
  virtual std::string print() { return ""; }
};

#endif // SYSYC_VALUE_H
