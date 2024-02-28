#pragma once

#include <stdio.h>
#include <cstdint>
#include <corecrt_malloc.h>
#include <sys/stat.h>
#include <direct.h>
#include <memory>

namespace CW {

	struct HashPair {
		size_t key;
		void *value;
	};

	class HashMap {
		public:
			inline HashMap(size_t count) {
				this->count = count;
				hash_pairs = (HashPair**) calloc(1, count * sizeof(HashPair));
			}

			inline void Put(size_t key, void *value, int data_size) {
				size_t hashed = Hash(key);
				
				HashPair *pair = (HashPair*) calloc(1, sizeof(HashPair));
				pair->key = key;
				pair->value = calloc (1, data_size);
				memcpy(pair->value, value, data_size);
				
				hash_pairs[hashed] = pair;
			}
			inline void* Get(size_t key) {
				size_t hashed = Hash(key);

				return hash_pairs[hashed]->value;
			}
			inline size_t Hash(size_t key) {
				return key % count;
			}
			inline void Clear() {
				for (int i = 0; i < count; i++) {
					if (hash_pairs[i] != 0) {
						//delete hash_pairs[i]->value;
						delete hash_pairs[i];
					}
					hash_pairs[i] = 0;
				}
			}
			inline size_t GetCount() {
				return count;
			}
			inline HashPair* GetRaw(size_t index) {
				return hash_pairs[index];
			}
		private:
			size_t count = 0;
			HashPair **hash_pairs;
	};

	void ConvertToRawData(const char *path, const char *variable_name, const char *output_file);
	char* ConvertWideCharToChar(const wchar_t *str);
	int StrToInt(char *str);
	double Random();
	int NumDigits(int x);
	int NumDigits(size_t x);
	size_t HashString(char* p);
	int FolderExists(char *path);
	int FolderCreate(char *path);
}