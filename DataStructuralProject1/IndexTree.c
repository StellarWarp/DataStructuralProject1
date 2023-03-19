#include "IndexTree.h"

#define T PathInfo
#include "ArrayCore.h"

#include "TypedDefineBegin.h"

#define T pNode
#include "ArrayTemplate.h"
#include "ArrayCore.h"

//只能是偶数
#define M 10

//参数
const int min_key = ((M % 2) ? M / 2 + 1 : M / 2);//m/2向上取整
const int max_key = (M - 1);
#define min_i (min_key - 1)
#define max_i (max_key - 1)

//结构


Page* PageInit(DT data)
{
	Page* page = MALLOC(Page, 1);
	page->data = data;
	page->next = NULL;
	return page;
}

void PageAdd(Page* enter, Page* page)
{
	CHECK(if (enter == NULL) ETNR(Error, "Enter null"));
	Page* p = enter;
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = page;
}
//是否删除此索引
bool PageDelete(Page** enter, Page* deletePage)
{
	Page* p = *enter;
	//删除全部
	if (deletePage == NULL)
	{
		while (p)
		{
			Page* next = p->next;
			free(p);
			p = next;
		}
		return true;
	}
	if (p == deletePage)//首页删除
	{
		*enter = p->next;
		free(deletePage);
		if (*enter == NULL) return true;
		return false;
	}
	while (p->next != deletePage)
	{
		p = p->next;
		if (!p) ET(Error, "PageDelete:Page not found", false);
	}
	Page* del = p->next;
	p->next = del->next;
	free(del);
	return false;
}

DT GetData(Page* page)
{
	return page->data;
}

void PagePrint(WCHAR* buffer, size_t count, Page* page, 
	void print(WCHAR* buffer, size_t count, DT data))
{
	buffer[0] = 0;
	WCHAR buffer1[100] = { 0 };
	while (page != NULL)
	{
		//swprintf(buffer1, 100, L"%X ", page->data);
		print(buffer1, 100, page->data);
		wcscat_s(buffer, count, buffer1);
		page = page->next;
	}
}

struct LayerData;
struct LayerN;
//typedef union pNode pNode;
//typedef union pNode//节点间指针
//{
//	struct LayerData* Dp;
//	struct LayerN* Np;
//	void* any;
//}pNode;
typedef struct LayerData//子叶节点
{
	uint16_t num;
	KT* key;//max_key
	PT* dataPointer;//max_key
	struct LayerData* next;
}LayerData;
LayerData* LayerDataInit()
{
	LayerData* ld = malloc(sizeof(LayerData));
	ld->key = MALLOC(KT, max_key);
	ld->dataPointer = MALLOC(PT, max_key);
	ld->next = NULL;
	ld->num = 0;
	return ld;
}
void LayerDataRelease(LayerData* ld)
{
	free(ld->key);
	free(ld->dataPointer);
	free(ld);
}
typedef struct LayerN
{
	uint16_t num;
	KT* key;//max_key
	union pNode* ptr;//M
}LayerN;
LayerN* LayerNInit()
{
	LayerN* ln = malloc(sizeof(LayerN));
	ln->key = MALLOC(KT, max_key);
	ln->ptr = MALLOC(union pNode, max_key);
	ln->num = 0;
	return ln;
}
void LayerNRelease(LayerN* ln)
{
	free(ln->key);
	free(ln->ptr);
	free(ln);
}

//函数
//子叶节点
#define _class_ LayerData
#define key_ self->key
#define dataPointer_ self->dataPointer
#define next_ self->next
#define num_ self->num

bool LDfull(_class_* self)
{
	return num_ == max_key;
}
bool LDlow(_class_* self)
{
	return num_ < min_key;
}
bool LDmore(_class_* self)
{
	return num_ > min_key;
}
bool LDinsert(_class_* self, PT DP, KT value, int index)
{
	//int in = BinarySearch(KT, key_, num_, value);
	//int in = DilitarySearch_SecondOrder_down(Node.key, Node.dataPointer, 0, Node.num - 1, value, DP);
	//if (key_[in] == value && dataPointer_[in] < DP)//同值
	//{
	//	in++;
	//}
	NativeArrayInsert(key_, max_key, index, value);
	NativeArrayInsert(dataPointer_, max_key, index, DP);
	num_++;
	if (index == num_ - 1) return true;//需要更新索引
	else return false;
}
pNode LDdivide(_class_* self, PT DP, KT value)
{
	LayerData* NewNode = LayerDataInit();
	int in = BinarySearch(KT, key_, num_, value);
	//int in = DilitarySearch_SecondOrder_down(Node.key, Node.dataPointer, 0, max_i, value, DP);
	//if (Node.key[in] == value && Node.dataPointer[in] < DP)
	//{
	//	in++;
	//}
	num_ = M / 2;
	NewNode->num = min_key;
	if (in <= min_i)//插入位置在前
	{
		//将原节点多出的移动到新的节点
		NativeArrayMove(key_, min_i, max_i, NewNode->key, 0);
		NativeArrayMove(dataPointer_, min_i, max_i, NewNode->dataPointer, 0);
		NativeArrayInsert(key_, min_key, in, value);
		NativeArrayInsert(dataPointer_, min_key, in, DP);
	}
	else//插入位置在后
	{
		NativeArrayMove(key_, min_i + 1, max_i, NewNode->key, 0);
		NativeArrayMove(dataPointer_, min_i + 1, max_i, NewNode->dataPointer, 0);
		NativeArrayInsert(NewNode->key, max_key, in - min_key, value);
		NativeArrayInsert(NewNode->dataPointer, max_key, in - min_key, DP);
	}
	NewNode->next = next_;
	next_ = NewNode;
	pNode ptr = { NewNode };
	return ptr;
}
//return if deleted
bool LDdelete(_class_* self, IndexTree* tree, KT value, PT DP, int* index)
{
	int del = BinarySearch(KT, key_, num_, value);
	*index = del;
	//int del = DilitarySearch_SecondOrder_down(Node.key, Node.dataPointer, 0, Node.num, value, DP);
	//DeleteArray(Node.key, del, Node.num, (KT)0);
	//DeleteArray(Node.dataPointer, del, Node.num, (PT)0);
	CHECK(if (key_[del] != value) ET(Error, "Key do not exist", false));
	//同值
	if (PageDelete(&dataPointer_[del], DP))
	{
		NativeArrayRemove(key_, num_, del);
		NativeArrayRemove(dataPointer_, num_, del);
		num_--;
		return true;
	}
	return false;
}
void LDshift(LayerData* Node1, int a1, int b1, LayerData* Node2, int a2)
{
	NativeArrayMove(Node1->key, a1, b1, Node2->key, a2);
	NativeArrayMove(Node1->dataPointer, a1, b1, Node2->dataPointer, a2);
	int num = b1 - a1 + 1;
	Node1->num -= num;
	Node2->num += num;
}

//函数
//子节点
#define _class_ LayerN
#define key_ self->key
#define ptr_ self->ptr
#define num_ self->num

bool LNfull(_class_* self)
{
	return num_ == max_key;
}
bool LNlow(_class_* self)
{
	return num_ < min_key;
}
bool LNmore(_class_* self)
{
	return num_ > min_key;
}
LayerN* newLayer(pNode P1, KT value1, pNode P2, KT value2)
{
	LayerN* NewNode = LayerNInit();
	NewNode->key[0] = value1;
	NewNode->key[1] = value2;
	NewNode->ptr[0] = P1;
	NewNode->ptr[1] = P2;
	NewNode->num = 2;
	return NewNode;
}
//返回是否最大值
bool LNinsert(_class_* self, pNode P, KT value)
{
	int in = BinarySearch(KT, key_, num_, value);
	//int in = DilitarySearch_SecondOrder_down(Node.key, Node.dataPointer, 0, Node.num - 1, value, DP);
	//if (Node.key[in] == value && Node.dataPointer[in] < DP)
	//{
	//	in++;
	//}
	NativeArrayInsert(key_, max_key, in, value);
	NativeArrayInsert(ptr_, max_key, in, P);
	//InsertArray(Node.key, in, max_i, value);
	//InsertArray(Node.NodePointer, in, max_i, P);
	//InsertArray(Node.dataPointer, in, max_i, DP);
	num_++;
	if (in == num_ - 1) return 1;
	else return 0;
}
//更新并回溯
bool LNupdate(LayerN* Node, IndexTree* tree, int i, KT value)
{
	if (i == Node->num - 1)
	{
		Node->key[i] = value;
		if (!StackEmpty(PathInfo, tree->path))
		{

			PathInfo path = StackTop(PathInfo, tree->path);
			StackPop(PathInfo, tree->path);
			LNupdate(path.Node, tree, path.index, value);
		}
		return 1;
	}
	else
	{
		Node->key[i] = value;
		return 0;
	}
}
//不回溯
void LNupdate0(LayerN* Node, int i, KT value)
{
	Node->key[i] = value;
}
//分裂并增加
pNode LNdivide(_class_* self, pNode P, KT value)
{
	LayerN* NewNode = LayerNInit();
	int in = BinarySearch(KT, key_, num_, value);
	//int in = DilitarySearch_SecondOrder_down(Node.key, Node.dataPointer, 0, max_i, value, DP);
	//if (Node.key[in] == value && Node.dataPointer[in] < DP)//同值
	//{
	//	in++;
	//}
	num_ = M / 2;
	NewNode->num = min_key;
	if (in <= min_i)
	{
		NativeArrayMove(key_, min_i, max_i, NewNode->key, 0);
		NativeArrayMove(ptr_, min_i, max_i, NewNode->ptr, 0);
		NativeArrayInsert(key_, min_key, in, value);
		NativeArrayInsert(ptr_, min_key, in, P);
		//MoveArray(Node.key, min_i, max_i, NewNode->key);//将原节点多出的移动到新的节点
		//MoveArray(Node.NodePointer, min_i, max_i, NewNode->NodePointer);
		//MoveArray(Node.dataPointer, min_i, max_i, NewNode->dataPointer);
		//SetArray(Node.key, min_i, max_i, KT());//初始值
		//SetArray(Node.NodePointer, min_i, max_i, pNode());
		//SetArray(Node.dataPointer, min_i, max_i, PT());
		//InsertArray(Node.key, in, min_i, value);
		//InsertArray(Node.NodePointer, in, min_i, P);
		//InsertArray(Node.dataPointer, in, min_i, DP);
	}
	else
	{
		NativeArrayMove(key_, min_key, max_i, NewNode->key, 0);
		NativeArrayMove(ptr_, min_key, max_i, NewNode->ptr, 0);
		NativeArrayInsert(NewNode->key, min_key, in - min_key, value);
		NativeArrayInsert(NewNode->ptr, min_key, in - min_key, P);
		//MoveArray(Node.key, min_key, max_i, NewNode->key);
		//MoveArray(Node.NodePointer, min_key, max_i, NewNode->NodePointer);
		//MoveArray(Node.dataPointer, min_key, max_i, NewNode->dataPointer);
		//SetArray(Node.key, min_key, max_i, KT());
		//SetArray(Node.NodePointer, min_key, max_i, pNode());
		//SetArray(Node.dataPointer, min_key, max_i, PT());
		//InsertArray(NewNode->key, in - min_key, min_i, value);
		//InsertArray(NewNode->NodePointer, in - min_key, min_i, P);
		//InsertArray(NewNode->dataPointer, in - min_key, min_i, DP);
	}
	pNode ptr = { NewNode };
	return ptr;
}
//删除并返回删除index
int LNdelete(_class_* self, KT value)
{
	int del = BinarySearch(KT, key_, num_, value);
	NativeArrayRemove(key_, num_, del);
	NativeArrayRemove(ptr_, num_, del);
	num_--;
	//int del = DilitarySearch_SecondOrder_down(Node.key, Node.dataPointer, 0, Node.num, value, DP);
	//DeleteArray(Node.key, del, Node.num, (KT)0);
	//DeleteArray(Node.NodePointer, del, Node.num, Empty);
	//DeleteArray(Node.dataPointer, del, Node.num, (PT)0);
	return del;
}
//删除index
void LNdeletei(_class_* self, int i)
{
	int del = i;
	NativeArrayRemove(key_, num_, del);
	NativeArrayRemove(ptr_, num_, del);
	num_--;
	/*DeleteArray(Node.key, del, Node.num, (KT)0);*/
	//pNode Empty;
	//DeleteArray(Node.NodePointer, del, Node.num, Empty);
	//DeleteArray(Node.dataPointer, del, Node.num, (PT)0);
}
//将1移到2
void LNshift(LayerN* Node1, int a1, int b1, LayerN* Node2, int a2)
{
	NativeArrayMove(Node1->key, a1, b1, Node2->key, a2);
	NativeArrayMove(Node1->ptr, a1, b1, Node2->ptr, a2);
	//MoveArray(Node1.key, a1, b1, Node2.key, a2);
	//MoveArray(Node1.NodePointer, a1, b1, Node2.NodePointer, a2);
	//MoveArray(Node1.dataPointer, a1, b1, Node2.dataPointer, a2);
	int num = b1 - a1 + 1;
	Node1->num -= num;
	Node2->num += num;
}

#undef key_
#undef ptr_
#undef num_
#undef next_

#define _class_ IndexTree
#define root_ self->root
#define height_ self->height
#define path_ self->path

IndexTree IndexTreeInit()
{
	IndexTree self;
	self.root.any = NULL;
	self.height = 0;
	self.path = StackInit(PathInfo, 4, false);
	self.count = 0;
	return self;
}

void IndexTreeRelease(_class_* self)
{
	Stack(pNode) stack = StackInit(pNode, 4, false);
	Stack(int) depthStack = StackInit(int, 4, false);

	StackPush(pNode, stack, root_);
	StackPush(int, depthStack, 0);

	while (stack->m_size > 0)
	{
		pNode node = StackTop(pNode, stack);
		int depth = StackTop(int, depthStack);
		StackPop(pNode, stack);
		StackPop(int, depthStack);
		if (node.any == NULL) continue;

		//next layer
		if (depth < height_ - 1)
		{
			for (int i = node.Np->num - 1; i >= 0; i--)
			{
				StackPush(int, depthStack, depth + 1);
				StackPush(pNode, stack, node.Np->ptr[i]);
			}
		}

		//release
		if (depth == height_ - 1)
		{
			for (int i = 0; i < node.Dp->num; i++)
			{
				if (node.Dp->dataPointer[i])
				{
					free(node.Dp->dataPointer[i]->data);
					PageDelete(&node.Dp->dataPointer[i], NULL);
				}
			}
			LayerDataRelease(node.Dp);
		}
		else
		{
			LayerNRelease(node.Np);
		}

	}

	StackRelease(PathInfo, path_);
	StackRelease(pNode, stack);
}

//树函数
LayerData* FIndLayerData(_class_* self, KT key)
{
	StackClear(PathInfo, path_);
	pNode p = root_;
	int h = height_;
	if (h > 1)
	{
		KT maxkey = root_.Np->key[root_.Np->num - 1];
		if (key > maxkey)
		{
			key = maxkey;
			//PT maxPD = root->Np->dataPointer[root->Np->num - 1];
			//if (DP > maxPD)
			//{
			//	DP = maxPD;
			//}
		}
	}
	while (h > 1)
	{
		PathInfo path;
		path.Node = p.Np;
		//path.IN(p.Np);
		int i = BinarySearch(KT, p.Np->key, p.Np->num, key);
		//int i = DilitarySearch_SecondOrder_down(p.Np->key, p.Np->dataPointer, 0, p.Np->num - 1, key, DP);//建立的时候出错//第二指标排序错误
		p = p.Np->ptr[i];
		//Pathi.IN(i);
		path.index = i;
		StackPush(PathInfo, path_, path);
		h--;
	}
	return p.Dp;
};
void LayerNAdd(_class_* self, pNode P, KT key)
{
	PathInfo path = StackTop(PathInfo, path_);
	LayerN* CLayer = path.Node;
	StackPop(PathInfo, path_);
	if (LNfull(CLayer))
	{
		pNode NLayer = LNdivide(CLayer, P, key);
		if (root_.Np == CLayer)
		{
			pNode C = { CLayer };
			//free(root_);
			root_.Np = newLayer(C, CLayer->key[min_i],
				NLayer, NLayer.Np->key[min_i]);
			/*root_ = new pNode(newLayer(C, CLayer->key[min_i], CLayer->dataPointer[min_i],
				NLayer, NLayer.Np->key[min_i], NLayer.Np->dataPointer[min_i]));*/
			height_++;
		}
		else
		{
			path = StackTop(PathInfo, path_);
			LNupdate0(path.Node, path.index, CLayer->key[min_i]);
			LayerNAdd(self, NLayer, NLayer.Np->key[min_i]);
		}
	}
	else
	{
		if (LNinsert(CLayer, P, key))
		{
			//更新key的函数
			if (!StackEmpty(PathInfo, path_))
			{
				path = StackTop(PathInfo, path_);
				StackPop(PathInfo, path_);
				LNupdate(path.Node, self, path.index, key);
				//update(path.OUT(), Pathi.OUT(), key, DP);
			}
		}
	}
}
void LayerDataAdd(_class_* self, PT dataPointer, KT key)
{
	if (height_ == 0)
	{
		/*root_ = MALLOC(pNode, 1);*/
		root_.Dp = LayerDataInit();
		LDinsert(root_.Dp, dataPointer, key, 0);
		//root = new pNode;
		//root->Dp = new LayerData;
		//Insert(*(root->Dp), dataPointer, key);
		height_++;
	}
	else
	{
		LayerData* CLayer = FIndLayerData(self, key);
		int in = BinarySearch(KT, CLayer->key, CLayer->num, key);
		if (CLayer->key[in] == key && in < CLayer->num)//同值
		{
			PageAdd(CLayer->dataPointer[in], dataPointer);
			//ET(Error, "Key is already in the tree",);
			return;
		}

		//insert
		if (LDfull(CLayer))
		{
			pNode NLayer = LDdivide(CLayer, dataPointer, key);
			if (height_ == 1)
			{
				pNode C = { CLayer };
				//free(root_);
				root_.Np = newLayer(C, CLayer->key[min_i],
					NLayer, NLayer.Dp->key[min_i]);
				//root_ = new pNode(newLayer(C, CLayer->key[min_i], CLayer->dataPointer[min_i],
				//	NLayer, NLayer.Dp->key[min_i], NLayer.Dp->dataPointer[min_i]));
				height_++;
			}
			else
			{
				PathInfo path = StackTop(PathInfo, path_);
				LNupdate0(path.Node, path.index, CLayer->key[min_i]);
				LayerNAdd(self, NLayer, NLayer.Dp->key[min_i]);
				//update0(path.Read0(), Pathi.Read0(), CLayer->key[min_i], CLayer->dataPointer[min_i]);
				//LayerNAdd(NLayer, NLayer.Dp->key[min_i], NLayer.Dp->dataPointer[min_i]);
			}
		}
		else
		{
			if (LDinsert(CLayer, dataPointer, key, in))
			{
				//更新key的函数
				if (!StackEmpty(PathInfo, path_))
				{
					PathInfo path = StackTop(PathInfo, path_);
					StackPop(PathInfo, path_);
					LNupdate(path.Node, self, path.index, key);
					//update(path.OUT(), Pathi.OUT(), key, DP);
				}
				//if (path.Nempty())
				//{
				//	update(path.OUT(), Pathi.OUT(), key, dataPointer);
				//}
			}
		}

	}
}

//删除
void LayerNRemove(_class_* self, int del)
{
	PathInfo path = StackTop(PathInfo, path_);
	LayerN* CLayer = path.Node;
	StackPop(PathInfo, path_);
	//LayerN* CLayer = *path.OUT();
	//Pathi.OUT();
	KT keymax = CLayer->key[min_i];
	//PT DPmax = CLayer->dataPointer[min_i];
	LNdeletei(CLayer, del);
	if (!StackEmpty(PathInfo, path_))
	{
		if (CLayer->num < min_key)
		{
			path = StackTop(PathInfo, path_);
			LayerN* FNode = path.Node;
			int Ci = path.index;
			LayerN* BNode;
			int Bi;
			if (Ci == 0)
			{//右 ->
				Bi = Ci + 1;
				BNode = FNode->ptr[Bi].Np;
				if (LNmore(BNode))//借用
				{
					int i = 0;
					LNshift(BNode, i, i, CLayer, min_i);
					LNshift(BNode, 1, BNode->num, BNode, 0);
					LNupdate0(FNode, Ci, CLayer->key[min_i]);
					StackPop(PathInfo, path_);
					//update0(path.OUT(), Pathi.OUT(), CLayer->key[min_i], CLayer->dataPointer[min_i]);
				}
				else//合并
				{
					LNshift(BNode, 0, min_i, CLayer, min_i);
					LNupdate0(FNode, Ci, CLayer->key[max_i]);
					LayerNRemove(self, Bi);
					LayerNRelease(BNode);
				}
			}
			else
			{//左 <-
				Bi = Ci - 1;
				BNode = FNode->ptr[Bi].Np;
				if (LNmore(BNode))//借用
				{
					int i = --BNode->num;//数量减一
					LNinsert(CLayer, BNode->ptr[i], BNode->key[i]);
					LNupdate0(FNode, Bi, BNode->key[i - 1]);
				}
				else//合并
				{
					LNshift(CLayer, 0, min_i - 1, BNode, min_i + 1);
					LNupdate0(FNode, Bi, BNode->key[max_i]);
					LayerNRemove(self, Ci);
					LayerNRelease(CLayer);
					//delete& CLayer;
				}
			}
		}
		else if (del == CLayer->num)
		{
			path = StackTop(PathInfo, path_);
			StackPop(PathInfo, path_);
			LNupdate(path.Node, self, path.index, CLayer->key[CLayer->num - 1]);
			//update(path.OUT(), Pathi.OUT(), CLayer->key[CLayer->num - 1], CLayer->dataPointer[CLayer->num - 1]);
		}
	}
	else if (CLayer->num == 1)
	{
		pNode newRoot = { CLayer->ptr[0].any };
		LayerNRelease(root_.Np);
		//free(root_);
		//pNode* NewMaster = new pNode(CLayer->ptr[0]);
		//delete root->Np;
		//delete root;
		root_ = newRoot;
		height_--;
	}

}
void LayerDataRemove(_class_* self, PT dataPointer, KT key)
{
	LayerData* CLayer = FIndLayerData(self, key);
	KT keymax = CLayer->key[min_i];
	PT DPmax = CLayer->dataPointer[min_i];
	int del;
	if (LDdelete(CLayer, self, key, dataPointer, &del) == false) return;
	if (!StackEmpty(PathInfo, path_))
	{
		if (CLayer->num < min_key)
		{
			PathInfo path = StackTop(PathInfo, path_);
			LayerN* FNode = path.Node;
			int Ci = path.index;
			LayerData* BNode;
			int Bi;
			if (Ci == 0)
			{//右 ->
				Bi = Ci + 1;
				BNode = FNode->ptr[Bi].Dp;
				if (LDmore(BNode))//借用
				{
					int i = 0;
					LDshift(BNode, i, i, CLayer, min_i);
					LDshift(BNode, 1, BNode->num, BNode, 0);
					//LNupdate0(path.OUT(), Pathi.OUT(), CLayer->key[min_i], CLayer->dataPointer[min_i]);
					LNupdate0(FNode, Ci, CLayer->key[min_i]);
					StackPop(PathInfo, path_);
				}
				else//合并
				{
					LDshift(BNode, 0, min_i, CLayer, min_i);
					LNupdate0(FNode, Ci, CLayer->key[max_i]);
					LayerNRemove(self, Bi);
					CLayer->next = BNode->next;
					LayerDataRelease(BNode);
				}
			}
			else
			{//左 <-
				Bi = Ci - 1;
				BNode = FNode->ptr[Bi].Dp;
				if (LDmore(BNode))//借用
				{
					int i = --BNode->num;
					LDinsert(CLayer, BNode->dataPointer[i], BNode->key[i], 0);
					LNupdate0(FNode, Bi, BNode->key[i - 1]);
				}
				else//合并
				{
					LDshift(CLayer, 0, min_i - 1, BNode, min_i + 1);
					LNupdate0(FNode, Bi, BNode->key[max_i]);
					LayerNRemove(self, Ci);
					BNode->next = CLayer->next;
					LayerDataRelease(CLayer);
				}
			}
		}
		else if (del == CLayer->num)
		{
			PathInfo path = StackTop(PathInfo, path_);
			StackPop(PathInfo, path_);
			LNupdate(path.Node, self, path.index, CLayer->key[CLayer->num - 1]);
		}
	}
}

void IndexTreeAdd(_class_* self, KT key, DT val)
{
	self->count++;//for not same value
	LayerDataAdd(self, PageInit(val), key);
}
//set del_page NULL to delete all page
void IndexTreeRemove(_class_* self, KT key, PT del_page)
{
	self->count--;//for not same value
	LayerDataRemove(self, NULL, key);
}

PT IndexTreeFind(_class_* self, KT key)
{
	LayerData* CLayer = FIndLayerData(self, key);
	if (CLayer == NULL) return NULL;
	int index = BinarySearch(KT, CLayer->key, CLayer->num, key);
	if (CLayer->key[index] != key) return NULL;
	//CHECK(if (CLayer->key[index] != key) ET(Error, "Key do not exist", NULL));
	return CLayer->dataPointer[index];
}

//void IndexTreePrint(IndexTree* tree, COORD pos)
//{
//	int depth = 0;
//	int Xshift = pos.X;
//	int shift = 20;
//	WCHAR* branchStr = MALLOC(WCHAR, shift + 1);
//	branchStr[0] = L'├';
//	for (int i = 1; i < shift; i++) branchStr[i] = L'─';
//	console.Print(pos, (WCHAR*)L"Tree: ");
//	Stack(pNode) stack = StackInit(pNode, 4, false);
//	Stack(int) depthStack = StackInit(int, 4, false);
//	StackPush(pNode, stack, tree->root);
//	StackPush(int, depthStack, depth);
//
//	Stack(int) BLStack = StackInit(int, 4, false);//line
//	Stack(int) BDStack = StackInit(int, 4, false);//depth
//
//	//先序遍历
//	while (!StackEmpty(pNode, stack))
//	{
//		pNode node = StackTop(pNode, stack);
//		depth = StackTop(int, depthStack);
//		StackPop(pNode, stack);
//		StackPop(int, depthStack);
//		printf("\n");
//		//for (int i = 0; i < (depth - 1) * shift; i++) printf(" ");
//		//if (node == NULL) continue;
//		//print├└ ─ │
//		pos.X = depth * shift + Xshift;
//		pos.Y += 1;
//
//		//branch print
//		if (!StackEmpty(int, BDStack))
//		{
//			while (StackTop(int, BDStack) != depth)
//			{
//				COORD pos1 = { (StackTop(int,BDStack) - 1) * shift + Xshift, StackTop(int,BLStack) };
//				pos1.Y--;
//				console.Print(pos1, (WCHAR*)L"└");
//				StackPop(int, BDStack);
//				StackPop(int, BLStack);
//			}
//			COORD pos1 = { (depth - 1) * shift + Xshift, StackTop(int,BLStack) };
//			while (pos1.Y < pos.Y)
//			{
//				console.Print(pos1, (WCHAR*)L"│");
//				pos1.Y += 1;
//			}
//			console.Print(pos1, branchStr);//├─────
//			StackTop(int, BLStack) = pos1.Y + 1;
//
//		}
//		if (node.any == NULL) continue;
//
//		//context print
//		if (node.any)
//		{
//			String str = StringInit();
//			WCHAR* buffer = MALLOC(WCHAR, 100);
//			//print key array
//			for (size_t i = 0; i < node.Np->num; i++)
//			{
//				swprintf(buffer, 100, L"%lld ", node.Np->key[i]);
//				StringExtend(&str, buffer);
//			}
//			console.SetTextColor(FORE_CYAN);
//			console.Print(pos, str.data);
//			//print dataPointer array
//			if (depth == tree->height - 1)
//			{
//				pos.X += str.len;
//				StringClear(&str);
//				for (size_t i = 0; i < node.Np->num; i++)
//				{
//					PagePrint(buffer, 100, node.Dp->dataPointer[i]);
//					StringExtend(&str, buffer);
//				}
//				console.SetTextColor(FORE_BLUE);
//				console.Print(pos, str.data);
//			}
//			console.SetTextColor(FORE_WHITE);
//			StringRelease(&str);
//			free(buffer);
//		}
//		else
//			console.Print(pos, (WCHAR*)L"*");
//
//		//next layer
//		depth += 1;
//		if (depth >= tree->height) continue;
//		StackPush(int, BLStack, pos.Y + 1);
//		StackPush(int, BDStack, depth);
//		for (int i = node.Np->num - 1; i >= 0; i--)
//		{
//			StackPush(int, depthStack, depth);
//			StackPush(pNode, stack, node.Np->ptr[i]);
//		}
//	}
//
//	while (!StackEmpty(int, BDStack))
//	{
//		COORD pos1 = { (StackTop(int,BDStack) - 1) * shift + Xshift, StackTop(int,BLStack) };
//		pos1.Y--;
//		console.Print(pos1, (WCHAR*)L"└");
//		StackPop(int, BDStack);
//		StackPop(int, BLStack);
//	}
//	pos.X = Xshift;
//	pos.Y++;
//	console.Print(pos, (WCHAR*)L"End Tree");
//
//	StackRelease(pNode, stack);
//	StackRelease(int, depthStack);
//	StackRelease(int, BLStack);
//	StackRelease(int, BDStack);
//	free(branchStr);
//}

char branch1[] = "├─────";
char branch2[] = "└─────";
char layer1[] = "│     ";
char layer2[] = "      ";

void PrintNode(pNode node, int height, Stack(pNode) path, Stack(int) count_stack,
	void print(WCHAR* buffer, size_t count, DT data))
{
	AutoMallocBegin();
	//context print
	if (node.any)
	{
		String str = StringInit();
		WCHAR* buffer = AUTO_MALLOC(WCHAR, 100);
		//print key array
		for (int i = 0; i < (int)path->m_size - 1; i++)
		{
			if (count_stack->m_data[i] < path->m_data[i].Np->num - 1)
				printf(layer1);
			else
				printf(layer2);
		}
		if (path->m_size != 0)
			if (StackTop(pNode, path).Dp->num - 1 == StackTop(int, count_stack))
				printf(branch2);
			else
				printf(branch1);
		for (size_t i = 0; i < node.Np->num; i++)
		{
			swprintf(buffer, 100, L"%lld ", node.Np->key[i]);
			StringExtend(&str, buffer);
		}
		console.SetTextColor(FORE_CYAN);
		wprintf(str.data);
		//print dataPointer array
		if (path->m_size == height - 1)
		{
			StringClear(&str);
			for (size_t i = 0; i < node.Np->num; i++)
			{
				PagePrint(buffer, 100, node.Dp->dataPointer[i],print);
				StringExtend(&str, buffer);
			}
			console.SetTextColor(FORE_BLUE);
			wprintf(str.data);
		}
		console.SetTextColor(FORE_WHITE);
		StringRelease(&str);
	}
	else
		printf("*");
	printf("\n");
	AutoMallocEnd();
}

void IndexTreePrint(IndexTree* tree, void print(WCHAR* buffer, size_t count, DT data))
{
	AutoMallocBegin();
	//int shift = 20;
	printf("Tree: \n");
	Stack(pNode) path = StackInit(pNode, 4, true);
	Stack(int) count_stack = StackInit(int, 4, true);
	pNode node = tree->root;
	if (tree->root.any == NULL) goto end;
	//先序遍历
	while (true)
	{
		PrintNode(node, tree->height, path, count_stack, print);
		if (path->m_size < tree->height - 1)
		{
			StackPush(pNode, path, node);
			StackPush(int, count_stack, 0);
			node = node.Np->ptr[0];
		}
		else//leaves node
		{
			if (StackEmpty(pNode, path)) goto end;
			while (StackTop(int, count_stack)
				== StackTop(pNode, path).Np->num - 1)
			{
				StackPop(pNode, path);
				StackPop(int, count_stack);
				if (StackEmpty(pNode, path)) goto end;//
			}
			node = StackTop(pNode, path);
			StackTop(int, count_stack) += 1;
			node = node.Np->ptr[StackTop(int, count_stack)];
		}

	}
end:
	printf("End Tree");

	AutoMallocEnd();
}
