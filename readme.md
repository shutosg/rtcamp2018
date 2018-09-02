# Aozora-Renderer

![](https://drive.google.com/uc?id=1428cvbxj1RePjmNSv6xnMJeUk8vcjKcK&export=download)

- [レイトレ合宿6](https://sites.google.com/site/raytracingcamp6/)で制作したレンダラ
- VC++ 2017でのみ動作確認

## 機能
- 一般的な準モンテカルロパストレーシングによるレンダリング
- Scene.hppのディレクティブ `#USE_PATHTRACING` をコメントアウトすると古典的レイトレーシングに切り替わる
- プリミティブとして、球/ディスク/無限平面
- OBJファイルの読み込みに対応（3角ポリゴン、頂点情報と面情報のみ対応）
- その他詳細は[スライド](https://speakerdeck.com/shutosg/reitorehe-su-6-rendarashao-jie)を参照

## 注意
- 外部アセットはリポジトリに含まれません
  - ドラゴンのobjモデル
  - 青空のIBL素材画像
- そのまま実行しても上記のモデルと画像読み込みでコケて動きません
