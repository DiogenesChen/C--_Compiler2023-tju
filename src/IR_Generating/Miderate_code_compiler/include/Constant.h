/*!
 *@file Constant.h
 *@brief 常量接口头文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#ifndef SYSYC_CONSTANT_H
#define SYSYC_CONSTANT_H

#include "Type.h"
#include "User.h"
#include "Value.h"

/*!
 *@brief 常量基类
 *constant variable
 */
class Constant : public User {
private:
public:
  /*!
   *@brief 常量基类构造函数
   *@param ty 常量类型
   *@param name 常量名称
   *@param 常量的操作数序号，默认为0
   *@return 自身类对象
   *constant variable
   */
  Constant(Type *ty, const std::string &name = "", unsigned num_ops = 0)
      : User(ty, name, num_ops) {}
  /*!
   *@brief 常量基类析构函数
   *constant variable
   */
  ~Constant() = default;
};

/*!
 *@brief 常量整型
 *constant int variable
 */
class ConstantInt : public Constant {
private:
  int value_; /// 初始值

public:
  /*!
   *@brief 常量整数类构造函数
   *@param ty 常量类型
   *@param val 常量数值
   *@return 自身类对象
   *constant variable
   */
  ConstantInt(Type *ty, int val) : Constant(ty, "", 0), value_(val) {}
  /*!
   *@brief 获取常量值
   *@param const_val 常量对象指针
   *@return 对象指针元素value值
   */
  static int get_value(ConstantInt *const_val) { return const_val->value_; }
  /*!
   *@brief 获取常量值
   *@return 内部对象指针元素value值
   */
  int get_value() const { return value_; }
  /*!
   *@brief 常量整数类32位创建函数
   *@param val 常量值
   *@param m 所属模块
   *@return 常量类对象指针
   */
  static ConstantInt *get(int val, Module *m);
  /*!
   *@brief 常量整数类1位创建函数
   *@param val 常量值
   *@param m 所属模块
   *@return 常量类对象指针
   */
  static ConstantInt *get(bool val, Module *m);
  /*!
   *@brief 打印常量类变量
   *@return 字符串
   */
  std::string print() override;
};

/*!
 *@brief 常量数组
 *constant int array
 */
class ConstantArray : public Constant {
private:
  std::vector<Constant *> const_array; ///! 常量数组
  /*!
   *@brief 常量整数类构造函数
   *@param ty 常量类型
   *@param val 常量数值类型数组
   *@return 自身类对象
   *constant int array
   */
  ConstantArray(ArrayType *ty, const std::vector<Constant *> &val);

public:
  /*!
   *@brief 常量整数类析构函数
   *constant int array
   */
  ~ConstantArray() = default;

  /*!
   *@brief 获取常量数组指定索引的常量数值
   *@param index 索引
   *@return 常量类指针
   *constant int array
   */
  Constant *get_element_value(int index);

  /*!
   *@brief 获取常量类数组大小
   *@return 常量数组大小
   *constant int array
   */
  unsigned get_size_of_array() { return const_array.size(); }

  /*!
   *@brief 常量数组类的创建函数
   *@param ty 数组元素的类型
   *@param val 常量类数组
   *@return 常量数组类指针
   *constant int array
   */
  static ConstantArray *get(ArrayType *ty, const std::vector<Constant *> &val);

  /*!
   *@brief 常量数组类打印函数
   *@return 字符串
   *constant int array
   */
  std::string print() override;

  /*!
   *@brief 常量整数类构造函数
   *@param dim 常量类型
   *@param init 初始化数组
   *@param m 所属数组
   *@return 常量类指针数组
   *@todo 仅适用于全局常量数组的赋值
   *constant int array
   */
  static std::vector<Constant *>
  IntegerList2Constant(const std::vector<int> &dim,
                       const std::vector<int> &init, Module *m) {
    std::vector<Constant *> st;
    std::vector<Constant *> ost;
    Type *ty = Type::get_int32_type(m);
    for (int i : init) {
      ost.emplace_back(ConstantInt::get(i, m));
    }
    int num_group = init.size();
    for (int i = (int)dim.size() - 1; i > 0; --i) {
      num_group /= dim[i];
      int offset = 0;
      ty = ArrayType::get(ty, dim[i]);
      for (int gp = 0; gp < num_group; ++gp) {
        std::vector<Constant *> arr;
        for (int j = 0; j < dim[i]; ++j) {
          arr.push_back(ost[offset + j]);
        }
        st.push_back(ConstantArray::get(dynamic_cast<ArrayType *>(ty), arr));
        offset += dim[i];
      }
      ost = st;
      st.clear();
    }
    return ost;
  };
};

/*! 常量零值
 *constant int zero
 */
class ConstantZero : public Constant {
private:
  explicit ConstantZero(Type *ty) : Constant(ty, "", 0) {}

public:
  /*!
   *@brief 常量整数类构造函数
   *@param ty 常量类型
   *@param val 常量数值类型数组
   *@return 自身类对象
   *constant int zero
   */
  static ConstantZero *get(Type *ty, Module *m);
  /*!
   *@brief 打印常量零值
   *@return 字符串
   *constant int zero
   */
  std::string print() override;
};
#endif // SYSYC_CONSTANT_H
