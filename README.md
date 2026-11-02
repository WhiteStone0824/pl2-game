# PL2 CUI Game Engine

C言語製のターミナル向けゲームエンジンです。Unityに馴染みのある開発者が扱いやすいよう、`GameObject`、`Transform`、`Component`、`Start`、`Update`、`OnDestroy` の構造を用意しています。

## ディレクトリ構成

```text
GameEngine/  エンジン本体: コア、描画、入力、アニメーション、音声、衝突判定
Build/       CMake定義と生成物
src/         ゲーム側のソースコードと機能サンプル
GameData/    ゲーム用アセットとセーブデータ
```

主なエンジン内の分割は次のとおりです。

```text
GameEngine/include/game_engine.h       全公開APIの入口
GameEngine/include/game_engine/        型、Component、描画、物理、音声などのAPI
GameEngine/src/core/                   Engine、GameObject、入力、共通パス処理
GameEngine/src/render/                 Canvas、Sprite、SpriteRenderer、Camera
GameEngine/src/animation/              AnimationデータとAnimator
GameEngine/src/physics/                Collider設定と衝突クエリ
GameEngine/src/audio/                  AudioClipとAudioSource
GameEngine/src/save/                   SaveData管理とファイル形式
GameEngine/src/platform/{windows,posix}/ 端末、入力、音声のOS別実装
GameEngine/src/internal/               内部構造体の機能別定義
```

ゲーム側は `game_engine.h` だけをincludeすれば全公開APIを使えます。各機能のヘッダーを直接includeすることもできます。プロジェクト内のソース、include、生成先は相対パスで管理します。

## エンジン機能

- `GameObject` に `Transform` と複数の `Component` を追加し、名前や有効状態を管理
- Componentの `Start` / `Update` / `OnDestroy` ライフサイクル
- `SpriteRenderer` とRGB背景色を使うドット描画
- フレームごとの再生速度を持つ `Animation` と `Animator`
- `AudioSource` と相対パス指定の音声クリップ再生・ループ・停止
- Box、Circle、Polygon、Spriteの不透明ドットマスクによるコライダー
- 整数、実数、文字列のセーブデータと読み込み。`.tmp` へ書いてから置き換える方式
- 入力、カメラ、FPS制御、カスタムComponentの登録

Windows音声バックエンドはWAVを再生します。POSIXバックエンドは現状、端末ベルで通知します。Spriteは現在コードから作成するAPIを用意しており、テキスト画像ファイルのローダーはまだありません。

## ビルド

GCCとNinjaをPATHから使える場合:

```powershell
cmake -S . -B Build/Output/GCC -G Ninja -DCMAKE_C_COMPILER=gcc
cmake --build Build/Output/GCC
./Build/Output/GCC/pl2_game.exe
```

## サンプル

`src/main.c` はGameObject/Component、ループアニメーション、ポリゴンとドットマスクの重なり、WAV再生、セーブ/ロードを示します。矢印キーで円形オブジェクトを動かし、三角形のターゲットに重ねてスコアを増やせます。Spaceで音声、Pで保存、Oで読み込み、Qで終了します。

セーブファイルの例は `GameData/Saves/demo.sav` です。ファイルAPIは絶対パスと `..` を含むパスを拒否します。
