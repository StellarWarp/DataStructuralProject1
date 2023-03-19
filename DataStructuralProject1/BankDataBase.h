#pragma once


void DataBaseInit(IndexTree* DataBase)
{
	*DataBase = IndexTreeInit();
	AutoRelease(DataBase, IndexTreeRelease, false);
}
void DataBaseRelease(IndexTree* DataBase)
{
	IndexTreeRelease(DataBase);
}
void DataBaseAdd(IndexTree* DataBase,Client client)
{
	Client* p = MALLOC(Client, 1);
	*p = client;
	IndexTreeAdd(DataBase, (size_t)p->id, (size_t)p);
}
void DataBaseRemove(IndexTree* DataBase,Client* client)
{
	IndexTreeRemove(DataBase, client->id, NULL);
	free(client);
}
Client* DataBaseFind(IndexTree* DataBase,uint64_t id)
{
	Page* p = IndexTreeFind(DataBase, id);
	if (!p) return NULL;
	return (Client*)(p->data);
}

void ClientPrint(WCHAR* buffer, size_t buffer_size, Client* client)
{
	swprintf_s(buffer, buffer_size,
		L"[$:%d]",
		client->account);
}

void DataBasePrint(IndexTree* DataBase)
{
	IndexTreePrint(DataBase, ClientPrint);
}