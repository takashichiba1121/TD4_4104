#include<vector>

class BaseNode
{
private:

	friend class NodeManager;

	BaseNode*previews_;
	std::vector<BaseNode*>nexts_;

	BaseNode* selectNode_;

public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	BaseNode() = default;
	~BaseNode() = default;
};
