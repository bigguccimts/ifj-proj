#define TRUE 1
#define FALSE 0

typedef struct ListElement {
	/** Užitečná data. */
    INT_T;
	FLOAT_T;
	STRING_T;
	/** Ukazatel na následující prvek seznamu. */
    struct ListElement *nextElement;
} *ListElementPtr;

/** Jednosměrně vázaný seznam. */
typedef struct {
	/** Ukazatel na první prvek seznamu. */
	ListElementPtr firstElement;
	/** Ukazatel na aktuální prvek seznamu. */
    ListElementPtr activeElement;
} List;