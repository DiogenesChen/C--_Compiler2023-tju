/*!
 *@file BasicBlock.h
 *@brief 基本块接口头文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#ifndef SYSYC_BASICBLOCK_H
#define SYSYC_BASICBLOCK_H

#include "Function.h"
#include "Instruction.h"
#include "Module.h"
#include "Value.h"

#include <list>
#include <set>
#include <string>
#include <vector>

class Function;
class Instruction;
class Module;

/*!
  @brief 基本块节点
*/
class BasicBlock : public Value {
private:
  std::list<BasicBlock *> pre_bbs_;     //!<  pre basic blocks
  std::list<BasicBlock *> succ_bbs_;    //!<  subsequence basic blocks
  std::list<Instruction *> instr_list_; //!<  instruction in basic block
  Function *parent_;                    //!<  belong to which function
  bool _fake;                           //!<  is fake basicblock

public:
  /*!
   *@brief 基本块的构造函数
   *@param m 所从属模块
   *@param name 基本块名称
   *@param parent 所从属的函数
   *@param fake 是否是假基本块，即基本块是否为空
   *@return 当前对象本身
   */
  explicit BasicBlock(Module *m, const std::string &name, Function *parent,
                      bool fake);

  /*!
   *@brief 基本块的创建函数
   *@param m 所从属模块
   *@param name 基本块名称
   *@param parent 所从属的函数
   *@param fake 是否是假基本块，即基本块是否为空，默认为
   *@return 创建的基本块对象指针
   *@note
   *----------
   *默认基本块名称为label_()number
   */
  static BasicBlock *create(Module *m, const std::string &name,
                            Function *parent, bool fake = false) {
    auto prefix = name.empty() ? "" : "label_";
    return new BasicBlock(m, prefix + name, parent, fake);
  }

  /*!
   *@brief 返回基本块的所属函数
   *@return 所从属的函数对象指针
   *@note
   *----------
   *return parent, or null if none.
   */
  Function *get_parent() { return parent_; }

  /*!
   *@brief 返回基本块的所属模块
   *@return 从属的模块对象指针
   *@note
   *----------
   *return parent, or null if none.
   */
  Module *get_module();

  /*!
   *@brief 返回基本块的前置基本块链
   *@return 前置基本块链
   *@note
   *----------
   *return pre_bbs_, or null if none.
   */
  std::list<BasicBlock *> &get_pre_basic_blocks() { return pre_bbs_; }

  /*!
   *@brief 返回基本块的前置基本块链
   *@return 后置基本块链
   *@note
   *----------
   *return succ_bbs_, or null if none.
   */
  std::list<BasicBlock *> &get_succ_basic_blocks() { return succ_bbs_; }

  /*!
   *@brief 向前置基本块链中插入新的基本块
   *@param bb 待插入的基本块指针
   *@note
   *----------
   */
  void add_pre_basic_block(BasicBlock *bb) { pre_bbs_.push_back(bb); }

  /*!
   *@brief 向后置基本块链中插入新的基本块
   *@param bb 待插入的基本块指针
   *@note
   *----------
   *return parent, or null if none.
   */
  void add_succ_basic_block(BasicBlock *bb) { succ_bbs_.push_back(bb); }

  /*!
   *@brief 更新基本块的前置基本块链
   *@param bb_list 基本块链的集合
   *@note
   *----------
   *&emsp; 清空原先的前置基本块链
   *&emsp; 用新的基本块链进行填充
   */
  void set_pre_bb(const std::set<BasicBlock *> &bb_list) {
    pre_bbs_.clear();
    pre_bbs_.insert(pre_bbs_.begin(), bb_list.begin(), bb_list.end());
  }

  /*!
   *@brief 更新基本块的后置基本块链
   *@param bb_list 基本块链的集合
   *@note
   *----------
   *&emsp; 清空原先的后置基本块链
   *&emsp; 用新的基本块链进行填充
   */
  void set_succ_bb(const std::set<BasicBlock *> &bb_list) {
    succ_bbs_.clear();
    succ_bbs_.insert(succ_bbs_.begin(), bb_list.begin(), bb_list.end());
  }

  /*!
   *@brief 替换指定的基本块
   *@param oldBB 被替换的旧基本块指针
   *@param newBB 用于替换的新基本块指针
   *@note
   *----------
   *####1 替换前置基本块链中的旧基本块
   *&emsp; **for** 遍历前置链表
   *&emsp;&emsp; 如果匹配到指定的基本块指针，进行替换
   *####2 替换后置基本块链中的就基本块
   *&emsp; **for** 遍历后置链表
   *&emsp;&emsp; 如果匹配到指定的基本块指针，进行替换
   */
  void replace_basic_block(BasicBlock *oldBB, BasicBlock *newBB) {
    for (auto it = pre_bbs_.begin(); it != pre_bbs_.end(); ++it) {
      if (*it == oldBB) {
        *it = newBB;
      }
    }
    for (auto it = succ_bbs_.begin(); it != succ_bbs_.end(); ++it) {
      if (*it == oldBB) {
        *it = newBB;
      }
    }
  }

  /*!
   *@brief 删除前置基本块链的指定基本块
   *@param 待删除的基本块指针
   *@note
   *----------
   *pre list remove bb
   */
  void remove_pre_basic_block(BasicBlock *bb) { pre_bbs_.remove(bb); }

  /*!
   *@brief 删除后置基本块链的指定基本块
   *@param 待删除的基本块指针
   *@note
   *----------
   *succ list remove bb
   */
  void remove_succ_basic_block(BasicBlock *bb) { succ_bbs_.remove(bb); }

  /*!
   *@brief 获取基本块内的终结指令
   *@return 终结指令常量指针
   *@note
   *----------
   *Returns the terminator instruction if the block is well formed
   *if the block is not well formed then null
   *返回基本块的的中的终结指令常量指针
   */
  const Instruction *get_terminator() const;

  /*!
   *@brief 获取基本块内的终结指令
   *@return 终结指令常量指针
   *@note
   *----------
   *Returns the terminator instruction if the block is well formed
   *if the block is not well formed then null
   *返回基本块的的中的终结指令常量指针
   */
  Instruction *get_terminator() {
    return const_cast<Instruction *>(
        static_cast<const BasicBlock *>(this)->get_terminator());
  }

  /*!
   *@brief 向基本块中添加指令
   *@param instr 待添加的指令指针
   *@note
   *----------
   *在基本块的尾部添加指令
   */
  void add_instruction(Instruction *instr);

  /*!
   *@brief 向基本块中添加指令
   *@param instr 待添加的指令指针
   *@note
   *----------
   *在基本块的头部添加指令
   */
  void add_instr_begin(Instruction *instr);

  /*!
   *@brief 在phi指令之后添加指令
   *@param 待添加的指令指针
   *@note
   *----------
   *向phi指令后添加指令
   */
  void add_instr_after_phi(Instruction *instr);

  /*!
   *@brief 删除基本块中的某个指令
   *@param 待删除的指令指针
   *@note
   *----------
   *&emsp; 删除维护的指令链表中的指令
   *&emsp; 获取删除的指令的前后指令
   *&emsp; 修正指令的链接关系：，
   *&emsp; **if** 如果前置指令不为空
   *&emsp;&emsp; 为前置指令设置新的后置指令
   *&emsp; **if** 如果后置指令不为空
   *&emsp;&emsp; 为后置指令设置新的前置指令
   *&emsp; 被删除的指令进行相关use的删除
   */
  void delete_instr(Instruction *instr);

  /*!
   *@brief 判断基本块维护的指令链表是否为空
   *@return 指令链表是否为空结果
   *@note
   *----------
   *为空1，不为空0
   */
  bool empty() { return instr_list_.empty(); }

  /*!
   *@brief 判断是否是假基本块
   *@return 基本块判定结果
   *@note
   *----------
   *为假1，不为假0
   */
  bool is_fake_block() { return _fake; }

  /*!
   *@brief 指针数量
   *@return 返回基本块维护指令链表的指针数量
   *@note
   *----------
   */
  int get_num_of_instr() { return instr_list_.size(); }

  /*!
   *@brief 获取基本块的指针链表
   *@return 指令指针链表引用
   *@note
   *----------
   */
  std::list<Instruction *> &get_instructions() { return instr_list_; }

  /*!
   *@brief 将基本块从从属的函数中删除
   *@note
   *----------
   *获取基本块的所属函数并删除函数所保存的关于该基本块的指针
   */
  void erase_from_parent();

  /*!
   *@brief 打印基本块
   *@note
   *----------
   *return parent, or null if none.
   */
  virtual std::string print() override;
};

#endif
