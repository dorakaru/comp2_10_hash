﻿#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"

static unsigned int hash_func(unsigned int key, int num)
{
	return key % num;
}

static unsigned int get_hash(const hash* h, unsigned int key)
{
	if (h == NULL) return ~0;
	if (key == ~0) return ~0;

	// ToDo: ハッシュ関数としてhash_funcを使った
	// オープンアドレス法によるハッシュ値を求める

	unsigned int ha = hash_func(key, h->max_size);
	unsigned int n = h->max_size;

	do {
		unsigned int k = h->nodes[ha].key;

		if (k == key)
			return ha;

		if (k == ~0)
			return ha;

		if (h->max_size <= ++ha)
		{
			ha = 0;
		}
	} while (0 < --n);

	return ~0;
}

// ハッシュの初期化(max_sizeは~0未満)
bool initialize(hash* h, unsigned int max_size)
{
	if (h == NULL) return false;
	if (max_size == 0 || max_size == ~0) return false;

	if (h->nodes != NULL) finalize(h);

	h->nodes = (hash_node*)malloc(max_size * sizeof(hash_node));
	if (NULL == h->nodes) return false;

	for (unsigned int i = 0; i < max_size; i++) {
		h->nodes[i].key = ~0;
		h->nodes[i].value[0] = '\0';
	}

	h->max_size = max_size;

	return true;
}

// 使用メモリの全解放
void finalize(hash* h)
{
	if (h == NULL) return;

	free(h->nodes);
	h->nodes = NULL;
	h->max_size = 0;
}

// keyの値を見てノードを追加する(追加できなかったらfalseを返す)
bool add(hash* h, unsigned int key, const char* value)
{
	if (h == NULL) return false;
	if (h->max_size == ~0) return false;
	if (key == ~0) return NULL;

	// ToDo: ハッシュ関数としてhash_funcを使った
	// オープンアドレス法によりキーを追加

	unsigned int ha = get_hash(h, key);

	if (ha == ~0)
		return false;

	if (h->nodes[ha].key == key)
		return false;

	hash_node* n = &h->nodes[ha];
	n->key = key;
	memcpy_s(n->value, 256, value, strnlen_s(value, 256) + 1);

	return true;
}

// keyの値を見てノードを検索して、値を取得する(なければNULLを返す)
const char* get(const hash* h, unsigned int key)
{
	if (key == ~0) return NULL;

	// ToDo: keyから値が格納されている場所を求め、値の場所を返す
	unsigned int ha = get_hash(h, key);

	if (ha == ~0)
		return NULL;

	if (h->nodes[ha].key == ~0)
		return NULL;

	return h->nodes[ha].value;

	return NULL;
}

// ハッシュの値を取得する(デバッグ用)
int debug_get_hash(const hash* h, unsigned int key)
{
	return get_hash(h, key);
}