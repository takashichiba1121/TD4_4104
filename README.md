## 目的

本コーディング規約はTD4および卒業制作における開発時に適用されます。

本コーディング規約はごく基本的な項目に限って記述しています。新しく出てきた不明点があれば、随時更新されます。

## プロジェクトの構成

| フォルダ名 | 用途 |
|-----------|-----------|
| GameCore      | dxlibなどの処理 |
| ThirdParty      | 外部ライブラリ |
| Include/Scene      | ゲームシーンなど |
| Include/Util      | その他 |


## 命名規則

変数名はローアーキャメルケースとします。  
定数名はアッパースネークケースとします。

クラス名は必ず大文字ではじめてください。  
プライベートメンバ変数は最後に_を付けてください

## コーディングスタイル

基本的なコーディング規約はAllmanスタイルに従います。

## 禁止事項

ビルド出来ない状態でのプッシュは禁止とします。  
必ずビルド出来る状態でプッシュしてください。  
  
ifやforのコードブロックを囲まないのは禁止とします。
```
if(hoge) return hogehoge;
```
ifやforを記述したらコードブロックを必ず囲んでください。

三項演算子の使用。  
newの使用。  
人のコードを勝手に変える。  
テキトーな変数名および関数名。  
構造体の値渡し。  
ブランチ名に日本語使用
コメントによる処理の説明は書かないでください。  
ただし必要であればその処理にした意図を書いて下さい。

## 制限事項

- シングルトンはなるべく使わない
- マジックナンバーはなるべく使わない

## 使用外部ライブラリ
- Dxlib
- IMGUI
- nlohmann-json
