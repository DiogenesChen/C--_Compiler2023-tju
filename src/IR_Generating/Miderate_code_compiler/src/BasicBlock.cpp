/*!
 *@file BasicBlock.cpp
 *@brief 基本块接口定义文件
 *@version 1.0.0
 *@date 2022-10-04
 */
#include "BasicBlock.h"
#include "Function.h"
#include "IRprinter.h"
#include "Module.h"
#include <cassert>

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
 *构建默认为真基本块，名称为空，类型为基本块
 *&emsp; 如果parent为空，创建失败，异常退出
 *&emsp; 向所属的函数中添加基本块指针
 */
BasicBlock::BasicBlock(Module *m, const std::string &name = "",
                       Function *parent = nullptr, bool fake = false)
    : Value(Type::get_label_type(m), name), parent_(parent), _fake(fake) {
  assert(parent && "currently parent should not be nullptr");
  parent_->add_basic_block(this);
}

/*!
 *@brief 向基本块中添加指令
 *@param 待添加的指令指针
 *@note
 *----------
 *在基本块的尾部添加指令
 *&emsp; 设置待添加的指令后缀为空
 *&emsp; **if** 如果指令链表为空
 *&emsp;&emsp; 设置指令为后继为空
 *&emsp;&emsp; 不为空，则获取指令链表的最后一个指令
 *&emsp;&emsp; 设置插入指令的前继为最后一个指令
 *&emsp;&emsp; 最后一个指令后继为待插入指令
 *&emsp; 尾部插入指令
 */
void BasicBlock::add_instruction(Instruction *instr) {
  instr->setSuccInst(nullptr);
  if (instr_list_.empty()) {
    instr->setSuccInst(nullptr);
  } else {
    Instruction *last_inst = instr_list_.back();
    instr->setPrevInst(last_inst);
    last_inst->setSuccInst(instr);
  }
  instr_list_.push_back(instr);
}

  /*!
   *@brief 返回基本块的所属模块
   *@return 从属的模块对象指针
   *@note
   *----------
   *return parent, or null if none.
   */
  Module *BasicBlock::get_module() { return get_parent()->get_parent(); }

  /*!
   *@brief 将基本块从从属的函数中删除
   *@note
   *----------
   *获取基本块的所属函数并删除函数所保存的关于该基本块的指针
   */
  void BasicBlock::erase_from_parent() { this->get_parent()->remove(this); }

/*!
 *@brief 向基本块中添加指令
 *@param 待添加的指令指针
 *@note
 *----------
 *在基本块的头部添加指令
 *&emsp; 设置待添加的指令前继为空
 *&emsp; **if** 如果指令链表为空
 *&emsp;&emsp; 设置指令为后继为空
 *&emsp;&emsp; 不为空，则获取指令链表的第一个指令
 *&emsp;&emsp; 设置插入指令的后继为第一个指令
 *&emsp;&emsp; 第一个指令前继为待插入指令
 *&emsp; 头部插入指令
 */
void BasicBlock::add_instr_begin(Instruction *instr) {
  instr->setPrevInst(nullptr);
  if (instr_list_.empty()) {
    instr->setSuccInst(nullptr);
  } else {
    Instruction *first_inst = instr_list_.front();
    instr->setSuccInst(first_inst);
    first_inst->setPrevInst(instr);
  }
  instr_list_.push_front(instr);
}

/*!
 *@brief 在phi指令之后添加指令
 *@param 待添加的指令指针
 *@note
 *----------
 *向phi指令后添加指令
 *&emsp; 设置指令的从属基本块
 *&emsp; 获取指令链表的开始
 *&emsp;&emsp; **for** 循环，遍历获得phi指令点
 *&emsp; **if** 如果不是链表头，phi节点前继为节点前继
 *&emsp; **if** 如果不是链表尾，phi节点后继设置为节点本身
 *&emsp; 修正插入节点的连接关系，前继和后继的修改
 */
void BasicBlock::add_instr_after_phi(Instruction *instr) {
  instr->set_parent(this);
  auto it = instr_list_.begin();
  //遍历获得phi指令点
  for (; it != instr_list_.end(); ++it) {
    if (!(*it)->is_phi()) {
      break;
    }
  }
  //获取插入点
  Instruction *front = nullptr, *back = nullptr;
  if (it != instr_list_.begin()) {
    front = *(--it);
    ++it;
  }
  if (it != instr_list_.end()) {
    back = *it;
  }
  if (front != nullptr) {
    front->setSuccInst(instr);
  }
  if (back != nullptr) {
    back->setPrevInst(instr);
  }
  // 设置待插入节点的前后继
  instr->setPrevInst(front);
  instr->setSuccInst(back);
  instr_list_.insert(it, instr);
}

/*!
 *@brief 删除基本块中的某个指令
 *@param 待删除的指令指针
 *@note
 *----------
 *&emsp; 删除维护的指令链表中的指令
 *&emsp; 获取删除的指令的前后指令
 *&emsp; 修正指令的链接关系：
 *&emsp; **if** 如果前置指令不为空
 *&emsp;&emsp; 为前置指令设置新的后置指令
 *&emsp; **if** 如果后置指令不为空
 *&emsp;&emsp; 为后置指令设置新的前置指令
 *&emsp; 被删除的指令进行相关use的删除
 */
void BasicBlock::delete_instr(Instruction *instr) {
  instr_list_.remove(instr);
  Instruction *prev = instr->getPrevInst();
  Instruction *succ = instr->getSuccInst();
  //修正指令的链接关系
  if (prev != nullptr) {
    prev->setSuccInst(succ);
  }
  if (succ != nullptr) {
    succ->setPrevInst(prev);
  }
  //被删除的指令进行相关use的删除
  instr->remove_use_of_ops();
}

/*!
 *@brief 获取基本块内的终结指令
 *@return 终结指令常量指针
 *@note
 *----------
 *Returns the terminator instruction if the block is well formed
 *if the block is not well formed then null
 *返回基本块的的中的终结指令常量指针
 *---------
 *&emsp; 如果为空，返回null
 *&emsp; 获取终结指令的类型，并进行对应的判定
 *&emsp;&emsp; 返回指令，返回指令表的最后一个指令
 *&emsp;&emsp; 条件指令，返回指令表的最后一个指令
 *&emsp;&emsp; 其他指令，判定为空，返回
 */
const Instruction *BasicBlock::get_terminator() const {
  if (instr_list_.empty()) {
    return nullptr;
  }
  switch (instr_list_.back()->get_instr_type()) {
  case Instruction::ret:
    return instr_list_.back();
    break;

  case Instruction::br:
    return instr_list_.back();
    break;

  default:
    return nullptr;
    break;
  }
}

/*!
 *@brief 打印基本块
 *@return 待打印的字符串
 *@note
 *----------
 *return parent, or null if none.
 *&emsp; 如果基本块为假，那么输出空
 *&emsp; 定义输出字符串string
 *&emsp; 添加基本块名称，
 *&emsp; 添加前置基本块的说明，依次打印前置基本块
 *&emsp; 隶属于函数则进行空行添加
 *&emsp; 依次打印维护的指令链表内容
 *&emsp; 如果基本块无终结指令，默认添加
 */
std::string BasicBlock::print() {
  if (_fake) {
    return "";
  }
  std::string bb_ir;
  bb_ir += this->get_name();
  bb_ir += ":";
  // print prebb
  if (!this->get_pre_basic_blocks().empty()) {
    bb_ir += "                                                ; preds = ";
  }
  for (auto bb : this->get_pre_basic_blocks()) {
    if (bb != *this->get_pre_basic_blocks().begin())
      bb_ir += ", ";
    bb_ir += print_as_op(bb, false);
  }

  // print func
  if (!this->get_parent()) {
    bb_ir += "\n";
    bb_ir += "; Error: Block without parent!";
  }
  bb_ir += "\n";
  for (auto instr : this->get_instructions()) {
    bb_ir += "  ";
    bb_ir += instr->print();
    bb_ir += "\n";
  }

  // 空BasicBlock，自动加上return语句
  if (get_terminator() == nullptr) {
    bb_ir += "  ";
    if (get_parent()->get_return_type()->is_void_type()) {
      bb_ir += "ret void\n";
    } else {
      bb_ir += "ret i32 0\n";
    }
  }

  return bb_ir;
}