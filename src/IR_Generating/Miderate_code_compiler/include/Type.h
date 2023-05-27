/*!
 *@file Type.h
 *@brief 类型接口头文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#ifndef SYSYC_TYPE_H
#define SYSYC_TYPE_H

#include <iostream>
#include <vector>

class Module;
class IntegerType;
class FunctionType;
class ArrayType;
class PointerType;
class FloatType;

/**
 * @brief 数值类型
 *
 * @note 包含基本的value类型
 */
class Type {
public:
  /**
   * @brief 类型ID
   *
   */
  enum TypeID {
    VoidTyID,     // Void
    LabelTyID,    // Labels, BasicBlock
    IntegerTy1ID, // Integers, include 32 bits and 1 bit
    IntegerTy32ID,
    FunctionTyID, // Functions
    ArrayTyID,    // Arrays
    PointerTyID,  // Pointer
    FloatTyID     // float
  };

private:
  TypeID tid_;
  Module *m_;
  virtual void _t(){};

public:
  /**
   * @brief Construct a new Type object
   *
   * @param tid 类型
   * @param m 所属模块
   */
  explicit Type(TypeID tid, Module *m);
  /**
   * @brief Destroy the Type object
   *
   */
  virtual ~Type() = default;

  /**
   * @brief Get the type id object，获取类型ID
   *
   * @return TypeID 类型常量
   */
  TypeID get_type_id() const { return tid_; }

  /**
   * @brief 判断是否为void类型
   *
   * @return true 是
   * @return false 不是
   */
  bool is_void_type() const { return get_type_id() == VoidTyID; }

  /**
   * @brief 判断是否为label类型（基本块）
   *
   * @return true 是
   * @return false 不是
   */
  bool is_label_type() const { return get_type_id() == LabelTyID; }

  /**
   * @brief 判断是否为整数类型
   *
   * @return true 是 （包括32位和1位）
   * @return false 不是
   */
  bool is_integer_type() const {
    return get_type_id() == IntegerTy32ID || get_type_id() == IntegerTy1ID;
  }

  /**
   * @brief 判断是否为function类型
   *
   * @return true 是
   * @return false 不是
   */
  bool is_function_type() const { return get_type_id() == FunctionTyID; }

  /**
   * @brief 判断是否为array类型
   *
   * @return true 是
   * @return false 不是
   */
  bool is_array_type() const { return get_type_id() == ArrayTyID; }

  /**
   * @brief 判断是否为指针类型
   *
   * @return true 是
   * @return false 不是
   */
  bool is_pointer_type() const { return get_type_id() == PointerTyID; }

  /**
   * @brief 判断是否为浮点类型
   *
   * @return true 是
   * @return false 不是
   */
  bool is_float_type() const { return get_type_id() == FloatTyID; }

  /**
   * @brief 判断是否为整数类型 32位
   *
   * @return true 是
   * @return false 不是
   */
  bool is_int32_type() const { return get_type_id() == IntegerTy32ID; }

  /**
   * @brief 判断是否为整数类型 1位
   *
   * @return true 是
   * @return false 不是
   */
  bool is_int1_type() const { return get_type_id() == IntegerTy1ID; }

  /**
   * @brief 判断两个类型是否一致
   *
   * @return true 是
   * @return false 不是
   */
  static bool is_eq_type(Type *ty1, Type *ty2);

  /**
   * @brief 获取一个void类型指针
   * @param m 所属模块
   */
  static Type *get_void_type(Module *m);

  /**
   * @brief Get the label type object
   *
   * @param m 所属模块
   * @return Type* label类型指针
   */
  static Type *get_label_type(Module *m);

  /**
   * @brief Get the int1 type object
   *
   * @param m 所属模块
   * @return IntegerType* 1位整数类型指针
   */
  static IntegerType *get_int1_type(Module *m);

  /**
   * @brief Get the int32 type object
   *
   * @param m 所属模块
   * @return IntegerType* 32位整数类型指针
   */
  static IntegerType *get_int32_type(Module *m);

  /**
   * @brief Get the int32 ptr type object
   *
   * @param m 所属模块
   * @return PointerType* 32位整数指针类型指针
   */
  static PointerType *get_int32_ptr_type(Module *m);

  /**
   * @brief Get the float type object
   *
   * @param m 所属模块
   * @return FloatType* 浮点数据类型指针
   */
  static FloatType *get_float_type(Module *m);

  /**
   * @brief Get the float ptr type object
   *
   * @param m 所属模块
   * @return PointerType* 浮点数据指针类型指针
   */
  static PointerType *get_float_ptr_type(Module *m);

  /**
   * @brief Get the pointer type object
   *
   * @param contained 所指向的数据类型指针
   * @return PointerType* 指针类型指针
   */
  static PointerType *get_pointer_type(Type *contained);

  /**
   * @brief Get the array type object
   *
   * @param contained 数组类型
   * @param num_elements 数组元素个数
   * @return ArrayType* 数组类型指针
   */
  static ArrayType *get_array_type(Type *contained, unsigned num_elements);

  /**
   * @brief Get the pointer element type object，获取指针指向数据的类型
   *
   * @return Type* 数据类型指针
   */
  Type *get_pointer_element_type();

  /**
   * @brief Get the array element type object，获取数组元素数据类型
   *
   * @return Type* 数组元素数据类型指针
   */
  Type *get_array_element_type();

  /**
   * @brief Get the size object，获取对应类型数据大小
   *
   * @param extended 范围是否延申，即在判定数组指针时，是否计算指向的数组大小
   * @return int 大小
   */
  int get_size(bool extended = true);

  /**
   * @brief Get the module object，获取所属模块
   *
   * @return Module* 模块指针
   */
  Module *get_module();

  /**
   * @brief 打印类型
   *
   * @return std::string，字符串
   */
  std::string print();
};

/**
 * @brief void类型
 *
 */
class VoidType : public Type {
private:
protected:
public:
  /**
   * @brief Construct a new Void Type object
   *
   * @param m 所属模块
   */
  explicit VoidType(Module *m);
};

/**
 * @brief 整数类型
 *
 */
class IntegerType : public Type {
private:
  /// @brief 整数的位：32/1
  unsigned num_bits_;

protected:
public:
  /**
   * @brief Construct a new Integer Type object
   *
   * @param num_bits 位数：32/1
   * @param m 所属模块
   */
  explicit IntegerType(unsigned num_bits, Module *m);

  /**
   * @brief 获取一个构建好的整数类型指针
   *
   * @param num_bits 位数
   * @param m 所属模块
   * @return IntegerType*
   */
  static IntegerType *get(unsigned num_bits, Module *m);

  /**
   * @brief Get the num bits object，获取整数类型对应的位数
   *
   * @return unsigned 位数结果
   */
  unsigned get_num_bits();
};

/**
 * @brief 函数类型
 *
 */
class FunctionType : public Type {
private:
  /// @brief 返回结果类型
  Type *result_;
  /// @brief 参数类型列表
  std::vector<Type *> args_;

protected:
public:
  /**
   * @brief Construct a new Function Type object
   *
   * @param result 返回参数类型
   * @param params 参数类型列表
   *
   * @return 创建对象本身
   */
  FunctionType(Type *result, std::vector<Type *> params);

  /**
   * @brief 判断返回类型是否有效
   *
   * @param ty 待判定的返回类型指针
   * @return true
   * @return false
   */
  static bool is_valid_return_type(Type *ty);
  /**
   * @brief 判断参数类型是否有效
   *
   * @param ty 待判定的参数类型指针
   * @return true
   * @return false
   */
  static bool is_valid_argument_type(Type *ty);

  /**
   * @brief 创建一个函数类型指针
   *
   * @param result 返回参数类型指针
   * @param params 参数类型指针数组
   * @return FunctionType* 函数类型指针
   */
  static FunctionType *get(Type *result, std::vector<Type *> params);

  /**
   * @brief Get the num of args object，获取参数个数
   *
   * @return unsigned 参数数量
   */
  unsigned get_num_of_args() const;

  /**
   * @brief Get the param type object，获取参数类型
   *
   * @param i 要获取的参数索引
   * @return Type* 参数类型结果
   */
  Type *get_param_type(unsigned i) const;
  /**
   * @brief 获取参数列表第一个参数
   *
   * @return std::vector<Type *>::iterator 迭代器，第一个参数指针
   */
  std::vector<Type *>::iterator param_begin() { return args_.begin(); }
  /**
   * @brief 获取参数列表最后一个参数
   *
   * @return std::vector<Type *>::iterator 迭代器，最后一个参数指针
   */
  std::vector<Type *>::iterator param_end() { return args_.end(); }
  /**
   * @brief Get the return type object，获取返回类型的指针
   *
   * @return Type* 返回类型结果指针
   */
  Type *get_return_type() const;
};

/**
 * @brief 数组类型
 *
 */
class ArrayType : public Type {
private:
  Type *contained_;       // The element type of the array.
  unsigned num_elements_; // Number of elements in the array.
protected:
public:
  /**
   * @brief Construct a new Array Type object，创建一个数组类型对象
   *
   * @param contained 数组类型
   * @param num_elements 数组元素个数
   */
  ArrayType(Type *contained, unsigned num_elements);

  /**
   * @brief 判断元素类型是否有效
   *
   * @param ty 元素类型
   * @return true
   * @return false
   */
  static bool is_valid_element_type(Type *ty);

  /**
   * @brief 创建一个数组类型，并返回指针
   *
   * @param contained 数组元素类型
   * @param num_elements 数组元素个数
   * @return ArrayType* 数组元素类型
   */
  static ArrayType *get(Type *contained, unsigned num_elements);

  /**
   * @brief Get the element type object，获取数组元素类型
   *
   * @return Type* 数组元素类型指针
   */
  Type *get_element_type() const { return contained_; }
  /**
   * @brief Get the num of elements object，获取数组元素个数
   *
   * @return unsigned 元素个数
   */
  unsigned get_num_of_elements() const { return num_elements_; }
};

/**
 * @brief 指针类型
 *
 */
class PointerType : public Type {
private:
  /// The element type of the ptr.
  Type *contained_;

protected:
public:
  /**
   * @brief Construct a new Pointer Type object
   *
   * @param contained 指针指向元素的类型
   */
  PointerType(Type *contained);
  /**
   * @brief Get the element type object，获取指针指向元素的类型
   *
   * @return Type*
   */
  Type *get_element_type() const { return contained_; }

  /**
   * @brief 创建一个指针类型
   *
   * @param contained 指向元素的类型
   * @return PointerType* 指针类型指针
   */
  static PointerType *get(Type *contained);
};

/**
 * @brief 浮点类型
 *
 */
class FloatType : public Type {
private:
protected:
public:
  /**
   * @brief Construct a new Float Type object，构建浮点类型
   *
   * @param m 所属模块
   */
  FloatType(Module *m);
  /**
   * @brief 创建一个浮点类型
   *
   * @param m 所属模块
   * @return FloatType*
   */
  static FloatType *get(Module *m);
};

#endif // SYSYC_TYPE_H