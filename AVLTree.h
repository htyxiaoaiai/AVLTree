#pragma once
#include<iostream>

using namespace std;

template<class K,class V>
struct AVLTreeNode
{
public:
	//构造函数
	AVLTreeNode(const K& key, const V& value)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _key(key)
		, _value(value)
		,_bf(0)
	{}

public:
	AVLTreeNode<K, V>* _left;//左孩子
	AVLTreeNode<K, V>* _right;//右孩子
	AVLTreeNode<K, V>* _parent;//父亲节点

	K _key; //关键字
	V _value; //数值
	int _bf; //平衡因子（右子树的高度减去左子树的高度）
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}
	~AVLTree()
	{
		_Destory(_root);
		_root = NULL;
	}
	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}

		//找到应该插入的节点
		Node* cur = _root;
		Node* parent = NULL;
		while (cur)
		{
			if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (key>cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		//将节点插入
		cur = new Node(key, value);
		if (key > parent->_key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//判断树是否平衡
		//更新平衡因子，不满足时进行旋转
		while (parent)
		{
			//更新平衡因子
			if (cur == parent->_left)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}
			//之前就是平衡的，再插入一个没有改变树的高度，所以肯定还是平衡树
			if (parent->_bf == 0)
			{
				break;
			}
			//插入一个节点之后子树平衡改变了树的高度，但是父节点不一定平衡
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				cur = parent;
				parent = cur->_parent;
			}
			else//2  -2 
			{
				if (cur->_bf == 1)
				{
					//左单旋
					if (parent->_bf == 2)
					{
						RotateL(parent);
					}
					//左右双旋
					else
					{
						RotateLR(parent);
					}
				}
				else
				{
					//右左双旋
					if (parent->_bf == 2)
					{
						RotateRL(parent);
					}
					//右单旋
					else
					{
						RotateR(parent);
					}
				}

				break;
			}
		}
		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
	}


	//判断树是否为平衡二叉树
	bool IsBalance()
	{
		return _IsBalance(_root);
	}


protected:
	//右单旋
	void RotateR(Node* parent)
	{
		Node* SubL = parent->_left;
		Node* SubLR = SubL->_right;

		//将SubL的右孩子摘下放到父亲节点的左树上
		parent->_left = SubLR;
		//防止SubL节点为空
		if(SubLR)
			SubLR->_parent = parent;

		//将SubL变成父节点
		Node* ppNode = parent->_parent;
		SubL->_right = parent;
		parent->_parent = SubL;
		//将Subl连接到父节点的
		SubL->_parent = ppNode;
		if (ppNode == NULL)
		{
			_root = SubL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = SubL;
			}
			else
			{
				ppNode->_right = SubL;
			}
		}

		//设置平衡因子
		SubL->_bf = parent->_bf = 0;
	}

	//左单旋
	void RotateL(Node* parent)
	{
		Node* SubR = parent->_right;
		Node* SubRL = SubR->_left;

		parent->_right = SubRL;
		if (SubRL)
			SubRL->_parent = parent;

		Node* ppNode = parent->_parent;
		SubR->_left = parent;
		parent->_parent = SubR;
		SubR->_parent = ppNode;

		if (ppNode == NULL)
		{
			_root = SubR;
		}
		else
		{
			if (parent == ppNode->_left)
				ppNode->_left = SubR;
			else
				ppNode->_right = SubR;
		}

		SubR->_bf = parent->_bf = 0;
	}

	//左右双旋
	void RotateLR(Node* parent)
	{
		Node* SubL = parent->_left;
		Node* SubLR = SubL->_right;
		int bf = SubLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		//根据SubLR来调整旋转因子 
		if (bf == -1)
		{
			SubL->_bf = 0;
			parent->_bf = 1;
		}
		else if (bf == 1)
		{
			SubL->_bf = -1;
			parent->_bf = 0;
		}
	}
	//右左双旋
	void RotateRL(Node* parent)
	{
		Node* SubR = parent->_right;
		Node* SubRL = SubR->_left;
		int bf = SubRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);

		//根据SubRL来调整旋转因子 
		if (bf == -1)
		{
			parent->_bf = 0;
			SubR->_bf = 1;
		}
		else if (bf == 1)
		{
			parent->_bf = -1;
			SubR->_bf = 0;
		}
	}


	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_key<< " ";
		_InOrder(root->_right);
	}

	//求树的高度
	int _Height(Node* root)
	{
		if (root == NULL)
			return 0;

		int left = _Height(root->_left);
		int right = _Height(root->_right);

		return left > right ? left + 1 : right + 1;
	}

	//判断二叉树是否为平衡二叉树
	bool _IsBalance(Node* root)
	{
		if (root == NULL)
			return true;

		int left = _Height(root->_left);
		int right = _Height(root->_right);

		if ((right - left != root->_bf) || (abs(right - left) > 1))
		{
			cout << root->_key << "节点异常" << endl;
			return false;
		}

		return _IsBalance(root->_left) && _IsBalance(root->_right);
	}


	//删除每个节点
	void _Destory(Node* root)
	{
		if (root == NULL)
			return;

		_Destory(root->_left);
		_Destory(root->_right);

		delete root;
	}

private:
	Node* _root;
};

void TestTree()
{
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };

	AVLTree<int, int> t;

	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		t.Insert(a[i], i);
	}
	t.InOrder();
	cout << endl;

	cout << "是否平衡?" << t.IsBalance() << endl;
}

void TestTree_SP()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;

	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		t.Insert(a[i], i);
	}

	t.InOrder();
	cout << endl;

	cout << "是否平衡?" << t.IsBalance() << endl;
}