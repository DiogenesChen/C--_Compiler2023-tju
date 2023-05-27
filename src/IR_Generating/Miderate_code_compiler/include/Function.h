/*!
 *@file Function.h
 *@brief 函数接口头文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#ifndef SYSYC_FUNCTION_H
#define SYSYC_FUNCTION_H

#include <cassert>
#include <cstddef>
#include <iterator>
#include <list>
#include <map>

#include "BasicBlock.h"
#include "Module.h"
#include "Type.h"
#include "User.h"

class Module;
class Argument;
class BasicBlock;
class Type;
class FunctionType;

/**
 * @brief 函数
 * @note 管理基本块，从属于模块
 */
class Function : public Value {
public:
  /**
   * @brief Construct a new Function object
   *
   * @param ty 函数类型指针
   * @param name 函数名称
   * @param parent 所属模块
   */
  Function(FunctionType *ty, const std::string &name, Module *parent);
  /**
   * @brief Destroy the Function object
   *
   */
  ~Function();
  /**
   * @brief 创建函数对象
   *
   * @param ty 函数类型
   * @param name 函数名称
   * @param parent 所属模块
   * @return Function* 函数对象指针
   */
  static Function *create(FunctionType *ty, const std::string &name,
                          Module *parent);
  /**
   * @brief Get the function type object，获取函数的返回类型
   *
   * @return FunctionType* 函数类型
   */
  FunctionType *get_function_type() const;
  /**
   * @brief Get the return type object，获取函数返回类型
   *
   * @return Type* 返回返回值类型
   */
  Type *get_return_type() const;
  /**
   * @brief 添加基本块
   *
   * @param bb 基本块指针
   */
  void add_basic_block(BasicBlock *bb);
  /**
   * @brief Get the num of args object，获取函数的参数个数
   *
   * @return unsigned 函数参数的个数
   */
  unsigned get_num_of_args() const;
  /**
   * @brief 获取函数参数数组的迭代器
   *
   * @return std::list<Argument *>::iterator
   * 默认返回函数参数数组的迭代器，指向第一个元素
   */
  std::list<Argument *>::iterator arg_begin() { return arguments_.begin(); }
  /**
   * @brief 获取函数参数数组的迭代器
   *
   * @return std::list<Argument *>::iterator
   * 返回函数参数数组的迭代器，指向最后一个元素
   */
  std::list<Argument *>::iterator arg_end() { return arguments_.end(); }
  /**
   * @brief Get the num basic blocks object，获取函数管理基本块的数量
   *
   * @return unsigned ，函数管理的基本快数量
   */
  unsigned get_num_basic_blocks() const;
  /**
   * @brief Get the parent object，获取函数所属模块
   *
   * @return Module* ，模块指针，默认为本文件
   */
  Module *get_parent() const;
  /**
   * @brief 删除函数内的指定基本块
   *
   * @param bb 基本块指针
   */
  void remove(BasicBlock *bb);
  /**
   * @brief Get the entry block object，获取基本块入口
   *
   * @return BasicBlock* 基本块指针
   *
   * @note 获取管理的基本块链第一个基本块
   */
  BasicBlock *get_entry_block() { return *basic_blocks_.begin(); }
  /**
   * @brief Get the basic blocks object，获取基本块链
   *
   * @return std::list<BasicBlock *>& 基本块链的引用
   */
  std::list<BasicBlock *> &get_basic_blocks() { return basic_blocks_; }
  /**
   * @brief Get the args object，获取参数列表
   *
   * @return std::list<Argument *>& 参数列表引用
   */
  std::list<Argument *> &get_args() { return arguments_; }
  /**
   * @brief 判断函数是否声明
   *
   * @return true 包含基本块
   * @return false 不包含基本块
   */
  bool is_declaration() { return basic_blocks_.empty(); }
  /**
   * @brief Set the instr name object，为参数和基本块设置名称
   *
   */
  void set_instr_name();
  /**
   * @brief 打印函数
   *
   * @return std::string 字符串
   */
  std::string print();

private:
  std::list<BasicBlock *> basic_blocks_; // basic blocks
  std::list<Argument *> arguments_;      // arguments
  Module *parent_;
  unsigned seq_cnt_;
  /**
   * @brief 创建函数参数列表
   *
   */
  void build_args();
};

/**
 * @brief Argument of Function, does not contain actual value，函数参数类
 *
 */
class Argument : public Value {
public:
  /**
   * @brief Construct a new Argument object
   *
   * @param ty 参数类型
   * @param name 参数名称
   * @param f 所属函数
   * @param arg_no 参数列表中的位置
   */
  explicit Argument(Type *ty, const std::string &name = "",
                    Function *f = nullptr, unsigned arg_no = 0)
      : Value(ty, name), parent_(f), arg_no_(arg_no) {}
  /**
   * @brief Destroy the Argument object，析构函数
   *
   */
  ~Argument() {}
  /**
   * @brief Get the parent object，获取参数所属函数
   *
   * @return const Function* 常量函数指针
   */
  inline const Function *get_parent() const { return parent_; }
  /**
   * @brief Get the parent object，获取参数所属函数
   *
   * @return Function* 函数指针
   */
  inline Function *get_parent() { return parent_; }
  /**
   * @brief 深拷贝
   *
   * @return Argument* ，获取新的参数对象指针
   */
  Argument *deepcopy() { return new Argument(type_, name_, parent_, arg_no_); }
  /**
   * @brief Get the arg no object，获取参数列表参数个数
   *
   * @return unsigned 参数个数
   * @note "void foo(int a, float b)" a is 0 and b is 1.
   */
  unsigned get_arg_no() const {
    assert(parent_ && "can't get number of unparented arg");
    return arg_no_;
  }
  /**
   * @brief 打印参数列表
   *
   * @return std::string 字符串
   */
  virtual std::string print() override;

private:
  Function *parent_;
  unsigned arg_no_; // argument No.
};

#endif // SYSYC_FUNCTION_H
