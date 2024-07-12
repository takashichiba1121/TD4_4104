#include<vector>
#include<Vector2.h>

class BaseNode
{
public:

	enum Type
	{
		REINFORCEMENT,//強化
		TRANSACTION,//取引
		BATTLE,//バトル
		SHOP,//アイテム
		HEALING,//回復
		NONE,//設定なし
		NO_CHILDREN,//子なし

		TYPE_NUM
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

	virtual void Initialize()=0;
	virtual void Update() =0;
	virtual void Draw()=0;

	BaseNode() = default;
	~BaseNode() = default;
};
