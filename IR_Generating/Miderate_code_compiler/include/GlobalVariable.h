/*!
 *@file GlobalVariable.h
 *@brief 全局变量接口头文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#ifndef SYSYC_GLOBALVARIABLE_H
#define SYSYC_GLOBALVARIABLE_H

#include "Constant.h"
#include "Module.h"
#include "User.h"

/*! 全局变量类，包含常量*/
class GlobalVariable : public User {
private:
  bool is_const_;      //<! 是否为常量
  Constant *init_val_; //<! 初始值
  std::vector<int>
      _flatten_init_val; //<! 常量数组的存储数组(扁平化，多维数组降为一维)
  /*!
   *@brief 全局变量的构造函数
   *@param name 全局变量名称
   *@param m 所从属模块
   *@param ty 变量的类型
   *@param is_const 是否为常量
   *@param init 常量指针
   *@return 当前对象本身
   */
  GlobalVariable(std::string name, Module *m, Type *ty, bool is_const,
                 Constant *init = nullptr);

public:
  /*!
   *@brief 全局变量的创建函数
   *@param name 全局变量名称
   *@param m 所从属模块
   *@param ty 变量的类型
   *@param is_const 是否为常量
   *@param init 常量指针
   *@return 当前对象本身
   */
  static GlobalVariable *create(std::string name, Module *m, Type *ty,
                                bool is_const, Constant *init);

  /*!
   *@brief 全局变量的创建函数
   *@param name 全局变量名称
   *@param m 所从属模块
   *@param ty 变量的类型
   *@param is_const 是否为常量
   *@param init 常量指针
   *@return 当前对象本身
   */
  Constant *get_init() { return init_val_; }

  /*!
   *@brief 判断是否是常量
   *@return 常量判定结果
   *@note 是常量为1，不是为0
   */
  bool is_const() const { return is_const_; }

  /*!
   *@brief 扁平化数组的创建
   *@param i 常量化的扁平数组
   */
  void setFlattenInit(const std::vector<int> &i) { _flatten_init_val = i; }

  /*!
   *@brief 获取扁平化数组
   *@return 常量扁平化数组
   */
  std::vector<int> getFlattenInit() const { return _flatten_init_val; }

  /*!
   *@brief 打印全局变量
   *@return 字符串
   */
  std::string print();
};
#endif // SYSYC_GLOBALVARIABLE_H
