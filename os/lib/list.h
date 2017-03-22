/**　　　　　　 ,-...　　　 　 　 　　　..-.
 *　　　　　 ./:::::＼　　　　 　 　  ／:::::ヽ
 *　　　　  /::::::::::::;ゝ--──-- .._/:::::::ヽ
 *　　　　 /,.-‐''"′                  ＼:::::::|
 *　　　  ／　 　　　　　　　　　　　　  ヽ.::|
 *　　　　/　　　　●　　　 　 　 　 　 　 ヽ|
 *　　 　 l　　　...　　 　 　 　  ●　      l
 *　　　 .|　　　 　　 (_人__丿   ...　    |
 *　 　 　l　　　　　　　　　　　　 　　  l
 *　　　　` .　　　　　　　　 　 　 　　 /
 *　　　　　　`. .__　　　 　 　 　　.／
 *　　　　　　　　　/`'''.‐‐──‐‐‐┬--- 
 * File      : list.h
 * This file is part of ACGrtos
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/**
 * *******Built with Linux kernel source*******
 */

/**
 *	left tail, right head
 */

#ifndef LIST_H_
#define LIST_H_

/**
 * @addtogroup Task 基本定义
 */

/*@{*/

/**
 * list节点基本模型
 */
typedef listHead_t {
  listHead_t *previous;    /**< 前节点 */
  listHead_t *next;        /**< 后节点 */
} listHead_t;

/*@}*/

/**
 * @addtogroup list 方法
 */

/*@{*/

/**
 * 节点添加(丢弃方法)
 *
 * @param listHead_t* new_lst
 * @param listHead_t* previous_list
 * @param listHead_t* next_list
 * 
 * @return none
 */
static __inline void __DROP__ListAdd(listHead_t* new_lst, listHead_t* previous_list, listHead_t* next_list) {
	next_list->previous = new_lst;
	new_lst->next = next_list;
	new_lst->previous = previous_list;
	previous_list->next = new_lst;
}


/**
 * 节点删除(丢弃方法)
 *
 * @param listHead_t* previous_list
 * @param listHead_t* next_list
 * 
 * @return none
 */
static __inline void __DROP__Delete(listHead_t* prev, listHead_t* next) {
	next->previous = prev;
	prev->next = next;
}


/**
 * 获取结构体中成员的位置
 *
 * @param tpye 结构体类型
 * @param name 成员名称
 * 
 * @return none
 */
#define osList_OffsetOf(tpye, name) \
	((unit32_t) &((TYPE *)0)->name)


/**
 * 获取当前list_head链表节点所在的宿主结构项
 *
 * @param ptr 宿主结构体下的listHead_t*
 * @param type 宿主类型
 * @param name 宿主结构类型定义中listHead_t成员名
 * 
 * @return 宿主结构体指针
 */
#define osList_Entry(ptr, type, name) \
    (type*)((uint8_t*)(ptr)-osList_OffsetOf(type, name))

	
/**
 * 遍历链表中的所有list_head节点
 *
 * @param pos 位置
 * @param head 节点头
 * 
 * @return none
 */
#define osList_ForEach(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)


/**
 * (安全模式)遍历链表中的所有list_head节点
 *
 * @param pos 位置
 * @param head 节点头
 * 
 * @return none
 */
#define osList_ForEachSafe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

/**
 * 获取结构体成员宿主指针
 *
 * @param ptr 成员指针
 * @param type 宿主类型
 * @param member 成员在宿主结构体中的名称
 * 
 * @return 宿主结构体指针
 */
#define container_of(ptr, type, member) ({ \
          const typeof( ((type *)0)->member ) *__mptr = (ptr); \
            (type *)( (char *)__mptr - __offsetof(type,member));})


/**
 * 初始化节点
 *
 * @param head 节点 
 * 
 * @return none
 */	
static __inline void osList_HeadInit(listHead_t* head) {
	head->next = head;
	head->previous = head;
}


/**
 * 将newNode添加到list节点尾部
 *
 * @param list 原节点 
 * @param newNode 新节点 
 * 
 * @return none
 */	
static __inline void osList_AddTail(listHead_t* list, listHead_t* newNode) {
	//__ListAdd(node, list->previous, list);
  list->previous->next = newNode;
  newNode->previous = list->previous;

  list->previous = newNode;
  newNode->next = list;
}


/**
 * 将newNode添加到list节点头部
 *
 * @param list 原节点 
 * @param newNode 新节点 
 * 
 * @return none
 */	
static __inline void osList_Add(listHead_t* list, listHead_t* newNode) {
	//__ListAdd(node, list, node->next);
  list->next->previous = newNode;
  newNode->next = list->next;

  list->next = newNode;
  newNode->previous = list;
}


/**
 * 删除一个节点
 *
 * @param node 节点 
 * 
 * @return none
 */	
static __inline void osList_DeleteNode(listHead_t* node) {
	node->next->previous = node->previous;
	node->previous->next = node->next;

	node->next = node->previous = node;
}


/**
 * 检查node是不是head的最后一个节点
 *
 * @param head 对象所在所在列表 
 * @param node 被检查对象 
 * 
 * @return bool
 */	
static __inline int osList_CheckIsLast(listHead_t* head, listHead_t* node) {
	return (node->next == head);
}


/**
 * 检查链表是不是空
 *
 * @param listHead_t* 节点 
 * 
 * @return bool
 */	
static __inline int osList_CheckIsEmpty(listHead_t* head) {
	return (head->next == head);
}


/**
 * 将node节点移动到head的头部
 *
 * @param head 节点 
 * @param node 被移动节点
 * 
 * @return none
 */	
static __inline void osList_Move(listHead_t* head, listHead_t* node) {
	osList_DeleteNode(node);
	osList_Add(node, head);
}


/**
 * 将node移动到head的尾部
 *
 * @param head 节点 
 * @param node 被移动节点
 * 
 * @return none
 */	
static __inline void osList_MoveTail(listHead_t* head, listHead_t* node) {
	osList_DeleteNode(node);
	osList_AddTail(node, head);
}


/**
 * 将外源列表list合并到head列表中
 *
 * @param head 原节点 
 * @param list 外源节点
 * 
 * @return none
 */	
static __inline void osList_Splice(listHead_t* head, listHead_t* list) {
	if (!osList_CheckIsEmpty(list)) {
		listHead_t *first = list->next;
		listHead_t *last = list->previous;
		listHead_t *at  = head->next;

		first->previous = head;
		head->next = first;

		last->next = at;
		at->previous = last;
	}
}

/*@}*/
#endif // !LIST_H_
