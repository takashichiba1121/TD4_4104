#pragma once
class NodeType
{
public:
	enum Type
	{
		REINFORCEMENT,//強化
		TRANSACTION,//取引
		BATTLE,//バトル
		SHOP,//アイテム
		HEALING,//回復
		START,//スタート
		NONE,//設定なし
		NO_CHILDREN,//子なし
		BOSS,

		TYPE_NUM
	};

	
	Type value;

};

