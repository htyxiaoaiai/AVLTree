#pragma once
#include<iostream>

using namespace std;

template<class K,class V>
struct AVLTreeNode
{
public:
	//���캯��
	AVLTreeNode(const K& key, const V& value)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _key(key)
		, _value(value)
		,_bf(0)
	{}

public:
	AVLTreeNode<K, V>* _left;//����
	AVLTreeNode<K, V>* _right;//�Һ���
	AVLTreeNode<K, V>* _parent;//���׽ڵ�

	K _key; //�ؼ���
	V _value; //��ֵ
	int _bf; //ƽ�����ӣ��������ĸ߶ȼ�ȥ�������ĸ߶ȣ�
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

		//�ҵ�Ӧ�ò���Ľڵ�
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

		//���ڵ����
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

		//�ж����Ƿ�ƽ��
		//����ƽ�����ӣ�������ʱ������ת
		while (parent)
		{
			//����ƽ������
			if (cur == parent->_left)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}
			//֮ǰ����ƽ��ģ��ٲ���һ��û�иı����ĸ߶ȣ����Կ϶�����ƽ����
			if (parent->_bf == 0)
			{
				break;
			}
			//����һ���ڵ�֮������ƽ��ı������ĸ߶ȣ����Ǹ��ڵ㲻һ��ƽ��
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				cur = parent;
				parent = cur->_parent;
			}
			else//2  -2 
			{
				if (cur->_bf == 1)
				{
					//����
					if (parent->_bf == 2)
					{
						RotateL(parent);
					}
					//����˫��
					else
					{
						RotateLR(parent);
					}
				}
				else
				{
					//����˫��
					if (parent->_bf == 2)
					{
						RotateRL(parent);
					}
					//�ҵ���
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


	//�ж����Ƿ�Ϊƽ�������
	bool IsBalance()
	{
		return _IsBalance(_root);
	}


protected:
	//�ҵ���
	void RotateR(Node* parent)
	{
		Node* SubL = parent->_left;
		Node* SubLR = SubL->_right;

		//��SubL���Һ���ժ�·ŵ����׽ڵ��������
		parent->_left = SubLR;
		//��ֹSubL�ڵ�Ϊ��
		if(SubLR)
			SubLR->_parent = parent;

		//��SubL��ɸ��ڵ�
		Node* ppNode = parent->_parent;
		SubL->_right = parent;
		parent->_parent = SubL;
		//��Subl���ӵ����ڵ��
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

		//����ƽ������
		SubL->_bf = parent->_bf = 0;
	}

	//����
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

	//����˫��
	void RotateLR(Node* parent)
	{
		Node* SubL = parent->_left;
		Node* SubLR = SubL->_right;
		int bf = SubLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		//����SubLR��������ת���� 
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
	//����˫��
	void RotateRL(Node* parent)
	{
		Node* SubR = parent->_right;
		Node* SubRL = SubR->_left;
		int bf = SubRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);

		//����SubRL��������ת���� 
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

	//�����ĸ߶�
	int _Height(Node* root)
	{
		if (root == NULL)
			return 0;

		int left = _Height(root->_left);
		int right = _Height(root->_right);

		return left > right ? left + 1 : right + 1;
	}

	//�ж϶������Ƿ�Ϊƽ�������
	bool _IsBalance(Node* root)
	{
		if (root == NULL)
			return true;

		int left = _Height(root->_left);
		int right = _Height(root->_right);

		if ((right - left != root->_bf) || (abs(right - left) > 1))
		{
			cout << root->_key << "�ڵ��쳣" << endl;
			return false;
		}

		return _IsBalance(root->_left) && _IsBalance(root->_right);
	}


	//ɾ��ÿ���ڵ�
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

	cout << "�Ƿ�ƽ��?" << t.IsBalance() << endl;
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

	cout << "�Ƿ�ƽ��?" << t.IsBalance() << endl;
}