#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999
#define NSIZE 50001


struct Dir { char name[NAME_MAXLEN + 1]; Dir *parent, *child, *prev, *next; }node[NSIZE], root; int nLen;


// The below commented functions are for your reference. If you want 
// to use it, uncomment these functions.
//Reference code start
int mstrcmp(const char *a, const char *b)
{
	int i;
	for (i = 0; a[i] != '\0'; i++)
	{
		if (a[i] != b[i]) {
			return a[i] - b[i];
		}
	}
	return a[i] - b[i];
}

void mstrcpy(char *dest, const char *src)
{
	int i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = src[i];
}
//Reference code end



void add(Dir *c, Dir *p) {
	c->parent = p;
	c->next = p->child;
	c->prev = 0;
	if (p->child) p->child->prev = c;
	p->child = c;
}

void del(Dir *c) {
	if (c->prev) c->prev->next = c->next;
	else c->parent->child = c->next;
	if (c->next) c->next->prev = c->prev;
}

Dir* addChild(Dir* p, char* name) {
	Dir *c = &node[nLen++];
	mstrcpy(c->name, name);
	c->child = 0;
	add(c, p);
	return c;
}

Dir* findChild(Dir *p, char* name) {

	for (Dir* c = p->child; c; c = c->next) {
		if (!mstrcmp(c->name, name)) return c;
	}
}

Dir* getDir(char *path) {
	Dir *cur = &root;
	int dirIdx = 0;
	int pathIdx = 0;
	char name[NAME_MAXLEN + 1];
	do {
		if (path[pathIdx] != '/') {
			name[dirIdx++] = path[pathIdx];
		}
		else if (dirIdx != 0) {
			name[dirIdx] = '\0';
			dirIdx = 0;
			cur = findChild(cur, name);
		}
	} while (path[++pathIdx] != '\0');
	return cur;
}



//MainCode
void init(int n) {
	nLen = 0;
	root = { 0,0,0,0,0 };
}

void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {
	addChild(getDir(path), name);
}

void cmd_rm(char path[PATH_MAXLEN + 1]) {
	del(getDir(path));
}

void _cp(Dir *s, Dir *d) {
	for (Dir *c = s->child; c; c = c->next) _cp(c, addChild(d, c->name));
}

void cmd_cp(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
	Dir *s = getDir(srcPath);
	Dir *d = getDir(dstPath);
	_cp(s, addChild(d, s->name));
}

void cmd_mv(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
	Dir *s = getDir(srcPath);
	Dir *d = getDir(dstPath);

	del(s);
	add(s, d);
}

int _find(Dir *p) {
	int cnt = 0;
	for (Dir *c = p->child; c; c = c->next) cnt += 1 + _find(c);
	return cnt;
}

int cmd_find(char path[PATH_MAXLEN + 1]) {
	return _find(getDir(path));
}