/*
author:caocun
function: 红黑树
date：2016-06-12
*/

#include<iostream>
using namespace std;

enum NodeColor{RED, BLACK};  //节点颜色类型

typedef int Type;        //关键字类型

typedef struct RBTreeNode
{
	NodeColor color;    //节点颜色
	Type key;                //关键字
	RBTreeNode *left;  //左孩子
	RBTreeNode *right;  //右孩子
	RBTreeNode *parent;  //父节点
}Node;

//左旋
void RBTreeLeftRotate(Node **root, Node *x)
{
	/* 
	* 左旋示意图(对节点x进行左旋)：
	*      px                              px
	*     /                               /
	*    x                               y                
	*   /  \      --(左旋)-->     / \                
	*  lx   y                          x  ry     
	*     /   \                       /  \
	*    ly   ry                     lx  ly  
	*
	*
	*/
	//第一步：将y的左孩子变成x的右孩子
	if(root == NULL || x == NULL) return ;
	Node *y= x->right;
	x->right = y->left;
	if(y->left != NULL)
		y->left->parent = x;

	//第二步：更新y的双亲
	y->parent = x->parent;
	if(y->parent == NULL)
	{
		*root = y;
	}
	else
	{
		if(x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}
	//第三步：更新x的双亲
	x->parent = y;
	y->left = x;
}
//右旋
static void RBTreeRightRotate(Node **root, Node *y)
{
	/* 
	* 右旋示意图(对节点y进行左旋)：
	*            py                               py
	*           /                                /
	*          y                                x                  
	*         /  \      --(右旋)-->      /  \                     
	*        x   ry                           lx   y  
	*       / \                                    / \                   
	*      lx  rx                                rx  ry
	* 
	*/
	if(root == NULL || y == NULL) return ;
	Node *x = y->left;

	//第一步：把x右孩子变成y的左孩子
	y->left = x->right;
	if(x->right != NULL) 
		x->right->parent = y;

	//第二步：更新x的双亲
	x->parent = y->parent;
	if(y->parent == NULL)
	{
		*root = x;
	}
	else 
	{
		if(y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right =x;
	}
	//第三步：更新y的双亲
	y->parent =x;
	x->right = y;
}
Node* FindNode(Node *root,const Type &key)
{
	if(root == NULL) return NULL;
	Node*ptr = root;
	Node *pa = NULL;
	while(ptr != NULL)
	{
		pa = ptr;
		if(ptr->key>key)
		{
			ptr = ptr->left;
		}
		else if(ptr->key<key)
		{
			ptr = ptr->right;
		}
		else
		{
			break;
		}
	}
	return pa;
}
static Node * Buynode(const Type &x)
{
	Node *ptr = (Node*)malloc(sizeof(Node));
	if(ptr == NULL)  return NULL;
	memset(ptr, 0, sizeof(Node));
	ptr->parent = NULL;
	ptr->left = ptr->right = NULL;
	ptr->color  = RED;
	ptr->key = x;
	return ptr;
}
void Freenode(Node *node)
{
	free(node);
}
void RBTreeInsertFixup(Node **root, Node *node)
{
	if(root == NULL  || node ==NULL) return;

	Node *pa = NULL;                //用来保存当前节点的父节点
	Node *gpa = NULL;              //用来保存当前节点的祖父节点

	while((pa = node->parent) != NULL && pa->color == RED)
	{
		gpa = pa->parent;
		//如果父节点是祖父节点的左孩子
		if(pa == gpa->left)
		{
			//叔叔结点
			Node *uncle = gpa->right;
			//case 1：叔叔是红色
			if(uncle != NULL && uncle->color == RED)
			{
				pa->color = BLACK;
				uncle->color = BLACK;
				gpa->color = RED;
				node = gpa;
				continue;
			} 
			//case 2：叔叔是黑色,且当前节点是父节点的右孩子
			if(node == pa->right)
			{
				node = pa;
				RBTreeLeftRotate(root, node);
			}
			//case 3: 叔叔是黑色,且当前节点是父节点的左孩子
			pa = node->parent;
			pa->color = BLACK;
			pa->parent->color = RED;
			RBTreeRightRotate(root, pa->parent);
		}
		else  		//如果父节点是祖父节点的右孩子
		{
			//叔叔结点
			Node *uncle = gpa->left;
			//case 1：叔叔是红色
			if(uncle != NULL && uncle->color == RED)
			{
				pa->color = BLACK;
				uncle->color = BLACK;
				gpa->color = RED;
				node = gpa;
				continue;
			} 
			//case 2：叔叔是黑色,且当前节点是父节点的左孩子
			if(node == pa->left)
			{
				node = pa;
				RBTreeRightRotate(root, node);
			}
			//case 3: 叔叔是黑色,且当前节点是父节点的右孩子
			pa = node->parent;
			pa->color = BLACK;
			pa->parent->color = RED;
			RBTreeLeftRotate(root, pa->parent);
		}
	}
	(*root)->color = BLACK;
}
 bool RBTreeInsert(Node **root, const Type &x)
{
	Node *s = Buynode(x);
	if(root == NULL || s == NULL) return false;

	if(*root == NULL)
	{
		s->color = BLACK;
		*root = s;
		return true;
	}

	Node *node = FindNode(*root, x);
	if(node == NULL || node->key == x) return false;

	s->parent = node;

	if(node->key>s->key)
		node->left = s;
	else
		node->right = s;

	//调整
	RBTreeInsertFixup(root, s);
	return true;
}
void InOrder(Node *root)
{
	if(root != NULL)
	{
		InOrder(root->left);
		cout<<root->key<<"  ";
		InOrder(root->right);
	}
}
//////////////////////////////////////////////////////////////////
void RBTreeDeleteFixup(Node **root, Node* node)
{
	if(root == NULL || node == NULL)  return ;
	//node不是根，且是“黑+黑节点”
	while(node != *root && node->color ==BLACK)
	{
		Node *pa = node->parent;
		if(pa == pa->left)
		{
			Node *other = pa->right;
			//case1：如果node节点的兄弟节点为红色
			if(other->color == RED)
			{
				other->color =BLACK;
				pa->color = RED;
				RBTreeLeftRotate(root, pa);
				other = pa->right;
			}
			//case2:兄弟节点的两个子节点全为黑色。
			if((other->left != NULL && other->left->color == BLACK)&&
				(other->right != NULL && other->right->color == BLACK))
			{
				other->color = RED;
				node = pa;
			}
			else
			{
				//case3:兄弟节点是黑色，兄弟的左子是红色，右子是黑色。
				if(other->right != NULL && other->right->color == BLACK)
				{
					other->left->color =BLACK;
					other->color = RED;
					RBTreeRightRotate(root, other);
					other = pa->right;
				}
				//case4:它的兄弟节点是黑色，但是兄弟节点的右子是红色，兄弟节点左子的颜色任意。
				other->color = pa->color;
				pa->color = BLACK;
				other->right->color = BLACK;
				RBTreeLeftRotate(root, pa);
				node = *root;
				break;
			}
		}
		else
		{
			Node *other = pa->left;
			//case1：如果node节点的兄弟节点为红色
			if(other->color == RED)
			{
				other->color =BLACK;
				pa->color = RED;
				RBTreeRightRotate(root, pa);
				other = pa->left;
			}
			//case2:兄弟节点的两个子节点全为黑色。
			if((other->left != NULL && other->left->color == BLACK)&&
				(other->right != NULL && other->right->color == BLACK))
			{
				other->color = RED;
				node = pa;
			}
			else
			{
				//case3:兄弟节点是黑色，兄弟的左子是红色，右子是黑色。
				if(other->left != NULL && other->left->color == BLACK)
				{
					other->right->color =BLACK;
					other->color = RED;
					RBTreeLeftRotate(root, other);
					other = pa->left;
				}
				//case4:它的兄弟节点是黑色，但是兄弟节点的右子是红色，兄弟节点左子的颜色任意。
				other->color = pa->color;
				pa->color = BLACK;
				other->left->color = BLACK;
				RBTreeRightRotate(root, pa);
				node = *root;
				break;
			}
		}
	}
	if(node != NULL)
		node->color = BLACK;
}
bool RBTreeDelete(Node **root, const Type &x)
{
	if(root == NULL || *root == NULL) return  false;
	Node *node = FindNode(*root, x);
	if(node == NULL ) return false;

	//删除的节点左孩子和右孩子都不为空
	if(node->left != NULL && node->right != NULL)
	{
		//查找当前节点的后序节点
		Node *next = node->right;
		while(next != NULL && next->left != NULL)
		{
			next = next->left;
		}
		//注意，这里只拷贝节点的值，不拷贝颜色
		node->key = next->key;
		node = next;
	}
	Node *pa = node->parent;
	//要考虑删除只有右孩子或者左孩子的根节点
	Node *child = node->right == NULL ? node->left:node->right;
	if(child != NULL)
	{
		child->parent = pa;
	}
	//删除的是根节点
	if(pa == NULL)
	{
		*root = child;
	}
	else //不是根节点
	{
		if(node == pa->left)
			pa->left = child;
		else
			pa->right = child;
	}
	if(node->color == BLACK)
		 RBTreeDeleteFixup(root, child);

	Freenode(node);
	return true;
}
int main()
{
	int arr[] = {11,2, 14, 1, 7, 15, 5, 8};
	int n = sizeof(arr)/sizeof(arr[0]);
	Node *root = NULL;
	for(int i=0; i<n; i++)
	{
	   cout<<RBTreeInsert(&root, arr[i]);
	}
	cout<<endl;
	InOrder(root);
	cout<<endl;
	int key;
	while(cin>>key, key!=-1)
	{
		RBTreeDelete(&root, key);
		InOrder(root);
		cout<<endl;
	}
	return 0;
}
