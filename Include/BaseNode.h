#include<vector>
#include<Vector2.h>

class BaseNode
{
public:

	enum Type
	{
		MONSTER,
		CAMPFIRE,
		SHOP,
		TREASURE,
		BOSS,
		NOT_ASSIGNED
	};

private:

	friend class NodeManager;

	BaseNode* previews_;
	std::vector<BaseNode*>nexts_;

	BaseNode* selectNode_;

	Vector2 position_;
	int32_t row_;
	int32_t column_;
	Type type_;

public:

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};

	BaseNode() = default;
	~BaseNode() = default;
};
