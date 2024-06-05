#pragma once
#include<BaseNode.h>

class NodeManager
{
private:

	BaseNode* node_;
	BaseNode* nextNode_;

public:

	static NodeManager* GetInstance();

	void Initialize();
	void Update();
	void Draw();

	void ChangeNodeSelectLeft();
	void ChangeNodeSelectCenter();
	void ChangeNodeSelectRight();

private:

	NodeManager() = default;
	~NodeManager() = default;
};

