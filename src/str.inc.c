
typedef struct str {
	size_t length;
	uint8_t *ptr;
} str;
#define STR_C(l,p) ((str){(size_t)(l),(uint8_t*)(p)})

/*
str* str__dup_shallow(str *this) {
	str *ptr = (str*)calloc(1, sizeof(str));
	ptr->length = this->length;
	ptr->ptr = this->ptr;
	return ptr;
}

void str__dup2_shallow(str *src, str *dst) {
	if (dst->ptr) {
		free(ptr->ptr);
	}
	dst->length = src->length;
	dst->ptr = src->ptr;
}
*/

str* str__new(char* cstr) {
	uint8_t* c = (uint8_t*)cstr;
	str *ptr = (str*)calloc(1, sizeof(str));
	while (*c++) {
		ptr->length++;
	}
	ptr->ptr = (uint8_t*)calloc(ptr->length+1, sizeof(uint8_t*));
	memcpy(ptr->ptr, this->ptr, ptr->length);
	return ptr;
}

str* str__dup(str *this) {
	str *ptr = (str*)calloc(1, sizeof(str));
	ptr->length = this->length;
	ptr->ptr = (uint8_t*)calloc(ptr->length+1, sizeof(uint8_t*));
	memcpy(ptr->ptr, this->ptr, ptr->length);
	return ptr;
}

void str__dup2(str *src, str *dst) {
	if (dst->ptr) {
		free(ptr->ptr);
	}
	dst->length = src->length;
	dst->ptr = (uint8_t*)calloc(dst->length+1, sizeof(uint8_t*));
	memcpy(dst->ptr, src->ptr, src->length);
}


inline
void str__destroy(str *this) {
	free(this->ptr);
}
