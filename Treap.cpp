mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
struct rope{
    struct node{
        int val,sz,priority,lazy,rev,sum;
        node *l, *r, *par;
        node(): lazy(0),rev(0),val(0),sz(0),sum(0),r(NULL),l(NULL),par(NULL){}
        node(int _val): lazy(0),rev(0),val(_val),sum(_val),sz(1),r(NULL),l(NULL),par(NULL){
            priority = rng();}};
    typedef node* pnode;
    pnode root;
    void clear(){root=NULL;}
    rope(){clear();}
    int size(pnode p){ 
        return p ? p->sz : 0;}
    void update_size(pnode &p){ 
        if(p) p->sz=size(p->l)+size(p->r)+1;}
    void update_parent(pnode &p){
        if(!p) return;
        if(p->l) p->l->par=p;
        if(p->r) p->r->par=p;}
    void push(pnode &p){
        if(!p) return;      
        p->sum+=size(p)*p->lazy;
        p->val+=p->lazy;
        if(p->rev) swap(p->l,p->r);
        if(p->l){
            p->l->lazy+=p->lazy; 
            p->l->rev^=p->rev; }
        if(p->r){ 
            p->r->lazy+=p->lazy;
            p->r->rev^=p->rev;}
        p->lazy = 0;
        p->rev = 0;}
    void reset(pnode &t){
        if(t) t->sum=t->val; }
    void combine(pnode &t, pnode l, pnode r){
        if(!l){
            t = r;return;}
        if(!r){
            t = l;return;}
        t->sum = l->sum + r->sum;}
    void operation(pnode &t){
        if(!t) return;
        reset(t);
        push(t->l);
        push(t->r);
        combine(t, t->l, t);
        combine(t, t, t->r);}
    void split(pnode t, pnode &l, pnode &r, int k, int add = 0){
        if(t == NULL){ 
            l=r=NULL; return;}
        push(t);
        int idx = add + size(t->l);
        if(idx <= k) split(t->r, t->r, r, k, idx + 1), l = t;
        else split(t->l, l, t->l, k, add), r = t;
        update_parent(t);
        update_size(t);
        operation(t);}
    void merge(pnode &t, pnode l, pnode r){
        push(l);
        push(r);
        if(!l){
            t=r;return;}
        if(!r){ 
            t=l;return;}
        if(l->priority > r->priority) merge(l->r, l->r, r), t = l;
        else merge(r->l, l, r->l), t = r;
        update_parent(t);
        update_size(t);
        operation(t);}
    void insert(int pos, int val){
        if(root == NULL){
            pnode to_add = new node(val);
            root = to_add;
            return;}
        pnode l, r, mid;
        mid = new node(val);
        split(root, l, r, pos - 1);
        merge(l, l, mid);
        merge(root, l, r);}
    void erase(int qL, int qR){
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        merge(root, l, r);}
    int query(int qL, int qR){
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        int answer = mid->sum;
        merge(r, mid, r);
        merge(root, l, r);
        return answer;}
    void update(int qL, int qR, int val){
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        mid->lazy += val;
        merge(r, mid, r);
        merge(root, l, r);}
    void reverse(int qL, int qR){
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        mid->rev ^= 1;
        merge(r, mid, r);
        merge(root, l, r);}
    void cyclic_shift(int qL, int qR, int k){
        if(qL == qR) return;
        k %= (qR - qL + 1);
        pnode l, r, mid, fh, sh;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        split(mid, fh, sh, (qR - qL + 1) - k - 1);
        merge(mid, sh, fh);
        merge(r, mid, r);
        merge(root, l, r);}};
rope r;
r.insert(i,x);
r.cyclic_shift(lf,rt,1);
r.reverse(lf,rt);
r.query(x,x) <<" ";